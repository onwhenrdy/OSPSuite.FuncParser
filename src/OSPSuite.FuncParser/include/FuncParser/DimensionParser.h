#ifndef _EF_DIMENSIONPARSER_H_
#define _EF_DIMENSIONPARSER_H_

#include <string>
#include "FuncParser/QuantityDimensionInfo.h"

namespace FuncParserNative
{

class DimensionParser
{
public:
	DimensionInfo GetDimensionInfoFor(const std::string & formula,
		                              const std::vector<QuantityDimensionInfo> & quantityDimensions,
									  FuncParserErrorData & ED);
};

}//.. end "namespace FuncParserNative"


#endif //_EF_DIMENSIONPARSER_H_

