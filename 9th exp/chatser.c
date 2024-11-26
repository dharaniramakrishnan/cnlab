#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> // For Windows socket programming
#include <ws2tcpip.h> // For sockaddr_in

#define SERVER_PORT 8081
#define BUFFER_SIZE 1024

void chat_server() {
    WSADATA wsaData;
    int sock;
    struct sockaddr_in server_address, client_address;
    int addr_len = sizeof(client_address);
    char buffer[BUFFER_SIZE];
    int bytes_received;

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
    server_address.sin_addr.s_addr = INADDR_ANY;  // Listen on any interface
    server_address.sin_port = htons(SERVER_PORT); // Set port number

    // Bind the socket to the address and port
    if (bind(sock, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Server is running and waiting for messages...\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE); // Clear buffer before each receive
        bytes_received = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &addr_len);

        if (bytes_received == SOCKET_ERROR) {
            printf("Error in receiving data. Error Code: %d\n", WSAGetLastError());
            continue;
        }

        printf("Client: %s\n", buffer); // Print received message from client

        // Respond back to the client
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin); // Get response from server
        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&client_address, addr_len);
    }

    // Cleanup
    closesocket(sock);
    WSACleanup();
}

int main() {
    chat_server();
    return 0;
}
