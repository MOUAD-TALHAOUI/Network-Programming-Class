#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

void outputOnFile(char *ligne) {
    FILE *descripteurFichier = NULL;
    char *fileName = "output.txt";
    descripteurFichier = fopen(fileName, "a"); //W écrase le contenu
    if (descripteurFichier != NULL) {
        fputs(ligne, descripteurFichier);
        fclose(descripteurFichier);
    } else {
        printf("Erreur d'accès au fichier\n");
    }
}

int getValAleat() {
    srand(time(NULL));
    int valAleatoire = rand() % 6 + 1;
    printf("Variable est %d", valAleatoire);
    return valAleatoire;
}

int main() {
    int descripteurSocketServeur, descripteurSocketClient;
    char *request = calloc(2048, sizeof(char));
    char *response = calloc(2048, sizeof(char));
    char *nomClient = calloc(2048, sizeof(char));
    char *nbTentatives = calloc(2048, sizeof(char));
    printf("HI");
    struct sockaddr_in adresseServeur, adressClient;
    socklen_t longeurAdresseClient = sizeof(struct sockaddr);
    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_port = htons(8015);
    inet_aton("127.0.0.1", &(adresseServeur.sin_addr));
    memset(&adresseServeur.sin_zero, '0', 8);
    descripteurSocketServeur = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (bind(descripteurSocketServeur, (struct sockaddr *) &adresseServeur, sizeof(struct sockaddr)) == -1) {
        perror("Binding problem");
        return EXIT_FAILURE;
    }
    if (listen(descripteurSocketServeur, 10) == -1) {
        perror("Binding problem");
        return EXIT_FAILURE;
    }

    while (1) {
        descripteurSocketClient = accept(descripteurSocketServeur, (struct sockaddr *) &adressClient,
                                         &longeurAdresseClient);
        memset(request, '\0', 2048);
        strcpy(request, "Bienvenu, entrez un nom\n");
        send(descripteurSocketClient, request, strlen(request), 0);
        memset(nomClient, '\0', 2048);
        if (recv(descripteurSocketClient, nomClient, 2048, 0) > 0) {
            printf("Nom du client : %s", nomClient);
        }
        int nbTentative = 3;
        bool found = false;
        memset(request, '\0', 2048);
        int valAleatoire = getValAleat();
        printf("Variable aléatoire est %d", valAleatoire);
        strcpy(request, "Devinez un nombre aléatoire caché entre 1 et 6\n");
        while (!found && nbTentative > 0) {
            send(descripteurSocketClient, request, strlen(request), 0);
            memset(request, '\0', 2048);
            if (recv(descripteurSocketClient, response, 2048, 0) > 0) {
                if (atoi(response) == valAleatoire) {
                    memset(request, '\0', 2048);
                    strcpy(request, "Bravo vous avez trouvé le nombre mystère !!!\n");
                    send(descripteurSocketClient, request, strlen(request), 0);
                    memset(request, '\0', 2048);
                    sprintf(nbTentatives, "%d \n", 3 - nbTentative);
                    strcat(nomClient, nbTentatives);
                    outputOnFile(nomClient);
                    found = true;
                    break;
                } else {
                    nbTentative--;
                    nbTentative == 1 ? sprintf(request,
                                               "Désolé votre réponse est incorrecte, il vous reste %d tantative\n",
                                               nbTentative) : sprintf(request,
                                                                      "Désolé votre réponse est incorrecte, il vous reste %d tantatives\n",
                                                                      nbTentative);
                }
            }
        }
        if (!found) {
            strcpy(request, "Vous n'avez pas trouvé le nombre mystère.\n");
            send(descripteurSocketClient, request, strlen(request), 0);
        }
        memset(request, '\0', 2048);
        close(descripteurSocketClient);
    }
    close(descripteurSocketServeur);
    return EXIT_SUCCESS;

}