#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    if (argc < 3)
    {
        std::cerr << "usage " << argv[0] << "hostname port\n";
        exit(1);
    }

    portno = atoi(argv[2]);

    // Get the symbol from cin as a line then process it and store it before sending to the server
    std::string message;
    std::getline(std::cin, message);

    std::unordered_map<char, int> symbols; 

    // Adds each of the symbols to an unordered_map with its number of occurances
    for (int i = 0; i < message.length(); i++) { 
        symbols[message[i]]++; 
    }

    // For each of the child processes to be able to store its value in a common memory location we'll likely need pipes
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        std::cerr << "ERROR opening socket";
        exit(1);
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        std::cerr << "ERROR, no such host\n";
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // internet family of protocols
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // Triggers the server to accept the connection
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "ERROR connecting";
        exit(1);
    }

    // sends data to the socket sockfd in the form of n number of bytes
    n = write(sockfd, &message, message.length());

    if (n < 0)
    {
        std::cerr << "ERROR writing to socket";
        exit(1);
    }

    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);

    if (n < 0)
    {
        std::cerr << "ERROR reading from socket";
        exit(1);
    }

    std::cout << buffer << std::endl;
    close(sockfd);

    return 0;
}
