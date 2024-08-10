#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main() {
    int sock;
    int addr_len, bytes_read;
    char recv_data[1024];
    struct sockaddr_in server_addr, client_addr;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Bind");
        exit(1);
    }

    addr_len = sizeof(struct sockaddr);
    printf("\nUDPServer Waiting for client on port 5000\n");
    fflush(stdout);

    while (1) {
        bytes_read = recvfrom(sock, recv_data, 1024, 0, (struct sockaddr *)&client_addr, &addr_len);
        recv_data[bytes_read] = '\0';

        printf("\n(%s,%d) said: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), recv_data);
        fflush(stdout);

        // Send a response back to the client
        printf("Enter your reply: ");
        fgets(recv_data, 1024, stdin);
        sendto(sock, recv_data, strlen(recv_data), 0, (struct sockaddr *)&client_addr, addr_len);
    }

    close(sock);
    return 0;
}
