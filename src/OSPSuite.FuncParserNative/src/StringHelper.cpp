#include "FuncParser/StringHelper.h"

#include <algorithm>
#include <cctype>
#include <string>

namespace FuncParserNative
{
std::string StringHelper::Capitalize(std::string pInString)
{
    std::transform(pInString.begin(), pInString.end(), pInString.begin(), [](unsigned char c) {
        return std::toupper(c);
    });
    return pInString;
}

} // namespace FuncParserNative
