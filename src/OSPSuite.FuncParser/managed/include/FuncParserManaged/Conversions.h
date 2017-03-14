#ifndef _Managed_Conversions_H_
#define _Managed_Conversions_H_

#include <string>
#include <vector>
#include "FuncParser/QuantityDimensionInfo.h"
#include "FuncParserManaged/ManagedDimensionParser.h"

namespace OSPSuite
{
	namespace FuncParser
	{
		using namespace System::Collections::Generic;

		ref class NETToCPPConversions
		{
		public:
			static std::string MarshalString(System::String^ s);
			static void DoubleArrayToVector(array<double>^ doubleArray, std::vector<double> & doubleVec);
			static void StringArrayToVector(array<System::String^>^ stringArray, std::vector<std::string> & stringVec);
			static void StringEnumerableToVector(IEnumerable<System::String^>^ stringList, std::vector<std::string> & stringVec);
			static void QuantityDimensionInfoEnumerableToVector(IEnumerable<IQuantityDimensionInfo^>^ quantityDimensionList, std::vector<FuncParserNative::QuantityDimensionInfo> & quantityDimensionVec);
		};

		ref class CPPToNETConversions
		{
		public:
			static System::String^ MarshalString(const std::string & s);
			static array<double>^ DoubleVectorToArray(const std::vector<double> & doubleVec);
			static array<System::String^>^ StringVectorToArray(const std::vector<std::string> & stringVec);
			static IList<System::String^>^ StringVectorToList(const std::vector<std::string> & stringVec);
		};
	}//.. end "namespace FuncParser"
}//.. end "namespace OSPSuite"


#endif //_Managed_Conversions_H_

