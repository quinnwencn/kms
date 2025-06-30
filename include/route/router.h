#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <boost/beast/http.hpp>

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
    
    void AddRoute(const std::string& path, http::verb method, std::shared_ptr<HandlerInterface> handler) {
        routes_.emplace(RouteKey(path, method), handler);
    }

    http::message_generator Handle(http::request<http::string_body>&& req);

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
        std::string path_;
        http::verb method_;
    };

    struct RouteKeyHash {
        size_t operator()(const RouteKey& key) const {
            size_t seed {0};
            boost::hash_combine(seed, key.Path());
            boost::hash_combine(seed, key.Method());
            return seed;
        }
    };

    std::unordered_map<RouteKey, std::shared_ptr<HandlerInterface>, RouteKeyHash> routes_;
};

}

#endif // ROUTER_H
