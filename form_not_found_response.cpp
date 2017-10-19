#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <sstream>
#include "cgi.h"

int form_not_found_response(std::string url, pid_t fdsocket_client) {
    std::stringstream response;
    std::stringstream response_body;
    response_body <<  "<title>404 Not Found</title>" 
        << "<h1>Not Found</h1>"
        << "<p>The requested URL "
        << url
        << " was not found on this server.</p>";
    response << "HTTP/1.1 400 Bad Request\r\n"
        << "Content-Type: text/html; charset=utf-8\r\n"
        << "Content-Length: " << response_body.str().length()
        << "\r\n\r\n"
        << response_body.str();
    int result = send(fdsocket_client, response.str().c_str(), response.str().length(), 0);
    return 0;
}
