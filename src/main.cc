#include <CLI/CLI.hpp>
#include <filesystem>

#include "kms/kms.h"

int main(int argc, char **argv) {
    CLI::App app {"KMS"};
    std::filesystem::path configFilePath;

    app.add_option("-c, --config", configFilePath, "Path of config file")->required();
    CLI11_PARSE(app, argc, argv);

    Config config(configFilePath);
    Kms kms(config);
    kms.Start();

    return 0;
}