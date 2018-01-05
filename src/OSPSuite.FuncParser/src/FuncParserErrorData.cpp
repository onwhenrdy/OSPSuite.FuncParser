#ifdef _WINDOWS_PRODUCTION
#pragma managed(push,off)
#endif

#include "FuncParser/FuncParserErrorData.h"

#ifdef _WINDOWS_PRODUCTION
#pragma managed(pop)
#endif

namespace FuncParserNative
{

FuncParserErrorData::FuncParserErrorData ()
{
	m_Number = err_OK;
	m_Source = "";
	m_Description = "";
}

FuncParserErrorData::FuncParserErrorData (errNumber Number, const std::string & Source, const std::string & Description)
{
	m_Number = Number;
	m_Source = Source;
	m_Description = Description;
}

const FuncParserErrorData::errNumber FuncParserErrorData::GetNumber () const
{
    return m_Number;
}

void FuncParserErrorData::SetNumber (errNumber p_Number)
{
    m_Number=p_Number;
}

const std::string FuncParserErrorData::GetSource () const
{
    return m_Source;
}

void FuncParserErrorData::SetSource (const std::string & p_Source)
{
    m_Source=p_Source;
}

const std::string FuncParserErrorData::GetDescription () const
{
    return m_Description;
}

void FuncParserErrorData::SetDescription (const std::string & p_Description)
{
    m_Description=p_Description;
}

void FuncParserErrorData::operator = (const FuncParserErrorData & ED)
{
	m_Number = ED.GetNumber();
	m_Description = ED.GetDescription();
	m_Source = ED.GetSource();
}

void FuncParserErrorData::SetError (errNumber Number, const std::string & Source, const std::string & Description)
{
	m_Number = Number;
	m_Description = Description;
	m_Source = Source;
}

void FuncParserErrorData::Clear ()
{
	m_Number = err_OK;
	m_Description = "";
	m_Source = "";
}


}//.. end "namespace FuncParserNative"
