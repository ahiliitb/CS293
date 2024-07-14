#include "receiver.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include "mymap.h"
using namespace std;

int string_to_int(string &s)
{
    int sum = 0;
    int n = s.size();
    for (int i = 0; i < n; i++)
        sum = sum * 10 + (int(s[i]) - 48);
    return sum;
}

string convert_into_label(const vector<string> tokens, int n)
{
    mymap<int> stockMap;
    string currentStr = "";
    for (int i = 0; i < n - 2; i += 2)
    {
        string key = tokens[i];
        int value = stoi(tokens[i + 1]);
        stockMap.insert(key, value);
    }

    vector<pair<string, int>> sortedStock;
    vector<string> keys = stockMap.allkeys();
    for (auto x : keys)
    {
        sortedStock.push_back(make_pair(x, stockMap.getValue(x)));
    }
    sort(sortedStock.begin(), sortedStock.end(), [](const auto a, const auto b)
         {
             return a.first < b.first; // Sort alphabetically by key
         });
    string stockLabel;
    for (const auto &pair : sortedStock)
    {
        stockLabel += pair.first + to_string(pair.second);
    }
    return stockLabel;
}

void updateMapFromNewString_2(mymap<int> &existingMap, const std::string &newInput, mymap<int> & store)
{
    std::string key = "";
    int value = 0;

    for (size_t i = 0; i < newInput.length(); ++i)
    {
        if (isalpha(newInput[i]))
        {
            key += newInput[i];
        }
        if(newInput[i]=='$'){
            break;
        }
        else if (isdigit(newInput[i]) || (newInput[i] == '-' && isdigit(newInput[i + 1])))
        {
            value = stoi(newInput.substr(i))*store.getValue(newInput); // Extract the number following the label

            // Update the map
            int currentValue = existingMap.getValue(key);
            currentValue += value;
            existingMap.insert(key, currentValue);

            // Reset key and move 'i' to the end of the number
            key = "";
            while (i < newInput.length() && (isdigit(newInput[i]) || newInput[i] == '-'))
            {
                ++i;
            }
            --i; // Adjust 'i' to stay on the last digit
        }
    }
}

void updateMapFromNewString(mymap<int> &existingMap, const std::string &newInput)
{
    std::string key = "";
    int value = 0;

    for (size_t i = 0; i < newInput.length(); ++i)
    {
        if (isalpha(newInput[i]))
        {
            key += newInput[i];
        }
        else if (isdigit(newInput[i]) || (newInput[i] == '-' && isdigit(newInput[i + 1])))
        {
            value = stoi(newInput.substr(i)); // Extract the number following the label

            // Update the map
            int currentValue = existingMap.getValue(key);
            currentValue += value;
            existingMap.insert(key, currentValue);

            // Reset key and move 'i' to the end of the number
            key = "";
            while (i < newInput.length() && (isdigit(newInput[i]) || newInput[i] == '-'))
            {
                ++i;
            }
            --i; // Adjust 'i' to stay on the last digit
        }
    }
}
string separatePairs(const string &input)
{
    string separatedString;

    size_t i = 0;
    while (i < input.length())
    {
        // If it's an alphabet character
        if (input[i]=='$'){
            separatedString+=" ";
        }
        while (isalpha(input[i]))
        {
            separatedString += input[i++];
            // Append characters until a non-digit is found
        }
        
        if ((isdigit(input[i]) || input[i] == '-'))
        {
            separatedString += " ";
        }

        while (i < input.length() && (isdigit(input[i]) || input[i] == '-'))
        {

            separatedString += input[i++];
        }
        if (isalpha(input[i]))
        {
            separatedString += " ";
        }
        else
        {
            ++i;
        }
    }

    // Remove the last space
    if (!separatedString.empty())
    {
        separatedString.pop_back();
    }

    return separatedString;
}
////////////////////-----------------------------

void final_set_ofarbitrages(vector<int> net_result, mymap<int> &store, vector<string> mapKeys, mymap<int> &buy_sell)
{
    for (auto &x : net_result)
    {

        int price = store.getValue(mapKeys[x]);
        int s_b = buy_sell.getValue(mapKeys[x]);

        if (s_b == 2)
        {
            cout << separatePairs(mapKeys[x]) << price << " "<< "s" << endl;
        }
        else
        {
            cout << separatePairs(mapKeys[x]) << price << " "<< "b" << endl; // s/b idont know //value map price
        }
    }
}
int count_arbitrages(vector<int> net_result, mymap<int> &store, vector<string> mapKeys, mymap<int> &buy_sell, int inh)
{
    int price = 0;
    for (auto &x : net_result)
    {
        int n = mapKeys[x].length();
        if (inh==3){
            size_t pos = mapKeys[x].find('$');
            int d = stoi(mapKeys[x].substr(pos));
            int stru = buy_sell.getValue(mapKeys[x]);
            if(stru ==1){
            price -= store.getValue(mapKeys[x])*d;
            }
            else if(stru ==2){
                price += store.getValue(mapKeys[x])*d;
            }
        }
        else{
        int stru = buy_sell.getValue(mapKeys[x]);
        if (stru == 1)
        {
            price -= store.getValue(mapKeys[x]);
        }
        else if (stru == 2)
        {
            price += store.getValue(mapKeys[x]);
        }
    }
    }
    return price;
}
vector<string> splitStringwithhash(const string &text)
{
    vector<string> tokens;
    size_t start = 0;
    size_t end = text.find('#');
    int n = text.length();
    while (end != string::npos)
    {
        tokens.push_back(text.substr(start, end - start));
        start = end + 2;
        if (start < n - 1)
        {
            end = text.find('#', start);
        }
        else
        {
            break;
        }
    }

    if (text.back() != '$')
    {
        string temp = "";
        temp.push_back(text.back());
        tokens.push_back(temp);
    }

    return tokens;
}
vector<string> splitStringwithspace(const string &text)
{
    vector<string> tokens;
    size_t start = 0;
    size_t end = text.find(' ');
    int n = text.length();
    while (end != string::npos)
    {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
        if (start < n - 1)
        {
            end = text.find(' ', start);
        }
        else
        {
            break;
        }
    }

    string temp = "";
    temp.push_back(text.back());
    tokens.push_back(temp);

    return tokens;
}

void function2_(const string &receivedData)
{
    int pos = 0;
    string action = "";
    string stockName = "";
    int p = 0;
    mymap<int> store;
    mymap<int> buy_sell;
    int index_count = 0;
    vector<string> to_store_index;
    int count = 0;
    int l = receivedData.length();
    vector<string> lines = splitStringwithhash(receivedData);
    for (const auto line : lines)
    {
        vector<string> tokens = splitStringwithspace(line);
        int n = tokens.size();
        stockName = convert_into_label(tokens, n);
        p = stoi(tokens[n - 2]);
        action = tokens[n - 1];

        if (!store.getValue(stockName + "v"))
        { // inside check whehter this value is present if not present
            store.insert(stockName + "v", p);
            to_store_index.push_back(stockName + "v");
            if (action == "s")
                buy_sell.insert(stockName + "v", 1);
            else if (action == "b")
            {
                buy_sell.insert(stockName + "v", 2);
            }

            store.insert(stockName + "b", 0);

            store.insert(stockName + "s", 0);
        }
        else
        {
            if (action == "s")
            {
                if (store.getValue(stockName + "b"))
                {
                    if (p >= store.getValue(stockName + "b"))
                    {
                        p = -1;
                        // just dont store that varibale
                        // got cancelled
                    }
                    else if (p < store.getValue(stockName + "b"))
                    {
                        store.insert(stockName + "b", 0);
                        // store that cancelled value in it
                        // just store the value as o next iteration we
                    }
                }
                if (store.getValue(stockName + "s") == p && p != 0)
                {
                    store.insert(stockName + "s", 0);
                    // this also got cancelled
                    p = -1;
                    // got cancelled till now the others also
                }
                if (p != -1 && p < store.getValue(stockName + "v"))
                {
                    // if that is not cancelled till now
                    // still is greater
                    store.insert(stockName + "v", p);
                    to_store_index.push_back(stockName + "v");
                    buy_sell.insert(stockName + "v", 1);
                }
                else if (p != -1 && p >= store.getValue(stockName + "v"))
                {
                    // if that is not cancelled tiill now
                    // bu ti slower htan value
                    store.insert(stockName + "b", p);
                    to_store_index.push_back(stockName + "v");
                    buy_sell.insert(stockName + "b", 1);
                }
            }
            else if (action == "b")
            {
                if (store.getValue(stockName + "s"))
                {
                    if (p < store.getValue(stockName + "s"))
                    {
                        p = -1;
                    }
                    else if (p >= store.getValue(stockName + "s"))
                    {
                        store.insert(stockName + "s", 0);
                    }
                }
                if (store.getValue(stockName + "b") == p && p != 0)
                {
                    store.insert(stockName + "b", 0);
                    p = -1;
                }
                if (p != -1 && p > store.getValue(stockName + "v"))
                {
                    store.insert(stockName + "v", p);
                    to_store_index.push_back(stockName + "v");
                    buy_sell.insert(stockName + "v", 2);
                }
                else if (p != -1 && p <= store.getValue(stockName + "v"))
                {
                    store.insert(stockName + "s", p);
                    to_store_index.push_back(stockName + "v");
                    buy_sell.insert(stockName + "s", 2);
                }
            }
        }

        vector<string> mapKeys;
        for (int k = to_store_index.size() - 1; k >= 0; k--)
        {
            if (store.getValue(to_store_index[k]) != 0)
            {
                mapKeys.push_back(to_store_index[k]);
            }
        }

        int o = mapKeys.size();
        int totalSubsets = 1 << o;
        vector<int> net_result;
        int flag = 0;

        for (int i = 0; i < totalSubsets; ++i)
        { // check all subsets of data inflow of data
            flag = 0;
            vector<int> currentSubset;
            mymap<int> comp;
            for (int j = 0; j < o; ++j)
            {
                if (!(i & (1 << j)))
                {
                    updateMapFromNewString(comp, mapKeys[j]);
                    currentSubset.push_back(j);
                }
            }
            vector<string> keys = comp.allkeys();
            if (keys.size() == 0)
            {
                flag = 1;
                continue;
            }
            for (auto &x : keys)
            {
                if (comp.getValue(x) != 0)
                {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0)
            {
                net_result = currentSubset;
                break;
            }
        }
        if (flag == 1)
        {
            cout << "No Trade" << endl;
            continue; // if there is flag==1 then No trade
        }             //
        // there is no such arbitrage then output cout;
        int currnt_value = count_arbitrages(net_result, store, mapKeys, buy_sell, 2);
        if (currnt_value < 0)
        {
            cout << "No Trade" << endl;
            continue;
        }                                                             // restart from here
        final_set_ofarbitrages(net_result, store, mapKeys, buy_sell); // output pushed out
        count += currnt_value;                                        // count ++
        for (auto &x : net_result)
        {
            store.remove(mapKeys[x]);
            buy_sell.remove(mapKeys[x]); // deleting the all the map_headings from it // all entries in selling buying and value just delete all such
        }
    }
    cout << count << endl;
}
//////-----------------------------done part2 --------------------------------///////////////
void function_3(const string &recievedData)
{

    int pos = 0;
    string action = "";
    string stockName = "";
    string p = "";
    int quant = 0;
    mymap<int> store;
    mymap<int> buy_sell;
    int index_count = 0;
    vector<string> to_store_index;
    int count = 0; // arbitrages done in this variable
    int l = recievedData.length();
    vector<string> lines = splitStringwithhash(recievedData);
    for (const auto line : lines)
    {
        
        vector<string> tokens = splitStringwithspace(line);
        int n = tokens.size();
        stockName = convert_into_label(tokens, n - 1);
        // here convert into labels should come only when taking to n-3
        p =(tokens[n - 3]);
        action = tokens[n - 1];
        quant = stoi(tokens[n - 2]);
        stockName+="$";
        stockName += p;
        cout<<stockName<< <<endl;

        if (!store.getValue(stockName))
        {
            store.insert(stockName, quant);
            to_store_index.push_back(stockName);
            if (action == "s")
                buy_sell.insert(stockName, 1);
            else if (action == "b")
            {
                buy_sell.insert(stockName, 2);
            }
        }
        else
        {
            if (action == "s")
            {
                if (store.getValue(stockName))
                {
                    if ( buy_sell.getValue(stockName)==2)// if it is buy net
                    {
                        //it becomes sell order 
                        int q= store.getValue(stockName);
                        if (quant >q ){
                            buy_sell.insert(stockName, 1);
                            store.insert(stockName, quant-q);
                            to_store_index.erase(std::remove(to_store_index.begin(), to_store_index.end(), stockName), to_store_index.end());
    
                        }
                        else if(quant <q){
                            buy_sell.insert(stockName, 2);
                            store.insert(stockName, q-quant);
                            to_store_index.erase(std::remove(to_store_index.begin(), to_store_index.end(), stockName), to_store_index.end());
    

                        }
                        else{
                            store.remove(stockName);
                            buy_sell.remove(stockName);
                        }
                    }
                    else if ( buy_sell.getValue(stockName)==1)
                    {
                        int q = store.getValue(stockName);
                        buy_sell.insert(stockName, 1);
                        store.insert(stockName, quant+q);
                        to_store_index.erase(std::remove(to_store_index.begin(), to_store_index.end(), stockName), to_store_index.end());

                    }
                }
            }
            else if(action =="b"){
                if (store.getValue(stockName))
                {
                    if ( buy_sell.getValue(stockName)==1)// if it is buy net
                    {
                        //it becomes sell order 
                        int q = store.getValue(stockName);
                        if (quant >q ){
                            buy_sell.insert(stockName, 2);
                            store.insert(stockName, quant-q);
                            to_store_index.erase(std::remove(to_store_index.begin(), to_store_index.end(), stockName), to_store_index.end());
    
                            //to_store index is left
                        }
                        else if(quant <q){
                            buy_sell.insert(stockName, 1);
                            store.insert(stockName, q-quant);
                            to_store_index.erase(std::remove(to_store_index.begin(), to_store_index.end(), stockName), to_store_index.end());
    

                        }
                        else{
                            store.remove(stockName);
                            buy_sell.remove(stockName);
                        }
                    }
                    else if ( buy_sell.getValue(stockName)==2)
                    {
                        int q = store.getValue(stockName);
                        buy_sell.insert(stockName, 2);
                        store.insert(stockName, quant+q);
                        to_store_index.erase(std::remove(to_store_index.begin(), to_store_index.end(), stockName), to_store_index.end());

                    }
                }
            }
        }
        vector<string> mapKeys;
        for (int k =to_store_index.size()-1 ; k>=0; k--)
        {
            if (store.getValue(to_store_index[k]) != 0)
            {
                mapKeys.push_back(to_store_index[k]);
            }
        }
        int o = mapKeys.size();
        int totalSubsets = 1 << o;
        vector<int> net_result;
        int flag = 0;

        for (int i = 0; i < totalSubsets; ++i)
        { // check all subsets of data inflow of data
            flag = 0;
            vector<int> currentSubset;
            mymap<int> comp;
            for (int j = 0; j < o; ++j)
            {
                if (!(i & (1 << j)))
                {
                    updateMapFromNewString_2(comp, mapKeys[j], store);
                    currentSubset.push_back(j);
                }
            }
            vector<string> keys = comp.allkeys();
            if (keys.size() == 0)
            {
                flag = 1;
                continue;
            }
            for (auto &x : keys)
            {
                if (comp.getValue(x) != 0)
                {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0)
            {
                net_result = currentSubset;
                break;
            }
        }
        if (flag == 1)
        {
            cout << "No Trade" << endl;
            continue; // if there is flag==1 then No trade
        }            
        int currnt_value = count_arbitrages(net_result, store, mapKeys, buy_sell, 3);
        if (currnt_value < 0)
        {
            cout << "No Trade" << endl;
            continue;
        }
        final_set_ofarbitrages(net_result, store, mapKeys, buy_sell);                                        // output pushed out
        count += currnt_value; // count ++

    }
    cout<<count<<endl;
}
////////////////--------part_3=-------------------------

    //------------------------$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$####################################$$$$$$$$$$$$$$$$$$$$$$*---------------//
    void function_(const string &receivedData)
    {
        int pos = 0;
        string action = "";
        string stockName = "";
        int p = 0;
        mymap<int> buyMap;
        mymap<int> sellMap;
        mymap<int> value;

        int l = receivedData.length();

        while (pos != string::npos)
        {
            int next_hash = receivedData.find("#", pos);
            string text = receivedData.substr(pos, next_hash - pos);

            int spacePos = text.find(" ");
            if (spacePos != string::npos)
            {
                stockName = text.substr(0, spacePos);
                text = text.substr(spacePos + 1);
                spacePos = text.find(" ");
                if (spacePos != string::npos)
                {
                    string s = text.substr(0, spacePos);
                    action = text.substr(spacePos + 1);
                    p = string_to_int(s);
                }
            }

            if (next_hash != string::npos)
            {
                pos = next_hash + 2;
            }
            else
            {
                pos = next_hash;
            }
            if (pos > receivedData.length())
            {
                break;
            }
            ///////////////////////////----code main ----////////////
            if (!value.getValue(stockName))
            {
                value.insert(stockName, p);
                buyMap.insert(stockName, -1);
                sellMap.insert(stockName, -1);
                if (action == "s")
                {
                    cout << stockName << " " << p << " "
                         << "b" << endl;
                }
                else
                {
                    cout << stockName << " " << p << " "
                         << "s" << endl;
                }
            }
            else
            {
                if (action == "s")
                {
                    if (buyMap.getValue(stockName) + 1)
                    {
                        if (p >= buyMap.getValue(stockName))
                        {
                            p = -1;
                            cout << "No Trade" << endl;
                        }
                        else if (p < buyMap.getValue(stockName))
                        {
                            buyMap.insert(stockName, -1);
                        }
                    }
                    if (sellMap.getValue(stockName) == p && p != -1)
                    {
                        sellMap.insert(stockName, -1);
                        p = -1;
                        cout << "No Trade" << endl;
                    }
                    if (p != -1 && p < value.getValue(stockName))
                    {
                        value.insert(stockName, p);
                        cout << stockName << " " << p << " "
                             << "b" << endl;
                    }
                    else if (p != -1 && p >= value.getValue(stockName))
                    {
                        buyMap.insert(stockName, p);
                        cout << "No Trade" << endl;
                    }
                }
                else if (action == "b")
                {
                    if (sellMap.getValue(stockName) + 1)
                    {
                        if (p < sellMap.getValue(stockName))
                        {
                            p = -1;
                            cout << "No Trade" << endl;
                        }
                        else if (p >= sellMap.getValue(stockName))
                        {
                            sellMap.insert(stockName, -1);
                        }
                    }
                    if (buyMap.getValue(stockName) == p && p != -1)
                    {
                        buyMap.insert(stockName, -1);
                        p = -1;
                        cout << "No Trade" << endl;
                    }
                    if (p != -1 && p > value.getValue(stockName))
                    {
                        value.insert(stockName, p);
                        cout << stockName << " " << p << " "
                             << "s" << endl;
                    }
                    else if (p != -1 && p <= value.getValue(stockName))
                    {
                        sellMap.insert(stockName, p);
                        cout << "No Trade" << endl;
                    }
                }
            }
        }
    }

    int main()
    {
        Receiver rcv;
        bool founddollar = false;
        string message = rcv.readIML();

        while (!founddollar)
        {
            string message1 = rcv.readIML();
            message.append(message1);
            if (message.find('$') != string::npos)
            {
                founddollar = true;
            }
        }
        rcv.terminate();
        // function_(message);
        function_3(message);
        return 0;
    }