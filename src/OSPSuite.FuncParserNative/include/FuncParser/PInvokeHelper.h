#ifndef _PInvokeHelper_H_
#define _PInvokeHelper_H_

#include <string>
#include "FuncParser/FuncParserErrorData.h"

namespace FuncParserNative
{
   char* MarshalString(const char* sourceString);
   char* MarshalString(const std::string & sourceString);
   char* ErrorMessageFrom(FuncParserErrorData& ED);
   char* ErrorMessageFromUnknown(const std::string & errorSource="");
}//.. end "namespace FuncParserNative"


#endif //_PInvokeHelper_H_

