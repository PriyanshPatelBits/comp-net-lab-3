#include <stdio.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CLIENTS 3
#define MAXPENDING 5
#define BUFFERSIZE 250

sem_t x, y;
struct sockaddr_in clientAddresses[MAX_CLIENTS];
int clientCount = 0;

void *clientRun(void *args)
{
    char msg[BUFFERSIZE];
    int clientSocket = *((int *)args); // Corrected argument type
    int temp2 = recv(clientSocket, msg, BUFFERSIZE, 0);
    if (temp2 < 0)
    {
        printf("Problem in temp 2");
        exit(0);
    }
    msg[temp2] = '\0';
    printf("%s\n", msg);
    printf("ENTER MESSAGE FOR CLIENT\n");
    fgets(msg, BUFFERSIZE, stdin); // Safe alternative to gets
    int bytesSent = send(clientSocket, msg, strlen(msg), 0);
    if (bytesSent != strlen(msg))
    {
        printf("Error while sending message to client");
        exit(0);
    }
    clientCount--;
    close(clientSocket);
    pthread_exit(NULL);
}

int main()
{
    int serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket < 0)
    {
        printf("Error while server socket creation");
        exit(0);
    }
    printf("Server Socket Created\n");

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12134);
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Use INADDR_ANY directly

    printf("Server address assigned\n");
    int temp = bind(serverSocket, (struct sockaddr *)&serverAddress,
                    sizeof(serverAddress));
    if (temp < 0)
    {
        printf("Error while binding\n");
        exit(0);
    }
    printf("Binding successful\n");

    int temp1 = listen(serverSocket, MAXPENDING);
    if (temp1 < 0)
    {
        printf("Error in listen");
        exit(0);
    }
    printf("Now Listening\n");

    while (1)
    {
        if (clientCount < MAX_CLIENTS)
        {
            int clientLength = sizeof(clientAddresses[clientCount]);
            int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddresses[clientCount], &clientLength);
            if (clientSocket < 0) // Corrected condition
            {
                printf("Error in client socket");
                exit(0);
            }
            else
            {
                printf("New client connected with client IP: %s\n", inet_ntoa(clientAddresses[clientCount].sin_addr));
            }
            pthread_t t;
            if (pthread_create(&t, NULL, clientRun, (void *)&clientSocket) != 0) // Corrected argument
            {
                printf("Thread creation failed\n");
                exit(EXIT_FAILURE);
            }
            clientCount++;
        }
        else
        {

            while (1)
            {
                if (clientCount < MAX_CLIENTS)
                {
                    break;
                }
                else
                {
                    continue;
                }
            }
        }
    }

    close(serverSocket);
    return 0;
}
