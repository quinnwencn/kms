//
// Created by quinn on 7/1/2025.
// Copyright (c) 2025 All rights reserved.
//

#ifndef HANDLERREGISTER_H
#define HANDLERREGISTER_H

#include <functional>
#include <vector>
#include <string>
#include <boost/beast/http.hpp>

namespace Http {

class HandlerInterface;

class HandlerRegistry {
public:
    using Factory = std::function<std::shared_ptr<HandlerInterface>()>;

    static HandlerRegistry& Instance() {
        static HandlerRegistry instance;
        return instance;
    }

    void Register(std::string path, boost::beast::http::verb method, Factory factory) {
        handlers_.emplace_back(path, method, std::move(factory));
    }

    const std::vector<std::tuple<std::string, boost::beast::http::verb, Factory>>& Handlers() const {
        return handlers_;
    }

private:
    HandlerRegistry() = default;
    std::vector<std::tuple<std::string, boost::beast::http::verb, Factory>> handlers_;
};

} // namespace Http

#endif //HANDLERREGISTER_H
