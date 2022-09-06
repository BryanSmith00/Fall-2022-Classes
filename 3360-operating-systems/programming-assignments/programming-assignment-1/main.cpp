// Write your program here
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <cmath>
using namespace std;

struct arg_struct {
    string prob;
    double cdf;
};

void* shannon(void* arguments);

int main(int argc, char* argv[]) {

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

    //Thread time
    for (int i = 0; i < probabilities.size(); i++)
    {
        struct arg_struct args;
        args.prob = probabilities.at(i);
        args.cdf = cdf.at(i);

        pthread_t id;
        pthread_create(&id, NULL, shannon, (void *) &args);
        pthread_join(id, NULL);
    }
    
    return 0;
}

void* shannon(void* arguments) {

    string binary = "";
    struct arg_struct* args = (arg_struct*)arguments;

    int length = ceil(log2( 1 / stod(args->prob))) + 1;

    while (length != 0) {
        length--;

        args->cdf *= 2;
        
        int fraction = args->cdf;
        if (fraction == 1) {
            args->cdf -= fraction;
            binary += '1';
        } else {
            binary += '0';
        }
    }
    cout << binary << endl;
    //prob = binary;
}