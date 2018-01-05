#ifdef _WINDOWS_PRODUCTION
#pragma managed(push,off)
#endif

#include "FuncParser/FuncParserErrorData.h"
#include "FuncParser/DimensionParser.h"
#include "FuncParser/ParsedFunction.h"

#ifdef _WINDOWS_PRODUCTION
#pragma managed(pop)
#endif

namespace FuncParserNative
{
	using namespace std;

	DimensionInfo DimensionParser::GetDimensionInfoFor(const string & formula,
		                                               const vector<QuantityDimensionInfo> & quantityDimensions,
													   FuncParserErrorData & ED)
	{
		ParsedFunction pf;
		return pf.GetDimensionInfoFor(formula, quantityDimensions, ED);
	}

}//.. end "namespace FuncParserNative"
