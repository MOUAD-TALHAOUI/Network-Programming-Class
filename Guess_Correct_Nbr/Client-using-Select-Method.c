#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
    int descripteurSocket;
    char *request = calloc(2014, sizeof(char));
    char *response = calloc(2014, sizeof(char));
    struct sockaddr_in adresseServeur;
    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_port = htons(8015);
    inet_aton("127.0.0.1", &(adresseServeur.sin_addr));
    memset(&adresseServeur.sin_zero, '0', 8);
    descripteurSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connect(descripteurSocket, (struct sockaddr *) &adresseServeur, sizeof(struct sockaddr)) == -1) {
        perror("Can't connect to server");
        return EXIT_FAILURE;
    }
    memset(request, '\0', 1024);
    memset(response, '\0', 1024);
    while (recv(descripteurSocket, response, 1024, 0) > 0) {
        printf("%s", response);
        memset(response, '\0', 1024);
        gets(request);
        send(descripteurSocket, request, strlen(request), 0);
        memset(request, '\0', 1024);
    }
    close(descripteurSocket);
    return EXIT_SUCCESS;

}
