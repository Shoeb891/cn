#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);
    char buffer[1024];
    int recv_len;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error opening socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change to the server's IP address
    server_addr.sin_port = htons(5000);

    while (1) {
        printf("Enter message to send to server (type 'exit' to quit): ");
        fgets(buffer, 1024, stdin);

        // Send message to server
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, server_len) < 0) {
            perror("Error sending data");
            exit(1);
        }

        // Check for exit command
        if (strncmp(buffer, "exit", 4) == 0)
            break;

        // Receive response from server
        recv_len = recvfrom(sockfd, buffer, 1024, 0, NULL, NULL);
        if (recv_len < 0) {
            perror("Error receiving data");
            exit(1);
        }

        buffer[recv_len] = '\0'; // Add null terminator to received message
        printf("Server's response: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
