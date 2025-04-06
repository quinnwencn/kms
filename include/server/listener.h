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
    Listener();

private:
    net::io_context& ioc_;
    ssl::context& ctx_;
    tcp::acceptor acceptor_;
};

}

#endif // LISTENER_H
