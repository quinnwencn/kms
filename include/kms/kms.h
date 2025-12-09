//
// Created by Quinn Wen on 2025/12/1.
//

#pragma once

#include <filesystem>

namespace kms {

class Kms {
public:
    Kms(const std::filesystem::path& configFile) : configFile(configFile) {}
    ~Kms() = default;
    void Start();

private:
    Config config_;
};

}
