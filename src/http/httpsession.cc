#include "httpsession.h"

namespace Http {

void HttpSession::Run() {
    net::dispatch(stream_.get_executor(),
                  beast::bind_front_handler(
                      &HttpSession::OnRun,
                      shared_from_this()));
}

void HttpSession::OnRun() {
    // Setup timeout
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

    // Perform TLS handshake
    stream_.async_handshake(ssl::stream_base::server,
                            beast::bind_front_handler(
                                &HttpSession::OnHandshake,
                                shared_from_this()));
}

void HttpSession::OnHandshake(beast::error_code ec) {
    if (ec) {
        Fail(ec, "OnHandshake");
        return;
    }

    DoRead();
}

void HttpSession::DoRead() {
    // Clear req before reading
    req_ = {};

    // Setup timeout
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

    // Read a request
    http::async_read(stream_, buffer_, req_,
                     beast::bind_front_handler(
                         &HttpSession::OnRead,
                         shared_from_this()));
}

void HttpSession::OnRead(beast::error_code ec, std::size_t transferedBytes) {
    boost::ignore_unused(transferedBytes);

    // Client closes the connection
    if (ec) {
        Fail(ec, "OnRead");
        return;
    }

    // Handle the request
    SendResponse(std::move(Router::Instance().Handle(std::move(req_))));
}

void HttpSession::SendResponse(http::message_generator&& resp) {
    bool keepAlive = resp.keep_alive();

    // Write the response
    beast::async_write(stream_,
                       std::move(resp),
                       beast::bind_front_handler(
                           &HttpSession::OnWrite,
                           shared_from_this(),
                           keepAlive));
}

void HttpSession::OnWrite(bool keepAlive, beast::error_code ec, std::size_t transferedBytes) {
    boost::ignore_unused(transferedBytes);

    if (ec) {
        Fail(ec, "OnWrite");
        return;
    }

    if (!keepAlive) {
        DoClose();
    }

    DoRead();
}

void HttpSession::DoClose() {
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

    // Perform ssl shutdown
    stream_.async_shutdown(beast::bind_front_handler(
                              &HttpSession::OnShutdown,
                              shared_from_this()));
}

void HttpSession::OnShutdown(beast::error_code ec) {
    if (ec) {
        Fail(ec, "OnShutdown");
    }

    // The connection closes here gracefully.
}

}