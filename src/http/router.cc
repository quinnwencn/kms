#include "router.h"
#include "handlerinterface.h"
#include "kms/logger.h"

namespace Http {

http::message_generator Router::Handle(http::request<http::string_body>&& req) {
    std::string api{
        req.target().begin(),
        req.target().end()
    };
    auto it = routes_.find(api);
    if (it != routes_.end()) {
        // for those api with wrong method, reply with forbidden msg
        if (it->second.first != req.method()) {
            return HandlerInterface::GenerateMsg(http::status::forbidden,
            R"({"error": "Forbidden", "message": "The requested resource is forbidden.}")",
            req.keep_alive(),
            req.version());
        }

        return it->second.second->Handle(std::move(req));
    }

    LOG_DEBUG("Can not find handler for api: {}", api);
    return HandlerInterface::GenerateMsg(http::status::not_found, 
        R"({"error": "Not Found", "Debug message": "The requested resource is not found.}")",
        req.keep_alive(), 
        req.version());
}

}
