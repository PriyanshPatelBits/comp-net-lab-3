#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFERSIZE 250

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address configuration
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12134);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Send message to server
    char msg[BUFFERSIZE];
    printf("Enter message to send to server: ");
    fgets(msg, BUFFERSIZE, stdin);

    int bytesSent = send(clientSocket, msg, strlen(msg), 0);
    if (bytesSent != strlen(msg))
    {
        perror("error while sending message to server");
        exit(EXIT_FAILURE);
    }

    // Receive response from server
    char response[BUFFERSIZE];
    int bytesReceived = recv(clientSocket, response, BUFFERSIZE, 0);
    if (bytesReceived < 0)
    {
        perror("error while receiving response from server");
        exit(EXIT_FAILURE);
    }

    response[bytesReceived] = '\0';
    printf("Response from server: %s\n", response);

    // Close socket
    close(clientSocket);

    return 0;
}
