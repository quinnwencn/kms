#ifndef AUTOREGISTER_H
#define AUTOREGISTER_H

#include "http/router.h"
#include "http/handlerinterface.h"

#define REGISTER_EXIST_HANDLER(Path, Method, HandlerClass)                                          \
namespace {                                                                                         \
    struct AutoRegister##HandlerClass {                                                             \
        AutoRegister##HandlerClass() {                                                              \
                Router::Instance().AddRouter(Path, Method, std::make_unique<##HandlerClass>());     \
            }                                                                                       \
        };                                                                                          \
        static AutoRegister##HandlerClass _autoRegister##HandlerClass;                              \
    } 


#define HANDLER_CLASS(Path, Method, HandlerClass)                                                   \
    class ##HandlerClass;                                                                           \
    REGISTER_EXIST_HANDLER(Path, ##HandlerClass)                                                    \
    class ##HandlerClass : public HandlerInterface


#endif
