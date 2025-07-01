#ifndef HTTPSESSION_H
#define HTTPSESSION_H

#include <iostream>
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>

#include "route/router.h"

namespace beast = boost::beast;
namespace http = beast::http;  
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

namespace Http {

static void Fail(beast::error_code ec, char const* what) {
    if (ec == net::ssl::error::stream_truncated) {
        return;
    }

    std::cerr << what << ": " << ec.message() << "\n";
}

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    HttpSession(tcp::socket&& socket, ssl::context& ctx) : stream_(std::move(socket), ctx) {}
    ~HttpSession() = default;

    void Run();

private:
    void OnRun();
    void OnHandshake(beast::error_code ec);
    void DoRead();
    void OnRead(beast::error_code ec, std::size_t transferedBytes);
    void SendResponse(http::message_generator&& resp);
    void OnWrite(bool keepAlive, beast::error_code ec, std::size_t transferedBytes);
    void DoClose();
    void OnShutdown(beast::error_code ec);

private:
    beast::ssl_stream<beast::tcp_stream> stream_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
};

}

#endif // HTTPSESSION_H
