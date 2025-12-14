//
// Created by Quinn Wen on 2025/12/1.
//

#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>
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

class RouteConfig : public IConfig {
public:
    class Route {
    public:
        Route() = default;
        Route(std::string_view api, std::string_view method, std::string_view handler) :
            api_(api), method_(method), handler_(handler) {}
        ~Route() = default;

        const std::string& Api() const { return api_; }
        const std::string& Method() const { return method_; }
        const std::string& Handler() const { return handler_; }

    private:
        std::string api_;
        std::string method_;
        std::string handler_;
    };

    RouteConfig() = default;
    ~RouteConfig() override = default;

    void LoadFromTable(const toml::table &tbl) override;
    void WriteToLog(std::string_view name) override;
    const std::vector<Route>& Routes() const { return routes_; }

private:
    std::vector<Route> routes_;
};

class Config {
public:
    Config() = default;
    explicit Config(const std::filesystem::path& filename);

    void LoadFromToml(const std::filesystem::path& configPath);

    const TLSConfig& TLSConf() const {
        return tlsConfig_;
    }

    const ServerConfig& ServerConf() const {
        return serverConfig_;
    }

    const LogConfig& LogConf() const {
        return logConfig_;
    }

    const RouteConfig& RouteConf() const {
        return routeConfig_;
    }

private:
    void RecordConfig();

    TLSConfig tlsConfig_;
    ServerConfig serverConfig_;
    LogConfig logConfig_;
    RouteConfig routeConfig_;
};

}
