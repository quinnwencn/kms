//
// Created by Quinn Wen on 2025/12/1.
//

#include "kms/config.h"
#include "kms/logger.h"

namespace kms {

template <typename T>
void WriteToLog(T& t, std::string_view name) {
    t.WriteToLog(name);
}

void TLSConfig::LoadFromTable(const toml::table& tbl) {
    std::string value = tbl.at("ca_type").value_or("file");
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    caCertType_ = value == "pkcs11" ? CryptoType::PKCS11 : CryptoType::FILE;
    caCertificate_ = tbl.at("ca_certificate").value_or("");

    value = tbl.at("cert_type").value_or("file");
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    certType_ = value == "pkcs11" ? CryptoType::PKCS11 : CryptoType::FILE;
    certificate_ = tbl.at("certificate").value_or("");

    value = tbl.at("key_type").value_or("file");
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    keyType_ = value == "pkcs11" ? CryptoType::PKCS11 : CryptoType::FILE;
    privateKey_ = tbl.at("private_key").value_or("");

    verifyPeer_ = tbl.at("verify_peer").value_or(false);
}

void TLSConfig::WriteToLog(std::string_view name) {
    LOG_TRACE("[{}]", name);
    LOG_TRACE("ca_cert_type = {}", static_cast<int>(caCertType_));
    LOG_TRACE("ca_certificate = ", caCertificate_);
    LOG_TRACE("cert_type = {}", static_cast<int>(certType_));
    LOG_TRACE("certificate = {}", certificate_);
    LOG_TRACE("key_type = {}", static_cast<int>(keyType_));
    LOG_TRACE("private_key = {}", privateKey_);
    LOG_TRACE("verify_peer = {}", verifyPeer_);
}

void ServerConfig::LoadFromTable(const toml::table &tbl) {
    address_ = tbl.at("address").value_or("");
    port_ = tbl.at("port").value_or(8443);
    threadNumber_ = tbl.at("thread_number").value_or(0);
}

void ServerConfig::WriteToLog(std::string_view name) {
    LOG_TRACE("[{}]", name);
    LOG_TRACE("address = {}", address_);
    LOG_TRACE("port = {}", port_);
    LOG_TRACE("thread_number = {}", threadNumber_);
}

void LogConfig::LoadFromTable(const toml::table &tbl) {
    logDirs_ = tbl.at("log_dir").value_or("");
    logLevel_ = tbl.at("level").value_or(2);
}

void LogConfig::WriteToLog(std::string_view name) {
    LOG_TRACE("[{}]", name);
    LOG_TRACE("log_dir = {}", logDirs_.string());
    LOG_TRACE("level = {}", logLevel_);
}

void RouteConfig::LoadFromTable(const toml::table &tbl) {
    auto* items = tbl.at("routes").as_array();
    if (items == nullptr) {
        LOG_DEBUG("No routes in config file.");
        return;
    }

    for (const auto& item : *items) {
        const auto& route = *item.as_table();
        routes_.emplace_back(Route{route["api"].value_or(""),
                                route["method"].value_or(""),
                                route["handler"].value_or("")});
    }
}

void RouteConfig::WriteToLog(std::string_view name) {
    for (const auto& route : routes_) {
        LOG_TRACE("[[{}]]", name);
        LOG_TRACE("api = {}", route.Api());
        LOG_TRACE("method = {}", route.Method());
        LOG_TRACE("handler = {}", route.Handler());
    }
}

Config::Config(const std::filesystem::path& filename) {
    LoadFromToml(filename);
}

void Config::LoadFromToml(const std::filesystem::path &configPath) {
    if (!std::filesystem::exists(configPath)) {
        throw std::runtime_error("Config file does not exist");
    }

    auto tbl = toml::parse_file(configPath.string());
    tlsConfig_.LoadFromTable(*tbl["tls"].as_table());
    serverConfig_.LoadFromTable(*tbl["server"].as_table());
    logConfig_.LoadFromTable(*tbl["log"].as_table());
    routeConfig_.LoadFromTable(tbl);

    RecordConfig();
}

void Config::RecordConfig() {
    WriteToLog(tlsConfig_, "tls");
    WriteToLog(serverConfig_, "server");
    WriteToLog(logConfig_, "log");
    WriteToLog(routeConfig_, "route");
}

}
