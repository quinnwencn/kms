#include <vector>
#include <thread>

#include "server/listener.h"

int main(int argc, char **argv) {
    auto const address = "0.0.0.0";
    uint16_t const port = 8443;
    auto const thread_num = 4;

    net::io_context ioc { thread_num };
    ssl::context ctx { ssl::context::tlsv12 };

    ctx.use_certificate_chain_file("/home/quinn/work/certificates/server.crt");
    ctx.use_private_key_file("/home/quinn/work/certificates/server.key", boost::asio::ssl::context::pem);
    ctx.load_verify_file("/home/quinn/work/certificates/ca-chain.crt");

    // mTLS
    // verify client's certificate, if client has no certificate, refuse connection
    ctx.set_verify_mode(ssl::verify_peer | ssl::verify_fail_if_no_peer_cert);

    std::make_shared<Http::Listener>(ioc, ctx, tcp::endpoint { net::ip::make_address(address), port})->Run();

    std::vector<std::thread> threads;
    threads.reserve(thread_num - 1);
    for (auto i = thread_num - 1; i > 0; --i) {
        threads.emplace_back([&ioc] {
            ioc.run();
        });
    }

    ioc.run();

    return 0;
}