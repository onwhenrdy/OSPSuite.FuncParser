#include "FuncParserManaged/Conversions.h"

using namespace System::Runtime::InteropServices;

namespace OSPSuite
{
	namespace FuncParser
	{
		//---- NET to C++ types conversions
		//

		std::string NETToCPPConversions::MarshalString(System::String^ s)
		{
			if (System::String::IsNullOrEmpty(s))
				return "";

			const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
			std::string os = chars;
			Marshal::FreeHGlobal(System::IntPtr((void*)chars));
			return os;
		}

		void NETToCPPConversions::DoubleArrayToVector(array<double>^ doubleArray, std::vector<double> & doubleVec)
		{
			doubleVec.clear();

			for (int i = doubleArray->GetLowerBound(0); i <= doubleArray->GetUpperBound(0); i++)
			{
				double d = doubleArray[i];
				doubleVec.push_back(d);
			}
		}

		void NETToCPPConversions::StringArrayToVector(array<System::String^>^ stringArray, std::vector<std::string> & stringVec)
		{
			stringVec.clear();

			for (int i = stringArray->GetLowerBound(0); i < stringArray->GetUpperBound(0); i++)
				stringVec.push_back(MarshalString(stringArray[i]));
		}

		void NETToCPPConversions::StringEnumerableToVector(IEnumerable<System::String^>^ stringList, std::vector<std::string> & stringVec)
		{
			stringVec.clear();

			for each(System::String^ s in stringList)
				stringVec.push_back(MarshalString(s));
		}

		void NETToCPPConversions::QuantityDimensionInfoEnumerableToVector(IEnumerable<IQuantityDimensionInfo^>^ quantityDimensionList, std::vector<FuncParserNative::QuantityDimensionInfo> & quantityDimensionVec)
		{
			quantityDimensionVec.clear();

			for each(IQuantityDimensionInfo^ quantityDimInfo in quantityDimensionList)
			{
				QuantityDimensionInfo^ quantityDimInfoClass = (QuantityDimensionInfo^)quantityDimInfo;
				quantityDimensionVec.push_back(*(quantityDimInfoClass->GetNativeQuantityDimensionInfo()));
			}
		}

		//---- C++ to NET types conversions
		//

		System::String^ CPPToNETConversions::MarshalString(const std::string & s)
		{
			return gcnew System::String(s.c_str());
		}

		array<double>^ CPPToNETConversions::DoubleVectorToArray(const std::vector<double> & doubleVec)
		{
			array<double>^ doubleArray = gcnew array<double>(doubleVec.size());;

			for (unsigned int i = 0; i < doubleVec.size(); i++)
				doubleArray[i] = doubleVec[i];

			return doubleArray;
		}

		array<System::String^>^ CPPToNETConversions::StringVectorToArray(const std::vector<std::string> & stringVec)
		{
			array<System::String^>^ stringArray = gcnew array<System::String^>(stringVec.size());;

			for (unsigned int i = 0; i < stringVec.size(); i++)
				stringArray[i] = MarshalString(stringVec[i]);

			return stringArray;
		}

		IList<System::String^>^ CPPToNETConversions::StringVectorToList(const std::vector<std::string> & stringVec)
		{
			IList<System::String^>^ stringList = gcnew List<System::String^>();;

			for (unsigned int i = 0; i < stringVec.size(); i++)
				stringList->Add(MarshalString(stringVec[i]));

			return stringList;
		}
	}

}