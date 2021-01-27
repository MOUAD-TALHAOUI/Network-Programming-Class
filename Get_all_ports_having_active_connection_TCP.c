#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define nb_ports 65535

int main() {
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    inet_aton("127.0.0.1", &(serverAddress.sin_addr));
    memset(&(serverAddress.sin_zero), '0', 8);
    int socketDescriptor;
    for (int i = 0; i < nb_ports; i++) {
        socketDescriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        serverAddress.sin_port = htons(i);
        if (connect(socketDescriptor, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr)) != -1) {
            printf("Active connection on port : %d\n", ntohs(serverAddress.sin_port));
        }
    }
    return 0;
}