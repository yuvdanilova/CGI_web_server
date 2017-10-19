#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include "cgi.h";

int form_static_response(std::string url, pid_t fdsocket_client) {
    std::string ext = url.substr(url.rfind('.')+1);
    std::stringstream response;
    std::stringstream response_body;
    int fd = open(url.c_str(), O_RDONLY);
    int numchar = 0;
    int len = 0;
    char buf[1024];
    while ((numchar = read(fd, buf, sizeof(buf))) > 0) {
        response_body << std::string(buf, numchar);
    }
    response << "HTTP/1.1 200 OK\r\n"
        << "Content-Type: "
        << get_MIME(ext)
        << "\r\n"
        << "Content-Length: " << response_body.str().length()
        << "\r\n\r\n"
        << response_body.str();
    int result = send(fdsocket_client, response.str().c_str(), response.str().length(), 0);
    return 0;
}
