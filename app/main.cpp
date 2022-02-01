#include <service/include/Gateway.h>
#include <common/context/include/Context.h>
#include <common/pipeline/include/Pipeline.h>

int main() {
    logger_t lg("sdk", "main");

    namespace ba = boost::asio;

    Context ctx;
    ba::ip::tcp::endpoint ep(ba::ip::make_address("127.0.0.1"),
                             8080);

    sdk::Gateway service(ctx.getContext(), ep);

    auto pipeline = std::make_unique<sdk::Pipeline>();

    auto print = [](sdk::Connection::request_t const& req)
            -> sdk::Core::error_code
        { std::cout << req << std::endl; return {}; };
    pipeline->append_handler(std::move(print));

    service.setup_pipeline(std::move(pipeline));
    service.run();

    ctx.getContext().run();
    return 0;
}
