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
            std::cout << "Here is the message: " << buffer << std::endl;
            n = write(newsockfd, "I got your message", 18);
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
