#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <bootst/beast/http.hpp>

namespace beast = boost::beast;
namespace http = beast::http;  

namespace Http {

class HandlerInterface;

class Router {
public:
    ~Router() = default;

    static Router& Instance() {
        static Router router;
        return router;
    }
    
    void AddRoute(const std::string& path, http::verb method, std::unique_ptr<HandlerInterface> handler) {
        routes_.emplace(RouteKey(path, method), std::move(handler));
    }

    http::response<http::string_body> Handle(const http::request<http::string_body>& req) {
        auto it = routes_.find(RouteKey(req.target().to_string(), req.method()));
        if (it != routes_.end()) {
            return it->second->Handle(req);
        }
        return HandlerInterface::GenerateMsg(http::status::not_found, 
            R"({"error": "Not Found", "message": "The requested resource is not found.}")", 
            req.keep_alive(), 
            req.version());
    }

private:
    Router() = default;

private:
    class RouteKey {
    public:
        RouteKey(const std::string& path, http::verb method)
            : path_(path), method_(method) {}

        bool operator==(const RouteKey& other) const {
            return path_ == other.path_ && method_ == other.method_;
        }

        const std::string& Path() const {
            return path_;
        }

        const http::verb Method() const {
            return method_;
        }

    private:
        string path_;
        http::verb method_;
    };

    struct RouteKeyHash {
        size_t operator()(const RouteKey& key) const {
            size_ seed {0};
            boost::hash_combine(seed, key.Path());
            boost::hash_combine(seed, key.Method());
            return seed;
        }
    };

    std::unordered_map<RouteKey, std::unique_ptr<HandlerInterface>, RouteKeyHash> routes_;
};

}

#endif // ROUTER_H
