//
// Created by Quinn Wen on 2025/12/1.
//

#include "kms/kms.h"

#include <vector>

#include "listener.h"

namespace kms {

void Kms::Start() {
    auto threadNum = config_.ServerConf().ThreadNumber();
    net::io_context ioc { threadNum };
    ssl::context ctx { ssl::context::tlsv12 };

    ctx.use_certificate_chain_file(config_.TLSConf().Certificate());
    ctx.use_private_key_file(config_.TLSConf().PrivateKey(), boost::asio::ssl::context::pem);
    ctx.load_verify_file(config_.TLSConf().CaCertificate());

    // mTLS
    // verify client's certificate, if client has no certificate, refuse connection
    if (config_.TLSConf().VerifyPeer()) {
        ctx.set_verify_mode(ssl::verify_peer | ssl::verify_fail_if_no_peer_cert);
    }

    std::make_shared<Http::Listener>(ioc, ctx,
        tcp::endpoint { net::ip::make_address(config_.ServerConf().Address()),
                        config_.ServerConf().Port()})->Run();

    std::vector<std::thread> threads;
    threads.reserve(threadNum - 1);
    for (auto i = threadNum - 1; i > 0; --i) {
        threads.emplace_back([&ioc] {
            ioc.run();
        });
    }

    ioc.run();

}

}
