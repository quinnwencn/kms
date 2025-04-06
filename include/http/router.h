#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <unordered_map>
#include <memory>


namespace Http {

class HandlerInterface;

class Router {
public:
    ~Router() = default;

    static Router& Instance() {
        static Router router;
        return router;
    }
    
    void AddRoute(const std::string& path, std::unique_ptr<HandlerInterface> handler) {
        routes_[path] = std::move(handler);
    }

private:
    Router() = default;

private:
    std::unordered_map<std::string, std::unique_ptr<HandlerInterface>> routes_;
};

}

#endif // ROUTER_H
