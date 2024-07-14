#include "receiver.h"
#include <iostream>
#include <string>
#include <map>
#include "mymap.h"
using namespace std;

int  string_to_int(string& s)
{
    int sum = 0;
    int n = s.size();
    for (int i = 0; i < n; i++)
        sum = sum * 10 + (int(s[i]) - 48); 
    return sum;
}

vector<string> getdatafromstring(string s)
{
    vector<string> ans(3);
    string temp = "";
    int k = 0;
    for (int i = 0; i < s.length(); i++)
    {
        if(s[i] == '#')
        {
            ans[k] = temp;
            break;
        }
        if(s[i] == ' ')
        {
            ans[k] = temp;
            temp = "";
            k++;
            continue;
        }
        temp.push_back(s[i]);
    }
    return ans;
    
}


void function_(const string& receivedData)
{
    vector<string> datastore = getdatafromstring(receivedData);
    stock_name = datastore[0];
    price = 
}

int main() {
    Receiver rcv;
    bool founddollar = false;
    string message = rcv.readIML();

    while (!founddollar) {
        string message1 = rcv.readIML();
        message.append(message1);
        if(message.find('$')!=string::npos){
            founddollar=true;
        }
    }
    rcv.terminate();
    function_(message);
    
    return 0;
}