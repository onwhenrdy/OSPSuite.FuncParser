#include "FuncParser/Constant.h"
#include "FuncParser/Math.h"

namespace FuncParserNative
{

Constant::Constant (const std::string & Name, double Value)
{
	_name = Name;
	_value = Value;
}

const std::string & Constant::GetName () const
{
    return _name;
}

const double Constant::GetValue () const
{
    return _value;
}

}//.. end "namespace FuncParserNative"
