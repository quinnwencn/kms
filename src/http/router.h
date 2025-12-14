#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <boost/beast/http.hpp>

#include <iostream>
namespace beast = boost::beast;
namespace http = beast::http;  

namespace Http {

class HandlerInterface;

class Router {
public:
    static Router& Instance() {
        static Router router;
        return router;
    }

    ~Router() = default;

    http::message_generator Handle(http::request<http::string_body>&& req);
    void AddRoute(const std::string& api, http::verb method, std::shared_ptr<HandlerInterface> handler) {
        routes_.emplace(api, std::make_pair(method, handler));
    }

private:
    Router() = default;
    std::unordered_map<std::string_view, std::pair<http::verb, std::shared_ptr<HandlerInterface>>> routes_;
};

}

#endif // ROUTER_H
