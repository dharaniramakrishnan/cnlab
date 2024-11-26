// goclient.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 9090
#define WINDOW_SIZE 4
#define MAX_FRAMES 10
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    int frame = 0;

    // Initialize Winsock
    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        return 1;
    }

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code : %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Connection failed. Error Code : %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    printf("Connected to server on port %d.\n", PORT);

    // Send frames
    while (frame < MAX_FRAMES) {
        // Send frames in window
        for (int i = 0; i < WINDOW_SIZE && (frame + i) < MAX_FRAMES; i++) {
            snprintf(buffer, BUFFER_SIZE, "%d", frame + i);
            printf("Sending frame %d\n", frame + i);
            send(clientSocket, buffer, strlen(buffer), 0);
            Sleep(500); // Delay for demo purposes
        }

        // Receive acknowledgment
        memset(buffer, 0, BUFFER_SIZE);
        recv(clientSocket, buffer, BUFFER_SIZE, 0);
        int ack = atoi(buffer);

        if (ack == frame) {
            printf("ACK received for frame %d\n", ack);
            frame++;
        } else {
            printf("Timeout or NAK for frame %d. Resending...\n");
        }
    }

    printf("All frames sent successfully!\n");
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
