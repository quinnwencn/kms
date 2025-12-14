//
// Created by Quinn Wen on 2025/12/1.
//

#pragma once

#include <filesystem>
#include "config.h"

namespace kms {

class Kms {
public:
    Kms(const Config& config) : config_(config) {}
    ~Kms() = default;

    void Initialize();
    void Start();

private:
    Config config_;
};

}
