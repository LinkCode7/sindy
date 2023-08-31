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
    std::ostringstream ss;
    ss << std::setfill('0') << std::hex;
    for (auto i = 0; i < length; ++i)
        ss << std::setw(2) << static_cast<unsigned int>(data[i]);
    return ss.str();
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

} // namespace sindy
