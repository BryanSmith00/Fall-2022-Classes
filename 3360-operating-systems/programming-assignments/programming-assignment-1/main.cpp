// Write your program here
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <bits/stdc++.h>
using namespace std;

int main() {

    char tempChar;
    string prob;
    vector<char> letters{};
    vector<string> probabilities{};
    vector<pair<double, char>> symbols{};

    // Reads in the first line of symbols
    cin.get(tempChar);
    while(tempChar != '\n') {
        letters.push_back(tempChar);
        cin.get(tempChar);
        if(tempChar == ' ')
            cin.get(tempChar);
    }

    // Reads in the second line of probabilities
    getline(cin, prob);
    int i = 0;
    while ((i = prob.find(' ')) != string::npos) {
        probabilities.push_back(prob.substr(0, i));
        prob = prob.substr(i + 1, string::npos);
    }
    if(i == string::npos)
        probabilities.push_back(prob);

    for (int i = 0; i < letters.size(); i++)
    {
        symbols.push_back(pair<double, char>(stod(probabilities.at(i)), letters.at(i)));
    }
    sort(symbols.begin(), symbols.end(), greater());
    

    
    return 0;
}