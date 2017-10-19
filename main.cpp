#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/wait.h>
#include <string.h>
#include "cgi.h"

int main(int argc, char *argv[]) {
    char addr_buf[128];
    char *myaddr = getcwd(addr_buf, sizeof(addr_buf));
    int no_port = 0;

    if (argc == 2 || argc == 3) {
        const char *path = "path=";
        const char *port = "port=";
        int counter = 0;
        int no_port1 = 0;
        char addr_buf1[128];
        for (int i = 1; i != argc; ++i) {
            if (strlen(argv[i]) > 5) {
                if (!strncmp(argv[i], path, 5)) {
                    if (argc == 2) {
                        strcpy(addr_buf, argv[i]+5);
                    } else {
                        strcpy(addr_buf1, argv[i]+5);
                        --counter;
                    }
                } else if (!strncmp(argv[i], port, 5)) {
                    char port_buf[128];
                    strcpy(port_buf, argv[i]+5);
                    if (argc == 2) {
                        no_port = atoi(port_buf);
                    } else {
                        no_port1 = atoi(port_buf);
                        ++counter;
                    }
                } else {
                    perror("Invalid argument");
                    exit(1);
                }
            } else {
                perror("Invalid argument");
                exit(1);
            }
        }
        if (argc == 3) {
            if (counter) {
                perror("Two arguments of the same type");
                exit(1);
            }
            strcpy(addr_buf, addr_buf1);
            no_port = no_port1;
        }
    }

    char buff[1024];
    struct sockaddr_in addr;
    socklen_t addrlen;

    int fdsocket = socket(AF_INET, SOCK_STREAM, 0);
    if (fdsocket == -1) {
        perror("socket");
        exit(1);
    }

    int sopt = 1;
    if (setsockopt(fdsocket, SOL_SOCKET, SO_REUSEADDR, &sopt, sizeof(sopt)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(no_port);
    addrlen = sizeof(struct sockaddr_in);

    if (bind(fdsocket, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("bind");
        exit(1);
    }

    getsockname(fdsocket, (struct sockaddr *) &addr, &addrlen);
    std::cout << "Socket created\nPORT=" << ntohs(addr.sin_port) << " DIRECTORY=" << addr_buf << std::endl;

    if (listen(fdsocket, 5) == -1) {
        perror("listen");
        exit(1);
    }
    while (1) {
        while (waitpid(-1, NULL, WNOHANG) > 0);
        int fdsocket_client = accept(fdsocket, (struct sockaddr *) &addr, &addrlen);
        if (fdsocket_client == -1) {
            perror("new socket");
            exit(1);
        }
        if (!fork()) {
            //son
            int message = recv(fdsocket_client, buff, 1024, 0);
            if (message < 0) {
                perror("recv");
                exit(1);
            } else {
                Request req;
                parse(std::string(buff, message), myaddr, req);
                std::cout << "METHOD: " << req.method << " URL: " << req.url << " QUERY: " << req.query << std::endl;
                form_response(req, fdsocket_client);
            }
            close(fdsocket_client);
            exit(0);
        }
        close(fdsocket_client);
    }
    return 0;
}
