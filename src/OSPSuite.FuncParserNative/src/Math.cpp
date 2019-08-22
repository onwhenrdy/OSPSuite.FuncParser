#include "FuncParser/Math.h"

#include <cmath>
#include <limits>

namespace FuncParserNative
{
double Math::GetNaN()
{
    return (std::numeric_limits<double>::quiet_NaN());
}

bool Math::IsNaN(double d)
{
    return (std::isnan(d));
}

double Math::GetInf ()
{
    return (std::numeric_limits<double>::infinity());
}

double Math::GetNegInf ()
{
	return -GetInf();
}

bool Math::IsFinite (double d)
{
    return (std::isfinite(d));
}

bool Math::IsInf (double d)
{
    return (d == GetInf());
}

bool Math::IsNegInf (double d)
{
    return (d == GetNegInf());
}

const int VAR_INVALID_INDEX = 0;

} // namespace FuncParserNative
