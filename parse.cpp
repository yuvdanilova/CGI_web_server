#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include "cgi.h"

void parse (std::string buf, char* path, Request &req) {
        size_t pos = buf.find(' ');
        req.method = buf.substr(0, pos);
        if (req.method == "GET") {
                size_t q_mark = buf.find('?');
                if (q_mark != -1) {
                        req.url = std::string(path) + buf.substr(pos+1, q_mark-pos-1);
                        req.query = buf.substr(q_mark+1, buf.find(' ', q_mark+1)-q_mark-1);
                } else {
                        req.url = std::string(path) + buf.substr(pos+1, buf.find(' ', pos+1)-pos-1);
                }
        } else if (req.method == "POST"){
            req.url = std::string(path) + buf.substr(pos+1, buf.find(' ', pos+1)-pos-1);
            size_t pos_len = buf.find("Content-Length: ")+16;
            req.length = buf.substr(pos_len, buf.find(' ', pos_len+1)-pos_len-1);
            int len = std::stoi(req.length);
            req.query = buf.substr(buf.find("\r\n\r\n")+4, len);
        } 
}
