//
// Created by Quinn Wen on 2025/12/22.
//

#pragma once

#include "handlerfactory.h"

namespace Http {

template <typename T>
class Registrar {
public:
    explicit Registrar(const std::string& name) {
        HandlerFactory::Instance().Register(
            name,
            []() { return std::make_shared<T>(); }
        );
    }
};

}
