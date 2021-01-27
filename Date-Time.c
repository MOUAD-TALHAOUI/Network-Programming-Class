#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int main() {
    int descripteurSocketServeur, descripteurSocketClient;
    char *response = calloc(2014, sizeof(char));
    char *request = calloc(2014, sizeof(char));
    struct sockaddr_in adresseServeur, adressClient;
    socklen_t longeurAdresseClient = sizeof(struct sockaddr);
    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_port = htons(8082);
    inet_aton("127.0.0.1", &(adresseServeur.sin_addr));
    memset(&adresseServeur.sin_zero, '0', 8);
    descripteurSocketServeur = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (bind(descripteurSocketServeur, (struct sockaddr *) &adresseServeur, sizeof(struct sockaddr)) == -1) {
        perror("Bind problem");
        return EXIT_FAILURE;
    }
    if (listen(descripteurSocketServeur, 10) == -1) {
      perror("Bind problem");
        return EXIT_FAILURE;
    }
    time_t laDate;
    time(&laDate);
    struct tm ti = *localtime(&laDate);
    while (1) {
        descripteurSocketClient = accept(descripteurSocketServeur, (struct sockaddr *) &adressClient,
                                         &longeurAdresseClient);
        while (1) {
            memset(request, '\0', 1024);
            memset(response, '\0', 1024);
            if (recv(descripteurSocketClient, request, 1024, 0) > 0) {
                if (strncmp(request, "TIME", 5) == 0 && strlen(request) <= 7)
                    sprintf(response, "%d : %d\n", ti.tm_hour, ti.tm_min);
                else if (strncmp(request, "DATE", 4) == 0 && strlen(request) <= 6)
                    sprintf(response, "%d/%d/%d\n", ti.tm_mday, ti.tm_mon + 1, ti.tm_year + 1900);
                else if (strncmp(request, "TIMEDATE", 9) == 0 && strlen(request) <= 11)
                    sprintf(response, "%s\n", asctime(&ti));
                else if (strncmp(request, "EXIT", 4) == 0 && strlen(request) <= 6)
                    break;
                else
                    sprintf(response, "Not a valid option\n");
                send(descripteurSocketClient, response, strlen(response), 0);
            }
        }
        close(descripteurSocketClient);
    }
    close(descripteurSocketServeur);
    return EXIT_SUCCESS;
}
