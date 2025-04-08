#include "server/httpsession.h"

namespace Http {

HttpSession::HttpSession(tcp::socket&& socket, ssl::context& ctx) 
    : stream_(std::move(socket), ctx_(ctx)) {
        // Set the timeout settings for the session
        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
}

void HttpSession::Run() {
    // Perform the SSL handshake
    stream_.async_handshake(ssl::stream_base::server,
                            beast::bind_front_handler(
                            &HttpSession::OnHandshake,
                            shared_from_this()));
}

void HttpSession::OnHandshake(beast::error_code ec, std::size_t usedBytes) {
    if (ec) {
        Fail(ec, "OnHandshake");
        return;
    }

    buffer_.consume(buffer_.size());
    DoRead();
}

void HttpSession::DoRead() {
    req_ = {};
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

    http::async_read(stream_, buffer_, req_,
                     beast::bind_front_handler(
                     &HttpSession::OnRead,
                     shared_from_this()));
}

void HttpSession::OnRead(beast::error_code ec, std::size_t transferedBytes) {
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

void HttpSession::DoWrite(http::response<http::string_body>&& res) {
    auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));

    http::async_write(stream_, 
                      *sp,
                      beast::bind_front_handler(
                        &HttpSession::OnWrite,
                        shared_from_this())
                      );
}

void HttpSession::OnWrite(beast::error_code ec, std::size_t transferedBytes) {
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

}