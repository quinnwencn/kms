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
    virtual http::response<http::string_body> Handle(const http::request<http::string_body>& req) = 0;
    static http::response<http::string_body> GenerateMsg(http::status status, 
                                                         const std::string& body, 
                                                         bool keepAlive, 
                                                         int httpVersion = 11) {
        http::response<http::string_body> res(status, httpVersion); // HTTP/1.1
        res.set(http::field::content_type, "application/json");
        res.keep_alive(keepAlive);
        res.body() = body;
        res.prepare_payload();
        return res;
    }
};

}

#endif // HANDLERINTERFACE_H
