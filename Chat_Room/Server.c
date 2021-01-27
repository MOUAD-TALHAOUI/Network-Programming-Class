#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
    int descripteurSocketServeur, descripteurSocketClient;
    int descripteurSocketsClients[20];
    int n, compteurClient, maxDescripteurSocket, i, j;
    fd_set readfDescripteurSocket, readAllDescripteursSocket;
    char *request = calloc(2048, sizeof(char));
    char *response = calloc(2048, sizeof(char));
    char *clientTalking = calloc(2048, sizeof(char));

    struct sockaddr_in adresseServeur, adressClient;

    socklen_t longeurAdresseClient = sizeof(struct sockaddr);
    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_port = htons(8080);
    inet_aton("127.0.0.1", &(adresseServeur.sin_addr));
    memset(&adresseServeur.sin_zero, '0', 8);
    descripteurSocketServeur = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (bind(descripteurSocketServeur, (struct sockaddr *) &adresseServeur, sizeof(struct sockaddr)) == -1) {
        perror("Probleme de bind");
        return EXIT_FAILURE;
    }
    if (listen(descripteurSocketServeur, 5) == -1) {
        perror("Probleme de bind");
        return EXIT_FAILURE;
    }

    compteurClient = 0;

    FD_ZERO(&readAllDescripteursSocket);
    FD_SET(descripteurSocketServeur, &readAllDescripteursSocket);
    maxDescripteurSocket = descripteurSocketServeur;
    while (1) {
        FD_ZERO(&readfDescripteurSocket);
        readfDescripteurSocket = readAllDescripteursSocket;
        select(maxDescripteurSocket + 1, &readfDescripteurSocket, NULL, NULL, NULL);
        if (FD_ISSET(descripteurSocketServeur, &readfDescripteurSocket)) {
            descripteurSocketClient = accept(descripteurSocketServeur, (struct sockaddr *) &adressClient,
                                             &longeurAdresseClient);
            compteurClient++;
            FD_SET(descripteurSocketClient, &readAllDescripteursSocket);
            descripteurSocketsClients[compteurClient - 1] = descripteurSocketClient;
            if (descripteurSocketClient > maxDescripteurSocket) maxDescripteurSocket = descripteurSocketClient;
            printf("New client via socket descriptor n° %d \n", descripteurSocketClient);
        } else {
            for (i = 0; i < compteurClient; i++) {
                descripteurSocketClient = descripteurSocketsClients[i];
                if (FD_ISSET(descripteurSocketClient, &readfDescripteurSocket)) {
                    memset(request, '\0', 2048);
                    n = recv(descripteurSocketClient, request, 2048, 0);
                    if (n == 0) {
                        memset(request, '\0', 2048);
                        memset(response, '\0', 2048);
                        memset(clientTalking, '\0', 2048);
                        FD_CLR(descripteurSocketClient, &readAllDescripteursSocket);
                        for (j = i; j < compteurClient - 1; j++)
                            descripteurSocketsClients[j] = descripteurSocketsClients[j + 1];
                        compteurClient--;
                        close(descripteurSocketClient);
                        if (descripteurSocketClient == maxDescripteurSocket && compteurClient != 0) {
                            for (j = 1; j < compteurClient; j++) {
                                if (descripteurSocketsClients[j] > maxDescripteurSocket)
                                    maxDescripteurSocket = descripteurSocketsClients[j];
                            }
                        }
                        printf("Client connected via socket %d just logged out\n", descripteurSocketClient);
                        memset(clientTalking, '\0', 2048);
                        sprintf(clientTalking, "Client %d logged out ", descripteurSocketClient);
                        strcpy(response, clientTalking);
                        for (int k = 0; k < compteurClient; k++) {
                            if (descripteurSocketsClients[k] != descripteurSocketClient)
                                send(descripteurSocketsClients[k], response, strlen(response), 0);
                        }
                        memset(request, '\0', 2048);
                        memset(response, '\0', 2048);
                        memset(clientTalking, '\0', 2048);
                        break;
                    } else if (n > 0) {
                        memset(response, '\0', 2048);
                        memset(clientTalking, '\0', 2048);
                        sprintf(clientTalking, "Client %d : ", descripteurSocketClient);
                        strcpy(response, clientTalking);
                        strcat(response, request);
                        for (int k = 0; k < compteurClient; k++)
                            if ((descripteurSocketsClients[k] != descripteurSocketClient) && sizeof(request) > 0)
                                send(descripteurSocketsClients[k], response, strlen(response), 0);
                        memset(request, '\0', 2048);
                        memset(response, '\0', 2048);
                        memset(clientTalking, '\0', 2048);
                        break;
                    }
                }
            }
        }
    }
    close(descripteurSocketServeur);
    return EXIT_SUCCESS;

}
