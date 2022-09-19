// Write your program here
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <cmath>
#include <map>
using namespace std;

struct arg_struct {
    string *prob;
    double cdf;
};

void* shannon(void* arguments);

int main(int argc, char* argv[]) {

    string prob;
    string symb;
    vector<string> symbols{};
    vector<string> probabilities{};
    vector<double> cdf{};

    /* 
      Reads in the two lines of input and separates them
      The symbols are stored into a string vector called symbols
      The probabilities are stored in a string vector called probabilities
    */
    getline(cin, symb);
    getline(cin, prob);
    int i, j = 0;
    while (((i = prob.find(' ')) != string::npos) && (j = symb.find(' ') != string::npos)) {
        probabilities.push_back(prob.substr(0, i));
        symbols.push_back(symb.substr(0, j));
        prob = prob.substr(i + 1, string::npos);
        symb = symb.substr(j + 1, string::npos);
    }
    if(i == string::npos) {
        probabilities.push_back(prob);
        symbols.push_back(symb);
    }

    /*
    Pre-calculates the modified CDF of the probabilities using stod() 
    and stores them in a double vector
    */
    for (int i = 0; i < probabilities.size(); i++)
    {
        double tempCdf = 0.0;
        for (int j = 0; j < i; j++)
            tempCdf += stod(probabilities.at(j));

        tempCdf += stod(probabilities.at(i)) / 2;
        cdf.push_back(tempCdf);
    }

    /*
    Creates a dynamic array of structs for passing the data into the encoder
    each index holds a pointer to the probability and the cdf value
    */
    arg_struct* args = new arg_struct[probabilities.size()];
    for (int i = 0; i < probabilities.size(); i++)
    {
        args[i].prob = &probabilities.at(i);
        args[i].cdf = cdf.at(i);
    }
    
    // Creates all of the threads and passes in the data
    pthread_t tid[probabilities.size()];    
    for (int i = 0; i < probabilities.size(); i++)
        pthread_create(&tid[i], NULL, shannon, (void *) &args[i]);

    // Threads are all rejoined
    for (int i = 0; i < probabilities.size(); i++)
        pthread_join(tid[i], NULL);
    
    // Output according to assignment formatting
    cout << "SHANNON-FANO-ELIAS Codes:" << endl << endl;
    for (int i = 0; i < probabilities.size(); i++)
        cout << "Symbol " << symbols.at(i) << ", Code: " << probabilities.at(i) << endl;
    
    return 0;
}

/* 
Encoding function takes in a pointer to the probability and the cdf value for a given symbol and saves the 
shannon code into the overwrites the probability value so it can be accessed by the main process
*/
void* shannon(void* arguments) {

    string binary = "";
    // The arguments come in as a void type so we must cast them back to the struct
    struct arg_struct* args = (arg_struct*)arguments;

    // Shannon encoding length formula
    int length = ceil(log2( 1 / stod(*args->prob))) + 1;
    //cout << "thread for prob: " << *args->prob << "length is " << length << endl;

    // Iterates through the cdf and if the fractional part is a 1 it adds a 1 to the code otherwise 0
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
    *args->prob = binary;
    return NULL;
}