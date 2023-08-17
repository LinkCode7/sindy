#include "SindyString.h"

#include <fstream>
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

} // namespace sindy
