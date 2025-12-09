//
// Created by Quinn Wen on 2025/12/1.
//

#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <toml++/toml.hpp>

#include "types.h"

namespace kms {

class IConfig {
public:
    virtual ~IConfig() = default;

    virtual void LoadFromTable(const toml::table& tbl) = 0;
    virtual void WriteToLog(std::string_view name) = 0;
};

class TLSConfig : public IConfig {
public:
    TLSConfig() = default;
    ~TLSConfig() override = default;

    CryptoType CaCertType() const { return caCertType_; }
    CryptoType CertType() const { return certType_; }
    CryptoType KeyType() const { return keyType_; }
    const std::string& PrivateKey() const { return privateKey_; }
    const std::string& CaCertificate() const { return caCertificate_; }
    const std::string& Certificate() const { return certificate_; }
    bool VerifyPeer() const { return verifyPeer_; }
    void LoadFromTable(const toml::table& tbl) override;
    void WriteToLog(std::string_view name) override;

private:
    CryptoType caCertType_;
    std::string caCertificate_;
    CryptoType certType_;
    std::string certificate_;
    CryptoType keyType_;
    std::string privateKey_;
    bool verifyPeer_;
};

class ServerConfig : public IConfig {
public:
    ServerConfig() = default;
    ~ServerConfig() override = default;

    const std::string& Address() const { return address_; }
    uint16_t Port() const { return port_; }
    uint8_t ThreadNumber() const { return threadNumber_; }
    void LoadFromTable(const toml::table &tbl) override;
    void WriteToLog(std::string_view name) override;

private:
    std::string address_;
    uint16_t port_;
    uint8_t threadNumber_;
};

class LogConfig : public IConfig {
public:
    LogConfig() = default;
    ~LogConfig() override = default;

    const std::filesystem::path& LogDir() const { return logDirs_; }
    int LogLevel() const { return logLevel_; }
    void LoadFromTable(const toml::table &tbl) override;
    void WriteToLog(std::string_view name) override;

private:
    std::filesystem::path logDirs_;
    int logLevel_;
};

class Config {
public:
    Config() = default;
    explicit Config(const std::filesystem::path& filename);

    void LoadFromToml(const std::filesystem::path& configPath);
    void RecordConfig();

    const TLSConfig& TLSConf() const {
        return tlsConfig_;
    }

    const ServerConfig& ServerConf() const {
        return serverConfig_;
    }

    const LogConfig& LogConf() const {
        return logConfig_;
    }

private:
    TLSConfig tlsConfig_;
    ServerConfig serverConfig_;
    LogConfig logConfig_;
};

}
