#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <cmath>
#include <map>

struct arg_struct {
    char *character;
    int *thread_num;
    std::vector<double> *prob;
    pthread_mutex_t *semB;
    pthread_mutex_t *semP;
    pthread_cond_t *print;
    int *turn;
};

void* shannon(void* arguments);

int main(int argc, char* argv[]) {

    static pthread_mutex_t semB;
    static pthread_mutex_t semP;
    pthread_mutex_init(&semB, NULL);
    pthread_mutex_init(&semP, NULL);

    static pthread_cond_t print = PTHREAD_COND_INITIALIZER;

    std::string message;
    std::string prob;
    std::string symb;
    std::vector<char> characters{};
    std::map<char, int> symbols; 
    std::vector<double> probabilities{};
    int turn = 0;

    // gets the input line from standard in 
    std::getline(std::cin, message);

    // Adds each of the symbols to an unordered_map with its number of occurances
    for (int i = 0; i < message.length(); i++) { 
        symbols[message[i]]++; 
        characters.push_back(message[i]);
    }

    // calculates the probability for each symbol and stores it in a vector
    for (auto const& [key, val] : symbols)
        probabilities.push_back((double)val / message.length());

    /*
    Creates a dynamic array of structs for passing the data into the encoder
    each index holds a pointer to the probability and the cdf value
    */
    arg_struct args;
    args.prob = &probabilities;
    args.semB = &semB;
    args.semP = &semP;
    args.print = &print;
    args.turn = &turn;
    
    // Creates all of the threads and passes in the data
    pthread_t tid[probabilities.size()];    
    for (int i = 0; i < probabilities.size(); i++) {
        //critical section start
        pthread_mutex_lock(&semB);  

        args.thread_num = &i;
        args.character = &characters.at(i);

        pthread_create(&tid[i], NULL, shannon, &args); 
    }

    // Threads are all rejoined
    for (int i = 0; i < probabilities.size(); i++)
        pthread_join(tid[i], NULL);

    /*
    // Output according to assignment formatting
    int i = 0;
    std::cout << "SHANNON-FANO-ELIAS Codes:" << std::endl << std::endl;
    for (auto const& [key, val] : symbols) {
        std::cout << "Symbol " << key << ", Code: " << *args[i].code << std::endl;
        i++;
    }
    */
    return 0;
}

/* 
Encoding function takes in a pointer to the probability and the cdf value for a given symbol and saves the 
shannon code into the overwrites the probability value so it can be accessed by the main process
*/
void* shannon(void* arguments) {

    std::vector<double> cdf{};

    // The arguments come in as a void type so we must cast them back to the struct
    struct arg_struct* args = (arg_struct*)arguments;

    int t_num = *args->thread_num - 1;
    char symb = *args->character;

    // end the critical section within the child thread after copying the thread number into local variable in child thread
    pthread_mutex_unlock(args->semB);

    // calculates the modified cdf value for each symbol and adds it to a vector
    for (int i = 0; i < (*args->prob).size(); i++)
    {
        double tempCdf = 0.0;
        for (int j = 0; j < i; j++)
            tempCdf += (*args->prob).at(j);

        tempCdf += (*args->prob).at(i) / 2;
        cdf.push_back(tempCdf);
    }

    // ----------------------------------------------------------------------------------------------
    // Shannon encoding
    // ----------------------------------------------------------------------------------------------
    std::string binary = "";

    // Shannon encoding length formula
    int length = ceil(log2( 1 / (*args->prob).at(t_num))) + 1;

    // Iterates through the cdf and if the fractional part is a 1 it adds a 1 to the code otherwise 0
    while (length != 0) {
        length--;
        cdf.at(t_num) *= 2;
        int fraction = cdf.at(t_num);

        if (fraction == 1) {
            cdf.at(t_num) -= fraction;
            binary += '1';
        } else {
            binary += '0';
        }
    }

    // synchronized printing
    pthread_mutex_lock(args->semP);
    while(*args->turn != t_num){
        pthread_cond_wait(args->print, args->semP);
    }

    std::cout << "Symbol " << symb << ", " << "Code: " << binary << std::endl;

    (*args->turn)++;
    pthread_mutex_unlock(args->semP);
    pthread_cond_broadcast(args->print);

    return NULL;
}
