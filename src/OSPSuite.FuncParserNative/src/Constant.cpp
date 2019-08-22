#include "FuncParser/Constant.h"

namespace FuncParserNative
{
Constant::Constant(std::string Name, double Value)
    : _name(std::move(Name))
    , _value(Value)
{}

const std::string &Constant::GetName() const
{
    return _name;
}

double Constant::GetValue() const
{
    return _value;
}

} // namespace FuncParserNative
