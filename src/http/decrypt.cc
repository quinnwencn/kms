//
// Created by Quinn Wen on 2025/12/22.
//
//
// Created by Quinn Wen on 2025/12/22.
//
#include "handler_register.h"

namespace Http {

    class DecryptHandler : public HandlerInterface {
    public:
        DecryptHandler() = default;
        ~DecryptHandler() override = default;

        http::message_generator Handle(http::request<http::string_body>&& req) override {
            // Parse the request body
            // TODO finish the function
            return HandlerInterface::GenerateMsg(http::status::ok,
                R"({"message": "cyphertext decrypted successfully."})",
                req.keep_alive(),
                req.version());
        }
    };

    static Registrar<DecryptHandler> _decrypt_handler("Decrypt");

}
