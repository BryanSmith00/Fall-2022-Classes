// Write your program here
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <cmath>
#include <map>

struct arg_struct {
    std::string* code;
    double* cdf;
    double *prob;
};

void* shannon(void* arguments);

int main(int argc, char* argv[]) {

    std::string message;
    std::string prob;
    std::string symb;
    std::map<char, int> symbols; 
    std::vector<double> probabilities{};
    std::vector<double> cdf{};
    std::vector<std::string> codes{};

    // gets the input line from standard in 
    std::getline(std::cin, message);

    // Adds each of the symbols to an unordered_map with its number of occurances
    for (int i = 0; i < message.length(); i++) { 
        symbols[message[i]]++; 
    }

    // calculates the probability for each symbol and stores it in a vector
    for (auto const& [key, val] : symbols)
        probabilities.push_back((double)val / message.length());

    // calculates the modified cdf value for each symbol and adds it to a vector
    for (int i = 0; i < probabilities.size(); i++)
    {
        double tempCdf = 0.0;
        for (int j = 0; j < i; j++)
            tempCdf += probabilities.at(j);

        tempCdf += probabilities.at(i) / 2;
        cdf.push_back(tempCdf);
        codes.push_back("");
    }

    /*
    Creates a dynamic array of structs for passing the data into the encoder
    each index holds a pointer to the probability and the cdf value
    */
    arg_struct* args = new arg_struct[probabilities.size()];
    for (int i = 0; i < probabilities.size(); i++)
    {
        args[i].prob = &probabilities.at(i);
        args[i].cdf = &cdf.at(i);
        args[i].code = &codes.at(i);
    }
    
    // Creates all of the threads and passes in the data
    pthread_t tid[probabilities.size()];    
    for (int i = 0; i < probabilities.size(); i++)
        pthread_create(&tid[i], NULL, shannon, (void *) &args[i]);

    // Threads are all rejoined
    for (int i = 0; i < probabilities.size(); i++)
        pthread_join(tid[i], NULL);
    
    // Output according to assignment formatting
    int i = 0;
    std::cout << "SHANNON-FANO-ELIAS Codes:" << std::endl << std::endl;
    for (auto const& [key, val] : symbols) {
        std::cout << "Symbol " << key << ", Code: " << *args[i].code << std::endl;
        i++;
    }
    
    return 0;
}

/* 
Encoding function takes in a pointer to the probability and the cdf value for a given symbol and saves the 
shannon code into the overwrites the probability value so it can be accessed by the main process
*/
void* shannon(void* arguments) {

    std::string binary = "";
    // The arguments come in as a void type so we must cast them back to the struct
    struct arg_struct* args = (arg_struct*)arguments;

            // ----------------------------------------------------------------------------------------------
            // Shannon encoding
            // ----------------------------------------------------------------------------------------------
            
            // Shannon encoding length formula
            int length = ceil(log2( 1 / *args->prob)) + 1;

            // Iterates through the cdf and if the fractional part is a 1 it adds a 1 to the code otherwise 0
            while (length != 0) {
                length--;
                *args->cdf *= 2;
                int fraction = *args->cdf;

                if (fraction == 1) {
                    *args->cdf -= fraction;
                    binary += '1';
                } else {
                    binary += '0';
                }
            }

    *args->code = binary;
    return NULL;
}