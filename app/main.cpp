#include "Logger.h"

int main() {
    sdk_logger_t lg("main", "tag name");

    lg.trace("hello world!");
    lg.debug("hello world!");
    lg.info("hello world!");
    lg.warning("hello world!");
    lg.error("hello world!");
    lg.fatal("hello world!");

    return 0;
}
