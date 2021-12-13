#include <common/logger/Logger.h>
#include <service/include/Gateway.h>
#include <common/context/include/Context.h>

#include <service/src/acceptor/Acceptor.h>

#include <future>

int main() {
    std::shared_ptr<IService> service(new Gateway(), [](IService* p) { delete(p); });
    service->prepare();

    Context ctx;
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address("127.0.0.1"), 8080);

    Acceptor a(ctx.getContext(), ep);
    a.prepare();
    a.run();

    char c;
    std::cin >> c;


    return 0;
}
