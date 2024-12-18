#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <libpq-fe.h>

#define PORT 8080

struct Record {
    char name[50];
    int age;
};

void insert_into_db(struct Record record) {
    PGconn *conn = PQconnectdb("host=localhost user=postgres password=Arush@123 dbname=mydb");
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    char query[256];
    sprintf(query, "INSERT INTO records (name, age) VALUES ('%s', %d)", record.name, record.age);

    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "INSERT command failed: %s\n", PQerrorMessage(conn));
    } else {
        printf("Record inserted: %s, %d\n", record.name, record.age);
    }

    PQclear(res);
    PQfinish(conn);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct Record record;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Bind the socket to the IP and PORT
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Listen for client connections
    listen(server_fd, 3);

    printf("Server is listening on port %d...\n", PORT);

    // Accept a connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    // Receive the record from the client
    recv(new_socket, &record, sizeof(record), 0);

    // Insert the record into the database
    insert_into_db(record);

    // Send acknowledgment to the client
    char *ack = "Record inserted into database!";
    send(new_socket, ack, strlen(ack), 0);

    close(new_socket);
    close(server_fd);
    return 0;
}
