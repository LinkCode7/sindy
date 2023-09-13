#include "SindyString.h"

#include <fstream>
#include <iomanip>
#include <sstream>

namespace sindy
{
bool readContents(std::string const& filename, std::string& text)
{
    std::fstream fs;
    fs.open(filename, std::ios::in);
    if (!fs.is_open())
        return false;

    std::stringstream ss;
    ss << fs.rdbuf();
    fs.close();

    text = ss.str();
    return true;
}

void writeContents(std::string const& filename, std::string const& text)
{
    std::fstream fs;
    fs.open(filename, std::ios::out);
    fs.write(text.c_str(), text.size());
    fs.close();
}

void readStream(char const* filename, uint8_t*& data, uint64_t& size)
{
    FILE* fp = fopen(filename, "rb");
    if (!fp)
        return;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    data = new uint8_t[size];
    memset(data, 0, size);

    fseek(fp, 0, SEEK_SET);
    fread(data, sizeof(char), size, fp);
    fclose(fp);
}

void writeStream(char const* filename, uint8_t*& data, uint64_t& size)
{
    FILE* fp = fopen(filename, "wb+");
    if (fp)
    {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

// 数据流转十六进制字符串
std::string stream2HexString(uint8_t* data, size_t length)
{
    constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    std::string str(length * 2, ' ');
    for (int i = 0; i < length; ++i)
    {
        str[2 * i]     = hexmap[(data[i] & 0xF0) >> 4];
        str[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return str;
}

// 十六进制字符串转数据流
void hexString2Stream(std::string const& hexString, uint8_t*& data, size_t& length)
{
    auto size = hexString.size();
    length    = size / 2;
    data      = new uint8_t[length];

    for (size_t i = 0; i < size; i += 2)
    {
        std::string   byte_str = hexString.substr(i, 2);
        unsigned char byte     = static_cast<unsigned char>(stoi(byte_str, nullptr, 16));
        data[i / 2]            = byte;
    }
}

std::string trimSpaces(std::string const& str)
{
    // size_t start = str.find_first_not_of(" ");
    // size_t end   = str.find_last_not_of(" ");
    // return str.substr(start, end - start + 1);
    return {}; // zh
}

std::string trimFrontSpaces(std::string const& str)
{
    size_t offset = 0;
    auto   size   = str.size();
    for (auto i = 0; i < size; ++i)
    {
        if (str[i] != ' ')
            break;
        ++offset;
    }
    return str.substr(offset, size);
}
std::string trimBackSpaces(std::string const& str)
{
    auto   size = str.size();
    size_t end  = size;
    for (int i = size - 1; i >= 0; --i)
    {
        if (str[i] != ' ')
            break;
        --end;
    }
    if (end < size)
        return std::string(str.begin(), str.begin() + end);
    return str;
}

std::string trimConsecutiveSpaces(std::string const& str)
{
    auto start = str.find_first_not_of(" ");
    auto end   = str.find_last_not_of(" ");

    auto size = str.size();
    for (auto i = start; i <= end; ++i)
    {
        // zh
    }
    return {};
}

std::string removeAnnotation(std::string const& oneLine)
{
    std::vector<std::pair<int, int>> arr;

    // "fun()/* // */"   "fun()/*  */  --  /* // */"
    auto size = oneLine.size();
    for (auto i = 0; i < size;)
    {
        auto left = oneLine.find("/*", i);
        if (left == std::string::npos)
            break;

        auto right = oneLine.find("*/", left + 2);
        if (right != std::string::npos)
        {
            arr.emplace_back(std::make_pair(left, right));
            i = right + 2;
        }
        else
        {
            arr.emplace_back(std::make_pair(left, -1));
            break;
        }
    }

    std::string            line;
    std::string::size_type pos = std::string::npos;
    for (auto i = 0; i < size;)
    {
        pos = oneLine.find("//", i);
        if (pos == std::string::npos)
        {
            line = oneLine;
            break;
        }

        // "/*hello*//*world*/"
        if ((pos > 0 && oneLine[pos - 1] == '*') && (size - pos >= 3 && oneLine[pos + 2] == '*'))
        {
            i = pos + 3;
            continue;
        }

        int annotateRight = -1;
        for (auto const& [left, right] : arr)
        {
            if (left < pos && pos < right)
            {
                annotateRight = right;
                break;
            }
        }
        if (annotateRight != -1)
        {
            i = annotateRight + 2;
            continue;
        }

        size_t count = 0;
        for (auto iter = arr.rbegin(); iter != arr.rend(); ++iter)
        {
            if (iter->first < pos) // 此时不可能出现"/* // */"的情况
                break;
            ++count;
        }
        for (auto i = 0; i < count; ++i)
            arr.pop_back();

        // 裁剪
        line = oneLine.substr(0, pos);
        break;
    }

    if (line.empty())
        return line;

    pos         = 0;
    size        = arr.size();
    auto length = line.size();

    std::stringstream ss;
    for (auto i = 0; i < size; ++i)
    {
        ss << line.substr(pos, arr[i].first - pos);
        if (arr[i].second == -1)
            break;

        if (i == size - 1)
        {
            ss << line.substr(arr[i].second + 2, length - arr[i].second);
            break;
        }
        pos = arr[i].second + 2;
    }
    return size > 0 ? ss.str() : line;
}

} // namespace sindy
