#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> // For Windows socket programming
#include <ws2tcpip.h> // For sockaddr_in

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

void chat_client() {
    WSADATA wsaData;
    int sock;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Create a socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost
    server_address.sin_port = htons(SERVER_PORT); // Set port number

    printf("Connected to server. Type messages below:\n");

    while (1) {
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin); // Get message from user

        // Send message to server
        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_address, sizeof(server_address));

        // Receive response from server
        memset(buffer, 0, BUFFER_SIZE);
        recvfrom(sock, buffer, BUFFER_SIZE, 0, NULL, NULL);
        printf("Server: %s\n", buffer); // Print server response
    }

    // Cleanup
    closesocket(sock);
    WSACleanup();
}

int main() {
    chat_client();
    return 0;
}
