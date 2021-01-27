#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <zconf.h>

int main() {
    struct hostent *host = NULL;
    const char *hostname = "www.youtube.com";
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(80);
    host = gethostbyname(hostname);
    serverAddress.sin_addr = *((struct in_addr *) host->h_addr_list[0]);
    memset(&(serverAddress.sin_zero), '0', 8);
    int socketDescriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connect(socketDescriptor, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr)) == -1)
        printf("Enable to connect to server");

    else
        printf("Connected to server");
    close(socketDescriptor);
    return 0;
}