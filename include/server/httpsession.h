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
    HttpSession(tcp::socket&& socket, ssl::context& ctx) : stream_(std::move(socket), ctx_(ctx)) {
        // Set the timeout settings for the session
        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
    }

    ~HttpSession() = default;

    void Run() {
        // Perform the SSL handshake
        stream_.async_handshake(ssl::stream_base::server,
                                beast::bind_front_handler(
                                &HttpSession::OnHandshake,
                                shared_from_this()));
    }

private:
    void OnHandshake(beast::error_code ec, std::size_t usedBytes) {
        if (ec) {
            Fail(ec, "OnHandshake");
            return;
        }

        buffer_.consume(buffer_.size());
        DoRead();
    }

    void DoRead() {
        req_ = {};
        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

        http::async_read(stream_, buffer_, req_,
                         beast::bind_front_handler(
                         &HttpSession::OnRead,
                         shared_from_this()));
    }

    void OnRead(beast::error_code ec, std::size_t transferedBytes) {
        boost::ignore_unused(transferedBytes);

        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

        if (ec) {
            Fail(ec, "OnRead");
            return;
        }

        // Handle the request
        auto response = Router::Instance().Handle(req_);
        // set kms version?
        response.keep_alive(req_.keep_alive());
        DoWrite(std::move(response));
    }

    void DoWrite(http::response<http::string_body>&& res) {
        auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));

        http::async_write(stream_, 
                          *sp,
                          beast::bind_front_handler(
                            &HttpSession::OnWrite,
                            shared_from_this())
                          );
    }

    void OnWrite(beast::error_code ec, std::size_t transferedBytes) {
        boost::ignore_unused(transferedBytes);

        if (ec) {
            Fail(ec, "OnWrite");
            return;
        }

        // close the connection
        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
        stream_.async_shutdown(beast::bind_front_handler(
                                &HttpSession::OnShutdown,
                                shared_from_this())
                              );
    }

private:
    beast::ssl_stream<beast::tcp_stream> stream_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
};

}

#endif // HTTPSESSION_H
