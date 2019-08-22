#include "FuncParser/QuantityDimensionInfo.h"

namespace FuncParserNative
{
QuantityDimensionInfo::QuantityDimensionInfo(std::string quantityName,
                                             const DimensionInfo &dimensionInfo)
    : _quantityName(std::move(quantityName))
    , _dimensionInfo(dimensionInfo)
{}

const std::string &QuantityDimensionInfo::GetQuantityName() const
{
    return _quantityName;
}

void QuantityDimensionInfo::SetQuantityName(std::string quantityName)
{
    _quantityName = std::move(quantityName);
}

DimensionInfo QuantityDimensionInfo::GetDimensionInfo() const
{
    return _dimensionInfo;
}

} // namespace FuncParserNative
