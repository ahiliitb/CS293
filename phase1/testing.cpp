#include "receiver.h"
#include <iostream>
#include<algorithm>
#include <string>
#include "mymap.h"
using namespace std;

string convert_into_label(const vector<string>& tokens) {
    mymap<int> stockMap;
    string currentStr = "";
    int n = tokens.size();
    for (int i = 0; i < n-2; i+=2) {
        string key = tokens[i];
        int value = stoi(tokens[i+1]);
        stockMap.insert(key,value);
    }
    
    vector<pair<string, int>> sortedStock;
    vector<string> keys = stockMap.allkeys();
    for(auto & x:keys){
        sortedStock.push_back(make_pair(x, stockMap.getValue(x)));
    }
    sort(sortedStock.begin(), sortedStock.end(), [](const auto& a, const auto& b) {
        return a.first < b.first; // Sort alphabetically by key
    });
    string stockLabel;
    for (const auto& pair : sortedStock) {
        stockLabel += pair.first + to_string(pair.second);
    }
    stockLabel+=tokens.back();
    return stockLabel;
}

// vector<string> splitString(const string& text, char delimiter) {
//     vector<string> tokens;
//     size_t start = 0;
//     size_t end = text.find(delimiter);

//     while (end != string::npos) {
//         tokens.push_back(text.substr(start, end - start));
//         start = end + 2;
//         if(start < n)end = text.find(delimiter, start);
//         else break;
//     }

//     // tokens.push_back(text.substr(start-1));
//     return tokens;
// }

// int main() {
//     string s = "$";
//     cout<<s.find('#', 0);
//     return 0;
// }
// #include <iostream>
// #include <string>

string removeNewlines( string input) {
    string output = input;

    size_t pos = output.find('^M');
    while (pos != string::npos) {
        output.replace(pos, 1, ""); // Replace '\n' with an empty string
        pos = output.find('^M', pos); // Find the next occurrence
    }

    return output;
}

int main() {
    string s = "ahil^Mkhan";
    cout<<removeNewlines(s);
}
