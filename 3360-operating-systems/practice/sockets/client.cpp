#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    if (argc < 3)
    {
        std::cerr << "usage " << argv[0] << "hostname port\n";
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        std::cerr << "ERROR opening socket";

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        std::cerr << "ERROR, no such host\n";
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // Triggers the server to accept the connection
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "ERROR connecting";
        exit(1);
    }

    std::cout << "Please enter the message: ";

    bzero(buffer, 256);
    fgets(buffer, 255, stdin);

    char message[] = "Bryan Smith";
    int sMessage = strlen(buffer);

    n = write(sockfd, &sMessage, strlen(buffer));

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
