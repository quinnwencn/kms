//
// Created by Quinn Wen on 2025/12/1.
//

#include "kms/config.h"
#include "kms/logger.h"

namespace kms {

template <typename T>
void UpdateFromToml(T& t, std::string_view name, const toml::table& tbl) {
    t.LoadFromTable(*tbl[name].as_table());
}

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
    Logger::Trace("[{}]", name);
    Logger::Trace("ca_cert_type = {}", static_cast<int>(caCertType_));
    Logger::Trace("ca_certificate = ", caCertificate_);
    Logger::Trace("cert_type = {}", static_cast<int>(certType_));
    Logger::Trace("certificate = {}", certificate_);
    Logger::Trace("key_type = {}", static_cast<int>(keyType_));
    Logger::Trace("private_key = {}", privateKey_);
    Logger::Trace("verify_peer = {}", verifyPeer_);
}

void ServerConfig::LoadFromTable(const toml::table &tbl) {
    address_ = tbl.at("address").value_or("");
    port_ = tbl.at("port").value_or(8443);
    threadNumber_ = tbl.at("thread_number").value_or(0);
}

void ServerConfig::WriteToLog(std::string_view name) {
    Logger::Trace("[{}]", name);
    Logger::Trace("address_ = {}", address_);
    Logger::Trace("port_ = {}", port_);
    Logger::Trace("threadNumber_ = {}", threadNumber_);
}

void LogConfig::LoadFromTable(const toml::table &tbl) {
    logDirs_ = tbl.at("log_dirs").value_or("");
    logLevel_ = tbl.at("log_level").value_or(2);
}

void LogConfig::WriteToLog(std::string_view name) {
    Logger::Trace("[{}]", name);
    Logger::Trace("logDirs_ = {}", logDirs_.string());
    Logger::Trace("logLevel_ = {}", logLevel_);
}

void Config::LoadFromToml(const std::filesystem::path &configPath) {
    if (!std::filesystem::exists(configPath)) {
        throw std::runtime_error("Config file does not exist");
    }

    auto tbl = toml::parse_file(configPath.string());
    UpdateFromToml(tlsConfig_, "tls", tbl);
    UpdateFromToml(serverConfig_, "server", tbl);
    UpdateFromToml(logConfig_, "log", tbl);
}

void Config::RecordConfig() {
    Logger::Init(logConfig_);
    WriteToLog(tlsConfig_, "tls");
    WriteToLog(serverConfig_, "server");
    WriteToLog(logConfig_, "log");
}

}
