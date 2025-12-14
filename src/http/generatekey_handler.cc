#include "handler_register.h"

namespace Http {

class GenerateKeyHandler : public HandlerInterface {
public:
    GenerateKeyHandler() = default;
    ~GenerateKeyHandler() override = default;

    http::message_generator Handle(http::request<http::string_body>&& req) override {
        // Parse the request body
        // TODO finish the function
        return HandlerInterface::GenerateMsg(http::status::ok, 
            R"({"message": "Key generated successfully."})", 
            req.keep_alive(), 
            req.version());
    }
};

static Registrar<GenerateKeyHandler> _generate_key_handler("KeyGenerate");

}
