#include "autoregister.h"

HANDLER_CLASS("/key/generate", http::verb::post, GenerateKeyHandler)
public:
    GenerateKeyHandler() = default;
    ~GenerateKeyHandler() = default;

    http::response<http::string_body> Handle(const http::request<http::string_body>& req) override {
        // Parse the request body
        // TODO finish the function
        return HandlerInterface::GenerateMsg(http::status::ok, 
            R"({"message": "Key generated successfully."})", 
            req.keep_alive(), 
            req.version());
    }
};
