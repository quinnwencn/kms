#include "autoregister.h"
#include "http/handler/handlerinterface.h"

namespace Http {

class GenerateKeyHandler : public HandlerInterface {
public:
    GenerateKeyHandler() = default;
    ~GenerateKeyHandler() = default;

    http::message_generator Handle(http::request<http::string_body>&& req) override {
        // Parse the request body
        // TODO finish the function
        return HandlerInterface::GenerateMsg(http::status::ok, 
            R"({"message": "Key generated successfully."})", 
            req.keep_alive(), 
            req.version());
    }
};

REGISTER_HANDLER("/key/generate", http::verb::post, GenerateKeyHandler)

}
