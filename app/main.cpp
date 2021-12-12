#include <common/logger/Logger.h>
#include <service/include/Gateway.h>


int main() {
    std::shared_ptr<IService> service(new Gateway(), [](IService* p) { delete(p); });
    service->prepare();


    return 0;
}
