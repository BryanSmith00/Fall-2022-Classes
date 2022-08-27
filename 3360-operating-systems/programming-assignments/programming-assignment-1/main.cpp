// Write your program here
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <cmath>
using namespace std;

string shannon(double, double);

int main() {

    char tempChar;
    string prob;
    vector<char> symbols{};
    vector<string> probabilities{};
    vector<double> cdf{};

    // Reads in the first line of symbols
    cin.get(tempChar);
    while(tempChar != '\n') {
        symbols.push_back(tempChar);
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

    // Calculate the CDF of the probabilties
    for (int i = 0; i < probabilities.size(); i++)
    {
        double tempCdf = 0.0f;
        for (int j = 0; j < i; j++)
        {
            tempCdf += stod(probabilities.at(j));
        }
        tempCdf += stod(probabilities.at(i)) / 2;
        cdf.push_back(tempCdf);
    }

    for (int i = 0; i < probabilities.size(); i++)
    {
        cout << "Symbol " << symbols.at(i) << ", Code: " << shannon(stod(probabilities.at(i)), cdf.at(i)) << endl;
    }
    
    return 0;
}

string shannon(double prob, double cdf) {

    string binary = "";
    int length = ceil(log2( 1 / prob)) + 1;

    while (length != 0) {
        length--;

        cdf *= 2;
        
        int fract_bit = cdf;
        if (fract_bit == 1) {
        cdf -= fract_bit;
        binary.push_back(1 + '0');
        } else
        binary.push_back(0 + '0');
    }
    
    return binary;
}