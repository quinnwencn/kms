#ifndef AUTOREGISTER_H
#define AUTOREGISTER_H

#include "route/router.h"
#include "route/handlerinterface.h"

#define REGISTER_EXIST_HANDLER(Path, Method, HandlerClass)                                          \
    struct AutoRegister##HandlerClass {                                                             \
        AutoRegister##HandlerClass() {                                                              \
                Http::Router::Instance().AddRoute(Path, Method, std::make_shared<HandlerClass>());  \
            }                                                                                       \
    };                                                                                              \
    static AutoRegister##HandlerClass _autoRegister##HandlerClass;                              

#define HANDLER_CLASS(Path, Method, HandlerClass)                                                   \
class HandlerClass : public Http::HandlerInterface {                                          \
    REGISTER_EXIST_HANDLER(Path, Method,HandlerClass)                                               


#endif
