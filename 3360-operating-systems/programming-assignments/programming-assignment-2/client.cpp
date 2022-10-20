#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <map>
#include <vector>

struct arg_struct {
    std::string* code;
    char* hostname;
    int port;
    double* cdf;
    double *prob;
};

void* shannon(void* arguments);

int main(int argc, char *argv[])
{
    int portno;
    std::map<char, int> symbols; 
    std::string fileName;
    std::string message;
    std::vector<double> probabilities{};
    std::vector<double> cdf{};
    std::vector<std::string> codes{};

    // if the client does not receive the hostname and port then it crashes
    if (argc < 3)
    {
        std::cerr << "usage " << argv[0] << "hostname port filename\n";
        exit(1);
    }

    // sets the port number to argument 3
    portno = atoi(argv[2]);

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
    Creates a dynamic array of structs for passing the data into the thread
    each index holds a pointer to the probability and the cdf value as well as
    the hostname and port number for the socket
    */
    arg_struct* args = new arg_struct[probabilities.size()];
    for (int i = 0; i < probabilities.size(); i++)
    {
        args[i].prob = &probabilities.at(i);
        args[i].cdf = &cdf.at(i);
        args[i].hostname = argv[1];
        args[i].port = portno;
        args[i].code = &codes.at(i);
    }

    //----------------------------------------------------------------
    // creates a thread for each symbol and passes the arguments to it
    //----------------------------------------------------------------

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

void* shannon(void* arguments) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    char buffer[256];

    // The arguments come in as a void type so we must cast them back to the struct
    struct arg_struct* args = (arg_struct*)arguments;

    // creates a new blank socket to talk to the server
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // if the socket cannot be created crashes the program
    if (sockfd < 0)
    {
        std::cerr << "ERROR opening socket";
        exit(1);
    }

    // if the hostname does not exist the program crashes
    struct hostent *server = gethostbyname(args->hostname);
    if (server == NULL)
    {
        std::cerr << "ERROR, no such host\n";
        exit(1);
    }

    // sets up the server address information including the port number
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // internet family of protocols
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(args->port);

    // tries to connect to the server with the socket and serv_addr struck with port, else crashes
    // Triggers the server to accept the connection
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "ERROR connecting";
        exit(1);
    }

    // converts the probability and cdf into a character array in order to send it to the socket
    std::string message = std::to_string(*args->prob) + " " + std::to_string(*args->cdf);
    char char_array[message.length()];
    strcpy(char_array, message.c_str());

    // sends the message to the socket sockfd in the form of n number of bytes
    n = write(sockfd, &char_array, message.length());

    if (n < 0)
    {
        std::cerr << "ERROR writing to socket";
        exit(1);
    }

    // reads the code from the socket
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);

    if (n < 0)
    {
        std::cerr << "ERROR reading from socket";
        exit(1);
    }

    *args->code = buffer;
    close(sockfd);

    return 0;
}