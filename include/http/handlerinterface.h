#ifndef HANDLERINTERFACE_H
#define HANDLERINTERFACE_H

#include <boost/beast/http.hpp>

namespace beast = boost::beast;
namespace http = beast::http;  

namespace Http {

class HandlerInterface {
public:
    HandlerInterface() = default;
    virtual ~HandlerInterface() = default;
    
    // Pure virtual function to handle requests
    virtual http::response<http::string_body> handleRequest() = 0;

};

}

#endif // HANDLERINTERFACE_H
