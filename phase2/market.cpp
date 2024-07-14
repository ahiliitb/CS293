#include "market.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include "custom_priority_queue.h"
#include "mymap.h"
using namespace std;

market::market(int argc, char **argv)
{

}

vector<string> parse_the_text(string s)
{
    int n = s.length();
    string temp = "";
    vector<string> ans;
    string prev = "";
    string stockname = "";
    for (int i = 0; i < n; i++)
    {
        if (s[i] == '#')
            continue;
        if (prev == "BUY" || prev == "SELL")
        {
            while (s[i] != '$')
            {
                stockname.push_back(s[i]);
                i++;
            }
            ans.push_back(stockname.substr(0, stockname.length()-1));
            prev = stockname;
            continue;
        }
        else if (s[i] == ' ')
        {
            ans.push_back(temp);
            prev = temp;
            temp = "";
            continue;
        }
        temp.push_back(s[i]);
    }
    ans.push_back(temp);
    return ans;
}



string arrange_linear_combination(string s)
{
    int n = s.length();
    vector<string> substring;
    bool intflag = 0;
    string temp = "";
    for (int i = 0; i < n; i++)
    {
        if(s[i] == ' ')
        {
            temp.push_back(s[i]);
            continue;
        }
        if ((s[i] - '0' < 10 && s[i] - '0' >= 0) || s[i] == 45)
        {
            intflag = 1;
        }
        if (intflag == 1 && !((s[i] - '0' < 10 && s[i] - '0' >= 0) || s[i] == 45))
        {
            intflag = 0;
            substring.push_back(temp);
            temp = "";
        }
        temp.push_back(s[i]);
    }
    temp.push_back(' ');
    substring.push_back(temp);
    sort(substring.begin(), substring.end());
    string ans = substring[0].substr(0, substring[0].length()-1);
    for (int i = 1; i < substring.size(); i++)
    {
        substring[i].pop_back();
        ans += " " + substring[i];
    }
    return ans;
}


void market::start()
{

    ifstream outputFile("output.txt");
    if (!outputFile.is_open())
    {
        cerr << "Error opening file: output.txt" << endl;
    }

    string fileContent((istreambuf_iterator<char>(outputFile)), istreambuf_iterator<char>());
    outputFile.close();

    vector<string> substringstemp;

    istringstream iss(fileContent);
    string line;
    while (getline(iss, line, '\n'))
    {
        substringstemp.push_back(line);
    }

    vector<string> Orders;
    int alpha = substringstemp.size();
    for (int i = 0; i < alpha - 4; i++)
    {
        Orders.push_back(substringstemp[i + 1]);
    }
    mymap<maxpriorityqueue> Buyorder;
    mymap<minpriorityqueue> Sellorder;

    long long Total_money_transfered = 0;
    long long Total_no_of_trade = 0, Total_share_traded = 0;
    mymap<long long> indivitual_money_transfered;
    mymap<long long> indivitual_buy_share_tranfered;
    mymap<long long> indivitual_sell_share_tranfered;


    for (int i = 0; i < Orders.size(); i++)
    {
        vector<string> temp = parse_the_text(Orders[i]);
        temp[3] = arrange_linear_combination(temp[3]);
        string stockname = temp[3];
        if (temp[2] == "BUY")
        {
            vector<vector<string>> residuestore;
            if (Sellorder.getValue(stockname).getsize() != 0)
            {
                int buystarttime = stoi(temp[0]), buystoptime = buystarttime + stoi(temp[6]);
                if (stoi(temp[6]) == -1)
                {
                    buystoptime = INT32_MAX;
                }
                else if (stoi(temp[6]) == 0)
                {
                    buystoptime = stoi(temp[0]);
                }
                int buyprice = stoi(temp[4]), buyquantity = stoi(temp[5]);
                string buyer = temp[1];
                while (true)
                {
                    buyquantity = stoi(temp[5]);
                    minpriorityqueue sellpq = Sellorder.getValue(stockname);
                    vector<string> tempvec = sellpq.getmin();
                    int sellstarttime = stoi(tempvec[0]), sellstoptime = sellstarttime + stoi(tempvec[6]);
                    if (stoi(tempvec[6]) == -1)
                    {
                        sellstoptime = INT32_MAX;
                    }
                    else if (stoi(tempvec[6]) == 0)
                    {
                        sellstoptime = stoi(tempvec[0]);
                    }
                    int sellprice = stoi(tempvec[4]), sellquantity = stoi(tempvec[5]);
                    string seller = tempvec[1];
                    if (buyprice >= sellprice && buystarttime >= sellstarttime && buystarttime <= sellstoptime)
                    {
                        // Deal done
                        int dealprice = sellprice;
                        if (sellstarttime == buystarttime)
                        {
                            int result = tempvec[1].compare(temp[1]);
                            if (result > 0)
                            {
                                dealprice = buyprice;
                            }
                        }
                        int dealquantity = min(buyquantity, sellquantity);
                        if (buyquantity >= sellquantity)
                        {

                            sellpq.removemin();
                            Sellorder.insert(stockname, sellpq);
                            temp[5] = to_string(buyquantity - sellquantity);
                            Total_money_transfered += dealquantity * dealprice;
                            Total_share_traded += dealquantity;
                            Total_no_of_trade++;
                            int x = indivitual_money_transfered.getintValue(buyer);
                            indivitual_money_transfered.insert(buyer, x - dealprice * dealquantity);

                            int y = indivitual_money_transfered.getintValue(seller);
                            indivitual_money_transfered.insert(seller, y + dealprice * dealquantity);

                            x = indivitual_buy_share_tranfered.getintValue(buyer);
                            indivitual_buy_share_tranfered.insert(buyer, x + dealquantity);

                            y = indivitual_sell_share_tranfered.getintValue(seller);
                            indivitual_sell_share_tranfered.insert(seller, y + dealquantity);
                            cout << buyer << " purchased " << dealquantity << " share of " << stockname << " from " << seller << " for $" << dealprice << "/share" << endl;
                            if (Sellorder.getValue(stockname).getsize() == 0)
                            {
                                break;
                            }
                        }
                        else
                        {
                            sellpq.removemin();
                            tempvec[5] = to_string(sellquantity - buyquantity);
                            sellpq.insert(tempvec);
                            Sellorder.insert(stockname, sellpq);
                            temp[5] = "0";

                            Total_money_transfered += dealquantity * dealprice;
                            Total_share_traded += dealquantity;
                            Total_no_of_trade++;
                            int x = indivitual_money_transfered.getintValue(buyer);
                            indivitual_money_transfered.insert(buyer, x - dealprice * dealquantity);

                            int y = indivitual_money_transfered.getintValue(seller);
                            indivitual_money_transfered.insert(seller, y + dealprice * dealquantity);

                            x = indivitual_buy_share_tranfered.getintValue(buyer);
                            indivitual_buy_share_tranfered.insert(buyer, x + dealquantity);

                            y = indivitual_sell_share_tranfered.getintValue(seller);
                            indivitual_sell_share_tranfered.insert(seller, y + dealquantity);
                            cout << buyer << " purchased " << dealquantity << " share of " << stockname << " from " << seller << " for $" << dealprice << "/share" << endl;
                            break;
                        }
                    }
                    else
                    {
                        residuestore.push_back(tempvec);
                        sellpq.removemin();
                        Sellorder.insert(stockname, sellpq);
                        if (Sellorder.getValue(stockname).getsize() == 0)
                        {
                            break;
                        }
                    }
                }
                minpriorityqueue sellpq = Sellorder.getValue(stockname);
                while (residuestore.size() != 0)
                {
                    sellpq.insert(residuestore.back());
                    residuestore.pop_back();
                }
                Sellorder.insert(stockname, sellpq);
            }
            if (temp[5] != "0")
            {
                if (Buyorder.getValue(stockname).getsize() == 0)
                {
                    maxpriorityqueue buypq;
                    buypq.insert(temp);
                    Buyorder.insert(stockname, buypq);
                }
                else
                {

                    maxpriorityqueue buypq = Buyorder.getValue(stockname);
                    buypq.insert(temp);
                    Buyorder.insert(stockname, buypq);
                }
            }
        }
        else
        {
            vector<vector<string>> residuestore;
            if (Buyorder.getValue(stockname).getsize() != 0)
            {
                int sellstarttime = stoi(temp[0]), sellstoptime = sellstarttime + stoi(temp[6]);
                if (stoi(temp[6]) == -1)
                {
                    sellstoptime = INT32_MAX;
                }
                else if (stoi(temp[6]) == 0)
                {
                    sellstoptime = stoi(temp[0]);
                }

                int sellprice = stoi(temp[4]), sellquantity = stoi(temp[5]);
                string seller = temp[1];
                while (true)
                {
                    sellquantity = stoi(temp[5]);
                    maxpriorityqueue buypq = Buyorder.getValue(stockname);
                    vector<string> tempvec = buypq.getmax();
                    int buystarttime = stoi(tempvec[0]), buystoptime = buystarttime + stoi(tempvec[6]);
                    if (stoi(tempvec[6]) == -1)
                    {
                        buystoptime = INT32_MAX;
                    }
                    else if (stoi(tempvec[6]) == 0)
                    {
                        buystoptime = stoi(tempvec[0]);
                    }

                    int buyprice = stoi(tempvec[4]), buyquantity = stoi(tempvec[5]);
                    string buyer = tempvec[1];
                    if (buyprice >= sellprice && sellstarttime >= buystarttime && sellstarttime <= buystoptime)
                    {
                        // Deal done
                        int dealprice = buyprice;
                        if (sellstarttime == buystarttime)
                        {
                            int result = tempvec[1].compare(temp[1]);
                            if (result > 0)
                            {
                                dealprice = sellprice;
                            }
                        }
                        int dealquantity = min(buyquantity, sellquantity);
                        if (buyquantity >= sellquantity)
                        {
                            buypq.removemax();
                            tempvec[5] = to_string(buyquantity - sellquantity);
                            buypq.insert(tempvec);
                            Buyorder.insert(stockname, buypq);
                            temp[5] = "0";
                            Total_money_transfered += dealquantity * dealprice;
                            Total_share_traded += dealquantity;
                            Total_no_of_trade++;
                            int x = indivitual_money_transfered.getintValue(buyer);
                            indivitual_money_transfered.insert(buyer, x - dealprice * dealquantity);

                            int y = indivitual_money_transfered.getintValue(seller);
                            indivitual_money_transfered.insert(seller, y + dealprice * dealquantity);

                            x = indivitual_buy_share_tranfered.getintValue(buyer);
                            indivitual_buy_share_tranfered.insert(buyer, x + dealquantity);

                            y = indivitual_sell_share_tranfered.getintValue(seller);
                            indivitual_sell_share_tranfered.insert(seller, y + dealquantity);
                            cout << buyer << " purchased " << dealquantity << " share of " << stockname << " from " << seller << " for $" << dealprice << "/share" << endl;
                            break;
                        }
                        else
                        {
                            buypq.removemax();
                            Buyorder.insert(stockname, buypq);
                            temp[5] = to_string(sellquantity - buyquantity);

                            Total_money_transfered += dealquantity * dealprice;
                            Total_share_traded += dealquantity;
                            Total_no_of_trade++;
                            int x = indivitual_money_transfered.getintValue(buyer);
                            indivitual_money_transfered.insert(buyer, x - dealprice * dealquantity);

                            int y = indivitual_money_transfered.getintValue(seller);
                            indivitual_money_transfered.insert(seller, y + dealprice * dealquantity);

                            x = indivitual_buy_share_tranfered.getintValue(buyer);
                            indivitual_buy_share_tranfered.insert(buyer, x + dealquantity);

                            y = indivitual_sell_share_tranfered.getintValue(seller);
                            indivitual_sell_share_tranfered.insert(seller, y + dealquantity);
                            cout << buyer << " purchased " << dealquantity << " share of " << stockname << " from " << seller << " for $" << dealprice << "/share" << endl;
                            if (Buyorder.getValue(stockname).getsize() == 0)
                            {
                                break;
                            }
                        }
                    }
                    else
                    {

                        residuestore.push_back(tempvec);
                        buypq.removemax();
                        Buyorder.insert(stockname, buypq);
                        if (Buyorder.getValue(stockname).getsize() == 0)
                        {
                            break;
                        }
                    }
                }
                maxpriorityqueue buypq = Buyorder.getValue(stockname);
                while (residuestore.size() != 0)
                {
                    buypq.insert(residuestore.back());
                    residuestore.pop_back();
                }
                Buyorder.insert(stockname, buypq);
            }

            if (temp[5] != "0")
            {
                // cout<<"ahil1"<<endl;
                if (Sellorder.getValue(stockname).getsize() == 0)
                {
                    minpriorityqueue sellpq;
                    sellpq.insert(temp);
                    Sellorder.insert(stockname, sellpq);
                }
                else
                {
                    minpriorityqueue sellpq = Sellorder.getValue(stockname);
                    sellpq.insert(temp);
                    Sellorder.insert(stockname, sellpq);
                }
            }
        }
    }
    cout << endl;
    cout << "---End of Day--- " << endl;
    cout << "Total Amount of Money Transferred: $" << Total_money_transfered << endl;
    cout << "Number of Completed Trades: " << Total_no_of_trade << endl;
    cout << "Number of Shares Traded: " << Total_share_traded << endl;
    // KarGoTravels bought 50 and sold 22 for a net transfer of $-3900
    vector<string> All_traders = indivitual_money_transfered.allkeys();
    for (int i = 0; i < All_traders.size(); i++)
    {
        cout << All_traders[i] << " bought " << indivitual_buy_share_tranfered.getintValue(All_traders[i]) << " and sold " << indivitual_sell_share_tranfered.getintValue(All_traders[i]) << " for a net transfer of $" << indivitual_money_transfered.getintValue(All_traders[i]) << endl;
    }
}
