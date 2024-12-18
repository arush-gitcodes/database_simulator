#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

struct Record {
    char name[50];
    int age;
};

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    struct Record record;

    // Input record data
    printf("Enter name: ");
    scanf("%s", record.name);
    printf("Enter age: ");
    scanf("%d", &record.age);

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Server address setup
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // Connect to the server
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Send the record to the server
    send(sock, &record, sizeof(record), 0);

    // Receive acknowledgment
    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    printf("Server: %s\n", buffer);

    close(sock);
    return 0;
}
