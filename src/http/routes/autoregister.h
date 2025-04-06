#ifndef AUTOREGISTER_H
#define AUTOREGISTER_H

#include "http/router.h"
#include "http/handlerinterface.h"

#define REGISTER_HANDLER_EXIST(Path, HandlerClass)                                          \
namespace {                                                                                 \
    struct AutoRegister##HandlerClass {                                                     \
        AutoRegister##HandlerClass() {                                                      \
                Router::Instance().AddRouter(Path, std::make_unique<##HandlerClass>());     \
            }                                                                               \
        };                                                                                  \
        static AutoRegister##HandlerClass _autoRegister##HandlerClass;                      \
    } 


#define REGISTER_HANDLER(Path, HandlerClass)                                                \
    class ##HandlerClass;                                                                   \
    REGISTER_HANDLER_EXIST(Path, ##HandlerClass)                                            \
        class ##HandlerClass : public HandlerInterface


#endif
