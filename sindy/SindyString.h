#ifndef SINDY_STRING_H
#define SINDY_STRING_H

namespace sindy
{
bool readContents(std::string const& filename, std::string& text);
void writeContents(std::string const& filename, std::string const& text);

// 从二进制文件中读取流
void readStream(char const* filename, uint8_t*& data, uint64_t& size);
// 将流写入到文件
void writeStream(char const* filename, uint8_t*& data, uint64_t& size);

inline std::string trimExtension(std::string const& filename)
{
    auto pos = filename.rfind('.');
    if (pos == std::string::npos)
        return std::string();
    return filename.substr(0, pos);
}

// 数据流转十六进制字符串
std::string stream2HexString(uint8_t* data, size_t length);
// 十六进制字符串转数据流
void hexString2Stream(std::string const& hexString, uint8_t*& data, size_t& length);

} // namespace sindy

#endif // !SINDY_STRING_H
