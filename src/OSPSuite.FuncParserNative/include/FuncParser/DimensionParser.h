#ifndef _EF_DIMENSIONPARSER_H_
#define _EF_DIMENSIONPARSER_H_

#include <string>
#include "FuncParser/QuantityDimensionInfo.h"

namespace FuncParserNative
{

class DimensionParser
{
public:
    static DimensionInfo GetDimensionInfoFor(
        const std::string &formula, const std::vector<QuantityDimensionInfo> &quantityDimensions);
};

//-------------- C interface for PInvoke -----------------------------------------
struct QuantityDimensionInformationStructure
{
   double Length;
   double Mass;
   double Time;
   double ElectricCurrent;
   double Temperature;
   double Amount;
   double LuminousIntensity;
   char* QuantityName;
};

struct DimensionInformationStructure
{
   double Length;
   double Mass;
   double Time;
   double ElectricCurrent;
   double Temperature;
   double Amount;
   double LuminousIntensity;

   void CopyFrom(const DimensionInfo& dimensionInformation);
};
extern "C"
{
   FUNCPARSER_EXPORT DimensionInformationStructure GetDimensionInfoFor(
      const char* formula, const QuantityDimensionInformationStructure* quantityDimensionStructs,
      int size, bool& parseSuccess, bool& calculateDimensionSuccess, char** errorMessage);
}

}//.. end "namespace FuncParserNative"


#endif //_EF_DIMENSIONPARSER_H_

