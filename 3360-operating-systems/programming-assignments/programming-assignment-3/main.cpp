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

    // semaphore for copying the argument info to the child thread
    static pthread_mutex_t semB;

    // semaphore for synchronizing the printing
    static pthread_mutex_t semP;

    // initializes both of the semaphores to NULL
    pthread_mutex_init(&semB, NULL);
    pthread_mutex_init(&semP, NULL);

    // condition variable used with semP for syncing print
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
    }

    // calculates the probability for each symbol and stores it in a vector
    for (auto const& [key, val] : symbols) {
        probabilities.push_back((double)val / message.length());
        characters.push_back(key);
    }

    //prints the first line of output
    std::cout << "SHANNON-FANO-ELIAS Codes:" << std::endl << std::endl;

    // creates the argument struct and sets the needed values
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

        // dynamically assigns the thread number and character to the struct
        args.thread_num = &i;
        args.character = &characters.at(i);

        // creates the threads and passes the argument struct in
        pthread_create(&tid[i], NULL, shannon, &args); 
    }

    // Threads are all rejoined
    for (int i = 0; i < probabilities.size(); i++)
        pthread_join(tid[i], NULL);

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
    std::vector<double> prob = *args->prob;

    // end the critical section within the child thread after copying the thread number into local variable in child thread
    pthread_mutex_unlock(args->semB);

    // calculates the modified cdf value for each symbol and adds it to a vector
    for (int i = 0; i < prob.size(); i++)
    {
        double tempCdf = 0.0;
        for (int j = 0; j < i; j++)
            tempCdf += prob.at(j);

        tempCdf += prob.at(i) / 2;
        cdf.push_back(tempCdf);
    }

    // ----------------------------------------------------------------------------------------------
    // Shannon encoding
    // ----------------------------------------------------------------------------------------------
    std::string binary = "";

    // Shannon encoding length formula
    int length = ceil(log2( 1 / prob.at(t_num)) + 1);

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

    // locks the printing semaphore
    pthread_mutex_lock(args->semP);

    // similar to exam 2 waits on condition print until the turn is correct
    while(*args->turn != t_num){
        pthread_cond_wait(args->print, args->semP);
    }

    // once it is a threads turn it prints
    std::cout << "Symbol " << symb << ", " << "Code: " << binary << std::endl;

    // increases the turn by reference
    (*args->turn)++;

    // unlocks the printing semaphore
    pthread_mutex_unlock(args->semP);

    // wakes up the condition variable print
    pthread_cond_broadcast(args->print);

    return NULL;
}
