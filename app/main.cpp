#include <iostream>
#include "common/logger/include/logger.h"

int main() {
    Logger lg(sdk::Severity::Info);
    lg.info("hello world!");

    return 0;
}
