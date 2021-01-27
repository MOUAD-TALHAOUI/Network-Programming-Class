#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
    int descripteurSocket;
    char *request = calloc(2048, sizeof(char));
    char *response = calloc(2048, sizeof(char));
    struct sockaddr_in adresseServeur;
    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_port = htons(8080);
    inet_aton("127.0.0.1", &(adresseServeur.sin_addr));
    memset(&adresseServeur.sin_zero, '0', 8);
    descripteurSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connect(descripteurSocket, (struct sockaddr *) &adresseServeur, sizeof(struct sockaddr)) == -1) {
        perror("Can't connect to server");
        return EXIT_FAILURE;
    }
    fd_set inputs, allInputs;
    FD_ZERO(&allInputs);
    FD_ZERO(&inputs);
    FD_SET(fileno(stdin), &allInputs);
    FD_SET(descripteurSocket, &allInputs);
    memset(request, '\0', 2048);
    while (1) {
        FD_ZERO(&inputs);
        inputs = allInputs;
        select(4, &inputs, NULL, NULL, NULL);
        memset(request, '\0', 2048);
        if (FD_ISSET(fileno(stdin), &inputs)) {
            gets(request);
            send(descripteurSocket, request, strlen(request), 0);
            memset(request, '\0', 2048);
        } else if (FD_ISSET(descripteurSocket, &inputs)) {
            memset(response, '\0', 2048);
            recv(descripteurSocket, response, 2048, 0);
            printf("%s\n", response);
            memset(response, '\0', 2048);
        }
    }
    close(descripteurSocket);
    return EXIT_SUCCESS;
}
