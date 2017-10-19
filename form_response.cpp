#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include "cgi.h"

int form_response(Request req, pid_t fdsocket_client) {
    //start making response
    if (!access(req.url.c_str(), F_OK)) {
        if (req.url[req.url.length()-1] == '/') {
            //folder case
            std::cout << "folder" << std::endl;
            if (!access((req.url + "index.html").c_str(), F_OK)) {
                form_static_response(req.url + "index.html", fdsocket_client);
            } else if (!access((req.url + "index.htm").c_str(), F_OK)) {
                form_static_response(req.url + "index.htm", fdsocket_client);
            } else {
                DIR *dp;
                struct dirent *entry;
                if ((dp = opendir(req.url.c_str())) == NULL) {
                    perror("cannot open directory");
                    form_not_found_response(req.url, fdsocket_client);
                    return 0;
                }
                int flag = 0;
                while ((entry = readdir(dp)) != NULL && !flag) {
                    if (std::string(entry->d_name).find("index.") != -1) {
                        form_static_response(req.url + std::string(entry->d_name), fdsocket_client);
                        flag = 1;
                    }
                }
                if (!flag) {
                    form_not_found_response(req.url + "index.html", fdsocket_client);
                }
                closedir(dp);
                return 0;
            }
        } else if (!access(req.url.c_str(), X_OK)) {
            //executable file case
            std::cout << "executable" << std::endl;
            if (req.method == "POST") {
                //cgi script gets data from STDIN, number of bytes given in environment variable CONTENT_LENGTH
                int fd[2];
                pipe(fd);
                write(fd[1], (req.query).c_str(), (req.query).length());
                pid_t pid = fork();
                if (!pid) {
                    //child
                    dup2(fd[0], 0);
                    close(fd[0]); close(fd[1]);
                    dup2(fdsocket_client, 1);
                    close(fdsocket_client);
                    char arg0[req.url.length() + 1]; strcpy(arg0, req.url.c_str());
                    char arg1[req.method.length() + sizeof("REQUEST_METHOD=") + 1]; strcpy(arg1, ("REQUEST_METHOD="+req.method).c_str());
                    char arg2[req.url.length() + sizeof("REQUEST_URI=") + 1]; strcpy(arg2, ("REQUEST_URI="+req.url).c_str());
                    char arg3[req.length.length() + sizeof("CONTENT_LENGTH=") + 1]; strcpy(arg3, ("CONTENT_LENGTH="+req.length).c_str());
                    char* args[] = {arg0, 0};
                    char* env[] = {arg1, arg2, arg3, 0};
                    execve(arg0, args, env);
                    exit(1);
                } else {
                    //parent
                    close(fd[0]); close(fd[1]);
                    wait(NULL);
                    return 0;
                }
            } else if (req.method == "GET") {
                //cgi script gets data from environmental variable QUERY_STRING
                pid_t pid = fork();
                if (!pid) {
                    //child
                    dup2(fdsocket_client, 1);
                    char arg0[req.url.length() + 1]; strcpy(arg0, req.url.c_str());
                    char arg1[req.method.length() + sizeof("REQUEST_METHOD=") + 1]; strcpy(arg1, ("REQUEST_METHOD="+req.method).c_str());
                    char arg2[req.url.length() + sizeof("REQUEST_URI=") + 1]; strcpy(arg2, ("REQUEST_URI="+req.url).c_str());
                    char arg3[req.query.length() + sizeof("QUERY_STRING=") + 1]; strcpy(arg3, ("QUERY_STRING="+req.query).c_str());
                    char* args[] = {arg0, 0};
                    char* env[] = {arg1, arg2, arg3, 0};
                    execve(arg0, args, env);
                } else {
                    //parent
                    wait(NULL);
                    close(fdsocket_client);
                    return 0;
                }
            }
        } else {
            //static file case
            std::cout << "static" << std::endl;
            form_static_response(req.url, fdsocket_client);
            return 0;
        }
    } else {
        //file does not exist
        form_not_found_response(req.url, fdsocket_client);
        return 0;
    }
    return 0;
}

