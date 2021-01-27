#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>


int main() {
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(80);
    inet_aton("127.0.0.1", &(serverAddress.sin_addr));
    memset(&(serverAddress.sin_zero), '0', 8);
    int socketDescriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (connect(socketDescriptor, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr)) == -1) {
        printf("Enable to connect to server port : %d", ntohs(serverAddress.sin_port));
        return EXIT_FAILURE;
    }
    printf("Active connection on port : %d\n", ntohs(serverAddress.sin_port));

    return EXIT_SUCCESS;
}