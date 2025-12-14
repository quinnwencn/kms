#include "listener.h"
#include "httpsession.h"

namespace Http {

Listener::Listener(net::io_context& ioc, ssl::context& ctx, tcp::endpoint endpoint):
    ioc_(ioc), 
    ctx_(ctx),
    acceptor_(ioc) {
    beast::error_code ec;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
        Fail(ec, "open");
        return;
    }

    // Allow address reuse
    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if (ec) {
        Fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);
    if (ec) {
        Fail(ec, "bind");
        return;
    }   

    // Start listening for connections
    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
        Fail(ec, "listen");
        return;
    }
}

void Listener::DoAccept() {
    acceptor_.async_accept(net::make_strand(ioc_),
                           beast::bind_front_handler(
                               &Listener::OnAccept,
                               shared_from_this()));
}

void Listener::OnAccept(beast::error_code ec, tcp::socket socket) {
    if (ec) {
        Fail(ec, "accept");
        return;
    } else {
        // Create a new session and start it
        std::make_shared<HttpSession>(std::move(socket), ctx_)->Run();
    }

    // Accept new connections
    DoAccept();
}
    
}