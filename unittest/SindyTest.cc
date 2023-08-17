#include <gtest/gtest.h>

#include <filesystem>

#include "../sindy/RunTime.h"
#include "../sindy/SindyString.h"

#define SPLIT_CHAR '\n'
#define TEST_DATA_DIR "../unittest/data/stream/"

void testGetLine(std::string const& contents)
{
    size_t cursor = 0;
    size_t length = contents.size();

    while (cursor < length)
    {
        int64_t index = contents.find(SPLIT_CHAR, cursor);
        if (index < 0)
            index = length;

        auto count = index - cursor;
        auto line  = contents.substr(cursor, count);
        cursor     = index + 1;
        // ...
    }
}

void testGetLine2(std::string const& contents)
{
    auto cursor = contents.begin();
    auto end    = contents.end();

    while (cursor < end)
    {
        auto begin = cursor;
        for (; cursor != end; ++cursor)
        {
            if (*cursor == SPLIT_CHAR)
                break;
        }

        if (cursor == end)
            break;
        std::string line(begin, cursor);
        ++cursor;
        // ...
    }
}

TEST(sindy, getline)
{
    namespace fs = std::filesystem;
    fs::path path(TEST_DATA_DIR);

    std::string contents;
    for (auto& el : fs::directory_iterator(path, fs::directory_options::skip_permission_denied))
    {
        if (fs::is_directory(el.path()))
            continue;

        auto filename(el.path().filename().string());

        sindy::readContents(TEST_DATA_DIR + filename, contents);
        if (contents.size() > 0)
            break;
    }

    // debug模式下testGetLine快很多，release模式下差不多
    sindy::RunTime time("getline");

    testGetLine(contents);
    time.addTimePoint("testGetLine");

    testGetLine2(contents);
    time.addTimePoint("testGetLine2");

    time.console();
}
