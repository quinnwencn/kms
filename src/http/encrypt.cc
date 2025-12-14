//
// Created by Quinn Wen on 2025/12/22.
//
#include "handler_register.h"

namespace Http {

    class EncryptHandler : public HandlerInterface {
    public:
        EncryptHandler() = default;
        ~EncryptHandler() override = default;

        http::message_generator Handle(http::request<http::string_body>&& req) override {
            // Parse the request body
            // TODO finish the function
            return HandlerInterface::GenerateMsg(http::status::ok,
                R"({"message": "plaintext encrypted successfully."})",
                req.keep_alive(),
                req.version());
        }
    };

    static Registrar<EncryptHandler> _encrypt_handler("Encrypt");

}
