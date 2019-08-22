#ifndef _Math_H_
#define _Math_H_

#include "FuncParser/FuncParserTypeDefs.h"

namespace FuncParserNative
{

class Math
{
public:
    FUNCPARSER_EXPORT static double GetNaN();
    FUNCPARSER_EXPORT static bool IsNaN(double d);
    FUNCPARSER_EXPORT static double GetInf();
    FUNCPARSER_EXPORT static double GetNegInf();
    FUNCPARSER_EXPORT static bool IsFinite(double d);
    FUNCPARSER_EXPORT static bool IsInf(double d);
    FUNCPARSER_EXPORT static bool IsNegInf(double d);
};

extern const int VAR_INVALID_INDEX;

}//.. end "namespace FuncParserNative"


#endif //_Math_H_

