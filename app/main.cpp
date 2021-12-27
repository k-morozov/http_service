#include <service/include/Gateway.h>
#include <common/context/include/Context.h>

#include <service/src/acceptor/Acceptor.h>

#include "session/include/SessionPeerBase.h"

int main() {
    logger_t lg("sdk", "main");

    namespace ba = boost::asio;
    Context ctx;
    ba::ip::tcp::endpoint ep(ba::ip::make_address("127.0.0.1"),
                             8080);

    auto acceptor = std::make_unique<sdk::Acceptor>(ctx.getContext(), std::move(ep));

    sdk::Gateway service(std::move(acceptor));
    service.prepare();
    service.run();

    ctx.getContext().run();

    return 0;
}
