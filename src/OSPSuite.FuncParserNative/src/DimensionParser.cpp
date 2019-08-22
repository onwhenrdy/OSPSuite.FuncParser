#include "FuncParser/FuncParserErrorData.h"
#include "FuncParser/DimensionParser.h"
#include "FuncParser/ParsedFunction.h"
#include "FuncParser/PInvokeHelper.h"

namespace FuncParserNative
{
	using namespace std;

	DimensionInfo DimensionParser::GetDimensionInfoFor(const string & formula,
		                                               const vector<QuantityDimensionInfo> & quantityDimensions)
	{
		ParsedFunction pf;
        return pf.GetDimensionInfoFor(formula, quantityDimensions);
    }

    //-------------- C interface for PInvoke -----------------------------------------
    DimensionInformationStructure GetDimensionInfoFor(
        const char *formula,
        const QuantityDimensionInformationStructure *quantityDimensionStructs,
        int size,
        bool &parseSuccess,
        bool &calculateDimensionSuccess,
        char **errorMessage)
    {
        DimensionInformationStructure dimension{};

        try
        {
            vector<QuantityDimensionInfo> quantityDimensions;
            quantityDimensions.reserve(size);

            for (auto i = 0; i < size; i++)
            {
                auto dimInfoStruct = quantityDimensionStructs[i];
                DimensionInfo dimensionInfo(dimInfoStruct.Length,
                                            dimInfoStruct.Mass,
                                            dimInfoStruct.Time,
                                            dimInfoStruct.ElectricCurrent,
                                            dimInfoStruct.Temperature,
                                            dimInfoStruct.Amount,
                                            dimInfoStruct.LuminousIntensity);

                quantityDimensions.push_back(
                    QuantityDimensionInfo(dimInfoStruct.QuantityName, dimensionInfo));
            }

            dimension.CopyFrom(DimensionParser::GetDimensionInfoFor(formula, quantityDimensions));

            parseSuccess = true;
            calculateDimensionSuccess = true;
        } catch (FuncParserErrorData &ED)
        {
            calculateDimensionSuccess = false;
            parseSuccess = ED.GetNumber() == FuncParserErrorData::err_DIMENSION
                           || ED.GetNumber() == FuncParserErrorData::err_CANNOTCALC_DIMENSION;
            *errorMessage = ErrorMessageFrom(ED);
        } catch (...)
        {
            parseSuccess = false;
            calculateDimensionSuccess = false;
            *errorMessage = ErrorMessageFromUnknown("GetDimensionInfoFor");
        }

        return dimension;
    }

   void DimensionInformationStructure::CopyFrom(const DimensionInfo& dimensionInformation)
	{
      Length = dimensionInformation.GetLength();
      Mass = dimensionInformation.GetMass();
      Time = dimensionInformation.GetTime();
      ElectricCurrent = dimensionInformation.GetElectricCurrent();
      Temperature = dimensionInformation.GetTemperature();
      Amount = dimensionInformation.GetAmount();
      LuminousIntensity = dimensionInformation.GetLuminousIntensity();
	}
}//.. end "namespace FuncParserNative"
