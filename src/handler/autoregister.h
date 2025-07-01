#ifndef AUTOREGISTER_H
#define AUTOREGISTER_H

#include "http/handler/handlerregistry.h"

#define REGISTER_HANDLER(Path, Method, HandlerClass)                       \
namespace {                                                                \
    struct AutoRegister_##HandlerClass {                                   \
        AutoRegister_##HandlerClass() {                                    \
            Http::HandlerRegistry::Instance().Register(                    \
                Path, Method,                                              \
                []() -> std::shared_ptr<Http::HandlerInterface> {         \
                    return std::make_shared<HandlerClass>();              \
                });                                                        \
        }                                                                  \
    };                                                                     \
    static AutoRegister_##HandlerClass _auto_register_##HandlerClass;     \
}

#endif
