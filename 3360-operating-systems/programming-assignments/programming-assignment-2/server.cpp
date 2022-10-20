// A simple server in the internet domain using TCP
// The port number is passed as an argument

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <wait.h>
#include <math.h>

void fireman(int)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    // fireman function deals with zombie threads
    signal(SIGCHLD, fireman);

    // if no port number is provided the program crashes
    if (argc < 2)
    {
        std::cerr << "ERROR, no port provided\n";
        exit(1);
    }

    // creates a new blank socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // if the blank socket could not be created the program crashes
    if (sockfd < 0)
    {
        std::cerr << "ERROR opening socket";
        exit(1);
    }

    // sets all the bytes of the server address to 0
    bzero((char *)&serv_addr, sizeof(serv_addr));

    // port number is set to the command argument
    portno = atoi(argv[1]);

    // setup for serv_addr struct, sets the port number to portno
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // binds the empty socket to the port given by the argument (crashes if binding fails)
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "ERROR on binding";
        exit(1);
    }

    // listens for the number of requests here (should be as many as are symbols)
    listen(sockfd, 5);

    clilen = sizeof(cli_addr);

    while (true)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
        if (fork() == 0)
        {
            if (newsockfd < 0)
            {
                std::cerr << "ERROR on accept";
                exit(1);
            }

            bzero(buffer, 256);
            n = read(newsockfd, buffer, 255);

            if (n < 0)
            {
                std::cerr << "ERROR reading from socket";
                exit(1);
            }

            // converts the read buffer back into the double values for prob and cdf
            std::string buff = buffer;
            double prob = stod(buff.substr(0, buff.find(" ")));
            double cdf = stod(buff.substr(buff.find(" "), std::string::npos));

            // ----------------------------------------------------------------------------------------------
            // Shannon encoding
            // ----------------------------------------------------------------------------------------------
            std::string binary = "";

            // Shannon encoding length formula
            int length = ceil(log2( 1 / prob)) + 1;

            // Iterates through the cdf and if the fractional part is a 1 it adds a 1 to the code otherwise 0
            while (length != 0) {
                length--;
                cdf *= 2;
                int fraction = cdf;

                if (fraction == 1) {
                    cdf -= fraction;
                    binary += '1';
                } else {
                    binary += '0';
                }
            }

            // converts the code into a character array in order to send it to the socket
            char char_array[binary.length()];
            strcpy(char_array, binary.c_str());
            
            // writes the code to the socket
            n = write(newsockfd, &char_array, binary.length());

            if (n < 0)
            {
                std::cerr << "ERROR writing to socket";
                exit(1);
            }

            close(newsockfd);
            _exit(0);
        }
    }
    close(sockfd);
    return 0;
}
