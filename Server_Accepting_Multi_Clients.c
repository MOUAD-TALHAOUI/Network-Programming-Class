#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
    int descripteurSocketServeur, descripteurSocketClient;
    char *request = calloc(2014, sizeof(char));
    char *response = calloc(2014, sizeof(char));
    struct sockaddr_in adresseServeur, adressClient;
    socklen_t longeurAdresseClient = sizeof(struct sockaddr);
    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_port = htons(8080);
    inet_aton("127.0.0.1", &(adresseServeur.sin_addr));
    memset(&adresseServeur.sin_zero, '0', 8);
    descripteurSocketServeur = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (bind(descripteurSocketServeur, (struct sockaddr *) &adresseServeur, sizeof(struct sockaddr)) == -1) {
        perror("Binding Problem");
        return EXIT_FAILURE;
    }
    if (listen(descripteurSocketServeur, 10) == -1) {
      perror("Binding Problem");
        return EXIT_FAILURE;
    }
    while (1) {
        descripteurSocketClient = accept(descripteurSocketServeur, (struct sockaddr *) &adressClient,
                                         &longeurAdresseClient);
        printf("New connection\n");
        int child = fork();
        if (child<0){
            perror("Child impossible");
            return EXIT_FAILURE;
        }else {
            if (child != 0)
                close(descripteurSocketClient);
            else {
                close(descripteurSocketServeur);
                while (1) {
                    memset(response, '\0', 1024);
                    strcpy(response, "Your message: ");
                    send(descripteurSocketClient, response, strlen(response), 0);
                    memset(request, '\0', 1024);
                    memset(request, '\0', 1024);
                    if (recv(descripteurSocketClient, request, 1024, 0) > 0) {
                        if (strncmp(request, "EXIT", 4) == 0 && strlen(request) < 7) {
                            strcpy(response, "Closing program\n");
                            send(descripteurSocketClient, response, strlen(response), 0);
                            break;
                        } else
                            send(descripteurSocketClient, request, strlen(request), 0);
                    }
                }
                close(descripteurSocketClient);
                return EXIT_SUCCESS;
            }
            close(descripteurSocketClient);
        }
    }
    close(descripteurSocketServeur);
    return EXIT_SUCCESS;

}
