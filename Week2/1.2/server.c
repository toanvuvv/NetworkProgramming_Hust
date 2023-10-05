#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>  

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3001 /*port*/

void receive_file(int sockfd) {
    int n;
    FILE *fp;
    char *filename = "received_file.txt";
    char buffer[MAXLINE];

    fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error in creating file");
        exit(1);
    }

    while ((n = recv(sockfd, buffer, MAXLINE, 0)) > 0) {
        fwrite(buffer, 1, n, fp);
        memset(buffer, 0, MAXLINE);
    }

    printf("File received successfully.\n");
    fclose(fp);
}

int main() {
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    int n;

    // socket create and verification
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    memset(&servaddr, 0, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
        perror("Socket binding failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully bound..\n");

    // Now server is ready to listen and verification
    if ((listen(listenfd, 10)) != 0) {
        perror("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    while (1) {
        if ((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) < 0) {
            perror("server accept failed...\n");
            exit(0);
        }
        else
            printf("server accept the client...\n");

        receive_file(connfd);
        close(connfd);
    }
    close(listenfd);
}
