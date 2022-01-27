#include <service/include/Gateway.h>
#include <common/context/include/Context.h>

#include "session/include/SessionPeerBase.h"

int main() {
    logger_t lg("sdk", "main");

    namespace ba = boost::asio;
    Context ctx;
    ba::ip::tcp::endpoint ep(ba::ip::make_address("127.0.0.1"),
                             8080);

    sdk::Gateway service(ctx.getContext(), std::move(ep));
    service.run();

    ctx.getContext().run();

    return 0;
}
