#include "FuncParser/Constants.h"
#include "FuncParser/Constant.h"

#include <cmath>
#include <vector>

namespace FuncParserNative
{

Constants::Constants ()
{
    std::vector<Constant *> tmpConstants;
    tmpConstants.reserve(2);

    tmpConstants.push_back(new Constant("PI", 2 * std::asin(1.0)));
    tmpConstants.push_back(new Constant("E", std::exp(1.0)));

    for (auto element : tmpConstants)
    {
        _elemConstants[element->GetName()] = element;
    }
}

Constants::~Constants ()
{
    for (auto element : _elemConstants)
    {
        delete element.second;
    }
}

const Constant * Constants::operator [ ] (const std::string & Key) const
{
    auto iter = _elemConstants.find(Key);
    return (iter != _elemConstants.end()) ? iter->second : nullptr;
}

bool Constants::Exists(const std::string &Key) const
{
	return (_elemConstants.find(Key) != _elemConstants.end());
}

int Constants::GetCount() const
{
    // to prevent warning
    return static_cast<int>(_elemConstants.size());
}

std::string Constants::GetList() const
{
    std::string ConstList;
    // approximation to prevent costly reallocations
    ConstList.reserve(this->GetCount() * 5);

    for (const auto element : _elemConstants)
    {
        auto ElemConst = element.second;
        ConstList = ConstList + ElemConst->GetName() + " ";
    }

    return ConstList;
}

} // namespace FuncParserNative
