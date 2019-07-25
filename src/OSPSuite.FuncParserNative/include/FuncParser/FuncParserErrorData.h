#ifndef _FuncParserErrorData_H_
#define _FuncParserErrorData_H_

#include <string>
#include "FuncParser/FuncParserTypeDefs.h"

namespace FuncParserNative
{

class FuncParserErrorData
{	
	public:
		enum errNumber 
		{
			err_OK,
			err_RUNTIME,
			err_PARSE,
			err_BADARG,
			err_BADCALL,
			err_NUMERIC,
			err_DIMENSION,
			err_CANNOTCALC_DIMENSION,
         err_ERROR
		};
	
	private:
		errNumber _number;
		std::string _source;
		std::string _description;
	
	public:
		FUNCPARSER_EXPORT FuncParserErrorData ();
		FUNCPARSER_EXPORT FuncParserErrorData (errNumber Number, const std::string & Source, const std::string & Description);
		FUNCPARSER_EXPORT const errNumber GetNumber () const;
		FUNCPARSER_EXPORT void SetNumber (errNumber number);
		FUNCPARSER_EXPORT const std::string GetSource () const;
		FUNCPARSER_EXPORT void SetSource (const std::string & source);
		FUNCPARSER_EXPORT const std::string GetDescription () const;
		FUNCPARSER_EXPORT void SetDescription (const std::string & description);
		FUNCPARSER_EXPORT void operator = (const FuncParserErrorData & ED);
		FUNCPARSER_EXPORT void SetError (errNumber Number, const std::string & Source, const std::string & Description);
		FUNCPARSER_EXPORT void Clear ();
};

}//.. end "namespace FuncParserNative"


#endif //_FuncParserErrorData_H_

