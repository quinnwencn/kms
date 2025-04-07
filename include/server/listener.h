#ifndef LISTENER_H
#define LISTENER_H

#include <memory>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ssl.hpp>

namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

namespace Http {

class Listener : public std::enable_shared_from_this<Listener> {
public:
    Listener(net::io_context& ioc, ssl::context& ctx, tcp::endpoint endpoint):
        ioc_(ioc), 
        ctx_(ctx),
        acceptor_(net::make_strand(ioc)) {
        beast::error_code ec;
        acceptor_.open(endpoint.protocol(), ec);
        if (ec) {
            Fail(ec, "open");
            return;
        }

        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if (ec) {
            Fail(ec, "set_option");
            return;
        }

        acceptor_.bind(endpoint, ec);
        if (ec) {
            Fail(ec, "bind");
            return;
        }   

        acceptor_.listen(net::socket_base::max_listen_connections, ec);
        if (ec) {
            Fail(ec, "listen");
            return;
        }

        // std::cout << "Listening on " << endpoint.address().to_string() << ":" << endpoint.port() << "\n";
    }

    ~Listener() = default;

    void Run() { DoAccept(); }

private:
    void DoAccept() {
        acceptor_.async_accept(net::make_strand(ioc_),
                               beast::bind_front_handler(
                               &Listener::OnAccept,
                               shared_from_this()));
    }

    void OnAccept(beast::error_code ec, tcp::socket socket) {
        if (ec) {
            Fail(ec, "accept");
        } else {
            // Create a new session and start it
            std::make_shared<HttpSession>(std::move(socket), ctx_)->Run();
        }

        DoAccept();
    }

private:
    net::io_context& ioc_;
    ssl::context& ctx_;
    tcp::acceptor acceptor_;
};

}

#endif // LISTENER_H
