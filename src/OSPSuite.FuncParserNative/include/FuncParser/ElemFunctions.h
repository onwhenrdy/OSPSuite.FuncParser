#ifndef _ElemFunctions_H_
#define _ElemFunctions_H_

#ifdef _WINDOWS
// Disable warning about truncation of type name to 255 characters
#pragma warning(disable:4786)
#endif

#include <map>
#include <string>
class string;
#include "FuncParser/ElemFunction.h"
#include "FuncParser/FuncParserTypeDefs.h"

namespace FuncParserNative
{

class ElemFunction;

class ElemFunctions
{	
	
	private:
		static ElemFunctions * _elemFunctionsInstance;

		static int _numberOfElemFunctions;
		static ElemFunction ** _elemFunctions;
		static char ** _elemFunctionKeys;
		static ElemFunction::enmElemFunctionType * _elemFunctionTypes;
	
	public:
		ElemFunctions ();
		~ElemFunctions ();
		ElemFunction * operator [ ] (ElemFunction::enmElemFunctionType FuncType) const;
		ElemFunction * operator [ ] (const std::string & Key) const;
		bool Exists (ElemFunction::enmElemFunctionType FuncType);
		const bool Exists (const std::string & Key);
		const int GetCount ();
		const std::string GetList (bool pLogical = false, bool bBinary = false) const;
		static ElemFunctions * GetInstance ();
};

}//.. end "namespace FuncParserNative"


#endif //_ElemFunctions_H_

