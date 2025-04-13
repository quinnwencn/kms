#include "route/router.h"
#include "route/handlerinterface.h"

namespace Http {

http::response<http::string_body> Router::Handle(const http::request<http::string_body>& req) {
    auto it = routes_.find(RouteKey(req.target().data(), req.method()));
    if (it != routes_.end()) {
        return it->second->Handle(req);
    }
    return HandlerInterface::GenerateMsg(http::status::not_found, 
        R"({"error": "Not Found", "message": "The requested resource is not found.}")", 
        req.keep_alive(), 
        req.version());
}

}