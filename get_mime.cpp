#include <string>

std::string get_MIME(std::string ext) {
    if (ext == "htm" || ext == "html") {
        return "text/html";
    } else if (ext == "css") {
        return "text/css";
    } else if (ext == "js") {
        return "application/javascript";
    } else if (ext == "bmp") {
        return "image/bmp";
    } else if (ext == "gif") {
        return "image/gif";
    } else if (ext == "jpeg" || ext == "jpg") {
        return "image/jpeg";
    } else if (ext == "png") {
        return "image/png";
    }
    return "text/plain";
}
