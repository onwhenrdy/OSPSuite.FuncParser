#ifndef _EF_QUANTITYDIMENSIONINFO_H_
#define _EF_QUANTITYDIMENSIONINFO_H_

#include <string>
#include "FuncParser/DimensionInfo.h"

namespace FuncParserNative
{

class QuantityDimensionInfo
{
protected:
	std::string _quantityName;
	DimensionInfo _dimensionInfo;

	public:
		QuantityDimensionInfo();

		FUNCPARSER_EXPORT QuantityDimensionInfo(const std::string & quantityName, 
			                                    const DimensionInfo & dimensionInfo);

		FUNCPARSER_EXPORT const std::string & GetQuantityName() const;
		FUNCPARSER_EXPORT void SetQuantityName(const std::string & quantityName);

		FUNCPARSER_EXPORT DimensionInfo GetDimensionInfo() const;
};

}//.. end "namespace FuncParserNative"


#endif //_EF_QUANTITYDIMENSIONINFO_H_

