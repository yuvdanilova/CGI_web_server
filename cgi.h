struct Request
{
        std::string method = "";
        std::string url = "";
        std::string query = "";
        std::string length = "";
};

std::string get_MIME(std::string ext);
int form_response(Request req, pid_t fdsocket_client);
int form_static_response(std::string url, pid_t fdsocket_client);
int form_not_found_response(std::string url, pid_t fdsocket_client);
void parse (std::string buf, char* path, Request &req);
