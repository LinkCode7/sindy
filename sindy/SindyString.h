#ifndef SINDY_STRING_H
#define SINDY_STRING_H

namespace sindy
{
bool readContents(std::string const& filename, std::string& text);
void writeContents(std::string const& filename, std::string const& text);

inline std::string trimExtension(std::string const& filename)
{
    auto pos = filename.rfind('.');
    if (pos == std::string::npos)
        return std::string();
    return filename.substr(0, pos);
}

} // namespace sindy

#endif // !SINDY_STRING_H
