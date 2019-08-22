#include "FuncParser/FuncParserErrorData.h"

namespace FuncParserNative
{
FuncParserErrorData::FuncParserErrorData()
    : _number(err_OK)
{
}

FuncParserErrorData::FuncParserErrorData(errNumber Number,
                                         std::string Source,
                                         std::string Description)
    : _source(std::move(Source))
    , _description(std::move(Description))
    , _number(Number)
{}

FuncParserErrorData::errNumber FuncParserErrorData::GetNumber() const
{
    return _number;
}

void FuncParserErrorData::SetNumber (errNumber number)
{
    _number=number;
}

const std::string &FuncParserErrorData::GetSource() const
{
    return _source;
}

void FuncParserErrorData::SetSource(std::string source)
{
    _source = std::move(source);
}

const std::string &FuncParserErrorData::GetDescription() const
{
    return _description;
}

void FuncParserErrorData::SetDescription(std::string description)
{
    _description = std::move(description);
}

void FuncParserErrorData::SetError(errNumber Number, std::string Source, std::string Description)
{
    _number = Number;
    _description = std::move(Description);
    _source = std::move(Source);
}

void FuncParserErrorData::Clear ()
{
    _number = err_OK;
    _description.clear();
    _source.clear();
}

} // namespace FuncParserNative
