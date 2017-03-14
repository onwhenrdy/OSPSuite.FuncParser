#ifdef WIN32_PRODUCTION
#pragma managed(push,off)
#endif

#include "FuncParser/Math.h"
#ifdef WIN32

#include <math.h>
#include <ymath.h>
#include <float.h>
#endif
#ifdef linux
#include <cmath>
#endif

#ifdef WIN32_PRODUCTION
#pragma managed(pop)
#endif

namespace FuncParserNative
{

double Math::GetNaN ()
{
#ifdef WIN32
	return _Nan._Double;
#endif
#ifdef linux 
	return NAN;
#endif
}

bool Math::IsNaN (double d)
{
#ifdef WIN32
	return _isnan(d) ? true : false;
#endif
#ifdef linux
	return isnan(d);
#endif
}

double Math::GetInf ()
{
#ifdef WIN32
	return _Inf._Double;
#endif
#ifdef linux
	return INFINITY;
#endif
}

double Math::GetNegInf ()
{
	return -GetInf();
}

bool Math::IsFinite (double d)
{
#ifdef WIN32
	return _finite(d) ? true : false;
#endif
#ifdef linux
	return (finite(d) != 0);
#endif
}

bool Math::IsInf (double d)
{
	return (d==GetInf());
}

bool Math::IsNegInf (double d)
{
	return (d==GetNegInf());
}


const int VAR_INVALID_INDEX = 0;

}//.. end "namespace FuncParserNative"
