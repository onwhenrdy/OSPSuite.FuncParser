#include "FuncParser/FuncParserErrorData.h"

namespace FuncParserNative
{

FuncParserErrorData::FuncParserErrorData ()
{
	_number = err_OK;
	_source = "";
	_description = "";
}

FuncParserErrorData::FuncParserErrorData (errNumber Number, const std::string & Source, const std::string & Description)
{
	_number = Number;
	_source = Source;
	_description = Description;
}

const FuncParserErrorData::errNumber FuncParserErrorData::GetNumber () const
{
    return _number;
}

void FuncParserErrorData::SetNumber (errNumber number)
{
    _number=number;
}

const std::string FuncParserErrorData::GetSource () const
{
    return _source;
}

void FuncParserErrorData::SetSource (const std::string & source)
{
    _source=source;
}

const std::string FuncParserErrorData::GetDescription () const
{
    return _description;
}

void FuncParserErrorData::SetDescription (const std::string & description)
{
    _description=description;
}

void FuncParserErrorData::operator = (const FuncParserErrorData & ED)
{
	_number = ED.GetNumber();
	_description = ED.GetDescription();
	_source = ED.GetSource();
}

void FuncParserErrorData::SetError (errNumber Number, const std::string & Source, const std::string & Description)
{
	_number = Number;
	_description = Description;
	_source = Source;
}

void FuncParserErrorData::Clear ()
{
	_number = err_OK;
	_description = "";
	_source = "";
}


}//.. end "namespace FuncParserNative"
