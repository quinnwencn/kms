#include "http/route/router.h"
#include "http/handler/handlerregistry.h"
#include "http/handler/handlerinterface.h"

namespace Http {

Router::Router() {
    auto& handlers = Http::HandlerRegistry::Instance().Handlers();
    for (const auto& [path, method, factory] : handlers) {
        AddRoute(path, method, factory());
    }
}

http::message_generator Router::Handle(http::request<http::string_body>&& req) {
    auto it = routes_.find(RouteKey(req.target().data(), req.method()));
    if (it != routes_.end()) {
        return it->second->Handle(std::move(req));
    }

    return HandlerInterface::GenerateMsg(http::status::not_found, 
        R"({"error": "Not Found", "message": "The requested resource is not found.}")", 
        req.keep_alive(), 
        req.version());
}

}