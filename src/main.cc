#include <CLI/CLI.hpp>
#include <filesystem>

#include "kms/kms.h"
#include "kms/logger.h"

int main(int argc, char **argv) {
    CLI::App app {"KMS"};
    std::filesystem::path configFilePath;

    app.add_option("-c, --config", configFilePath, "Path of config file")->required();
    CLI11_PARSE(app, argc, argv);

    kms::Config config(configFilePath);
    Logger::Init(config.LogConf());

    LOG_TRACE("Starting KMS...");
    kms::Kms kms(config);
    kms.Initialize();
    kms.Start();

    return 0;
}
