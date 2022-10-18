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
#include <unordered_map>

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    std::unordered_map<char, int> symbols; 
    std::string fileName;
    std::string message;

    // if the client does not receive the hostname, port, and filename then it crashes
    if (argc < 4)
    {
        std::cerr << "usage " << argv[0] << "hostname port filename\n";
        exit(1);
    }

    // sets the port number to argument 3
    portno = atoi(argv[2]);

    // sets the file name to argument 4
    fileName = argv[3];

    // opens the input file and crashes if input cannot be opened
    std::fstream input(fileName);
    if(!input) {
        std::cerr << "input file could not be opened\n";
        exit(1);
    }

    // Get the input line from the file then close the file
    std::getline(input, message);
    input.close();

    // Adds each of the symbols to an unordered_map with its number of occurances
    // TODO make this work
    for (int i = 0; i < message.length(); i++) { 
        symbols[message[i]]++; 
    }


    // For each of the child processes to be able to store its value in a common memory location we'll likely need pipes or we can just use an array of arguments again
    // This is where we need to create a new thread for each symbol and pass the data to them
    // --------------------------------------------------------------------------------------



    // creates a new blank socket to talk to the server
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // if the socket cannot be created crashes the program
    if (sockfd < 0)
    {
        std::cerr << "ERROR opening socket";
        exit(1);
    }

    // if the hostname does not exist the program crashes
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        std::cerr << "ERROR, no such host\n";
        exit(1);
    }

    // sets up the server address information including the port number
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // internet family of protocols
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // tries to connect to the server with the socket and serv_addr struck with port, else crashes
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
