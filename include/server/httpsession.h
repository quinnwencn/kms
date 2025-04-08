#ifndef HTTPSESSION_H
#define HTTPSESSION_H

#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>

#include "http/router.h"

namespace beast = boost::beast;
namespace http = beast::http;  

namespace Http {

static void Fail(beast::error_code ec, char const* what) {
    if (ec == net::ssl::error::stream_truncated) {
        return;
    }

    std::cerr << what << ": " << ec.message() << "\n";
}

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    HttpSession(tcp::socket&& socket, ssl::context& ctx);
    ~HttpSession() = default;

    void Run();

private:
    void OnHandshake(beast::error_code ec, std::size_t usedBytes);
    void DoRead();
    void OnRead(beast::error_code ec, std::size_t transferedBytes);
    void DoWrite(http::response<http::string_body>&& res);
    void OnWrite(beast::error_code ec, std::size_t transferedBytes);

private:
    beast::ssl_stream<beast::tcp_stream> stream_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
};

}

#endif // HTTPSESSION_H
