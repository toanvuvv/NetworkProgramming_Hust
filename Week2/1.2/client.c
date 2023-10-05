#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3001 /*port*/

void send_file(FILE *fp, int sockfd) {
    int n;
    char sendline[MAXLINE] = {0};

    while ((n = fread(sendline, 1, MAXLINE, fp)) > 0) {
        if (send(sockfd, sendline, n, 0) == -1) {
            perror("Error in sending file");
            exit(1);
        }
        memset(sendline, 0, MAXLINE);
    }
}

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    FILE *fp;
    char *filename = "file.txt";

    // basic check of the arguments
    if (argc != 2) {
        perror("Usage: TCPClient <IP address of the server>");
        exit(1);
    }

    if ((fp = fopen(filename, "r")) == NULL) {
        perror("Could not open file for reading");
        exit(1);
    }

    // Create a socket for the client
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Problem in creating the socket");
        exit(2);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    // Connection of the client to the socket
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Problem in connecting to the server");
        exit(3);
    }

    // Send the file
    send_file(fp, sockfd);

    printf("File sent successfully.\n");

    fclose(fp);
    close(sockfd);

    return 0;
}
