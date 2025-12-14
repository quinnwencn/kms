//
// Created by Quinn Wen on 2025/12/22.
//

#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include <string>

#include "handlerinterface.h"

namespace Http {

class HandlerFactory {
public:
    using Creator = std::function<std::shared_ptr<Http::HandlerInterface>()>;

    static HandlerFactory& Instance() {
        static HandlerFactory instance;
        return instance;
    }

    void Register(const std::string& name, Creator creator) {
        creators_[name] = std::move(creator);
    }

    std::shared_ptr<Http::HandlerInterface> Create(const std::string& name) {
        auto it = creators_.find(name);
        if (it == creators_.end()) {
            return nullptr;
        }

        return it->second();
    }

private:
    HandlerFactory() = default;

    std::unordered_map<std::string, Creator> creators_;
};

}
