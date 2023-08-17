#include <gtest/gtest.h>

#include <filesystem>

#include "../thirdparty/easylog/easylog.hpp"

TEST(easylog, basic)
{
    std::string filename = "easylog.log";
    // std::filesystem::remove(filename);
    easylog::init_log(easylog::Severity::DEBUG, filename, false, true, 5000, 3, false);

    std::unique_ptr<int> ptr(new int(42));
    ELOG_INFO << ptr.get();
    ELOG_INFO << 42 << " " << 4.5 << 'a' << easylog::Severity::DEBUG;
    ELOG_INFO << false << ", " << true;

    ELOG_WARN << "out of range:" << 10;
    ELOG_ERROR << "crash:" << 10;
}