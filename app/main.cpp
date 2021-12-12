#include <common/logger/Logger.h>
#include <service/include/Gateway.h>

#include <service/src/acceptor/Acceptor.h>

#include <future>

int main() {
    std::shared_ptr<IService> service(new Gateway(), [](IService* p) { delete(p); });
    service->prepare();

    boost::asio::io_context context;
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address("127.0.0.1"), 8080);

    Acceptor a(context, ep);
    a.prepare();
    a.run();


    context.run();

    return 0;
}
