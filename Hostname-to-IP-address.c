#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    struct hostent *hote = NULL;
    const char *hostname = "www.youtube.com";
    struct in_addr ip;

    hote = gethostbyname(hostname);
    if (hote == NULL) {
        fprintf(stderr, "Unknown hostname : %s\n", hostname);
        exit(EXIT_FAILURE);
    }
    memcpy (&(ip.s_addr), hote->h_addr_list[0], sizeof(ip.s_addr));
    printf("%s ----------- %s", hostname, inet_ntoa(ip));
    return 0;
}