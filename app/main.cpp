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

    auto controller = pipeline->get_controller();
    controller->subscribe([](){ std::cout << "have signal by cancel" << std::endl; });

    auto print = [controller](sdk::Pipeline::request_t const& req)
            -> sdk::Pipeline::error_code
        {
            std::cout << "have request. enter char" << std::endl;
            char ch;
            std::cin >> ch;
            std::cout << "char = " << ch << std::endl;
            if (!controller->is_cancel())
            {
                controller->cancel();
                std::cout << "send cancel" << std::endl;
            }
            return {};
        };
    for(int i=0; i<5; ++i)
        pipeline->append_handler(print);



    service.setup_pipeline(std::move(pipeline));
    service.run();

    ctx.getContext().run();
    return 0;
}
