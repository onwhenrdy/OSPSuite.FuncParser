#include "FuncParserManaged/Conversions.h"
#include "FuncParserManaged/ManagedDimensionParser.h"
#include "FuncParserManaged/ExceptionHelper.h"

namespace OSPSuite
{
	namespace FuncParser
	{
		//-----------------------------------------------------------------------------
		//---- Managed dimension info
		//-----------------------------------------------------------------------------
		FuncParserNative::DimensionInfo * DimensionInfo::GetNativeDimensionInfo()
		{
			return _dimensionInfo;
		}

		//internal constructors
		DimensionInfo::DimensionInfo(FuncParserNative::DimensionInfo * dimensionInfo)
		{
			_dimensionInfo = dimensionInfo;
		}
		DimensionInfo::DimensionInfo(const FuncParserNative::DimensionInfo & dimensionInfo)
		{
			_dimensionInfo = new FuncParserNative::DimensionInfo(
				dimensionInfo.GetLength(), dimensionInfo.GetMass(), dimensionInfo.GetTime(),
				dimensionInfo.GetElectricCurrent(), dimensionInfo.GetTemperature(),
				dimensionInfo.GetAmount(), dimensionInfo.GetLuminousIntensity());
		}

		//constructors
		DimensionInfo::DimensionInfo(double length, double mass, double time,
			double electricCurrent, double temperature,
			double amount, double luminousIntensity)
		{
			_dimensionInfo = new FuncParserNative::DimensionInfo(length, mass, time,
				electricCurrent, temperature,
				amount, luminousIntensity);
		}

		DimensionInfo::DimensionInfo()
		{
			_dimensionInfo = new FuncParserNative::DimensionInfo();
		}

		// destructor
		DimensionInfo::~DimensionInfo()
		{
			if (_dimensionInfo)
				delete _dimensionInfo;
			_dimensionInfo = NULL;
		}

		// finalizer
		DimensionInfo::!DimensionInfo()
		{
			if (_dimensionInfo)
				delete _dimensionInfo;
			_dimensionInfo = NULL;
		}

		double DimensionInfo::Length::get()
		{
			return _dimensionInfo->GetLength();
		}

		void DimensionInfo::Length::set(double value)
		{
			_dimensionInfo->SetLength(value);
		}

		double DimensionInfo::Mass::get()
		{
			return _dimensionInfo->GetMass();
		}

		void DimensionInfo::Mass::set(double value)
		{
			_dimensionInfo->SetMass(value);
		}

		double DimensionInfo::Time::get()
		{
			return _dimensionInfo->GetTime();
		}

		void DimensionInfo::Time::set(double value)
		{
			_dimensionInfo->SetTime(value);
		}

		double DimensionInfo::ElectricCurrent::get()
		{
			return _dimensionInfo->GetElectricCurrent();
		}

		void DimensionInfo::ElectricCurrent::set(double value)
		{
			_dimensionInfo->SetElectricCurrent(value);
		}

		double DimensionInfo::Temperature::get()
		{
			return _dimensionInfo->GetTemperature();
		}

		void DimensionInfo::Temperature::set(double value)
		{
			_dimensionInfo->SetTemperature(value);
		}

		double DimensionInfo::Amount::get()
		{
			return _dimensionInfo->GetAmount();
		}

		void DimensionInfo::Amount::set(double value)
		{
			_dimensionInfo->SetAmount(value);
		}

		double DimensionInfo::LuminousIntensity::get()
		{
			return _dimensionInfo->GetLuminousIntensity();
		}

		void DimensionInfo::LuminousIntensity::set(double value)
		{
			_dimensionInfo->SetLuminousIntensity(value);
		}

		//-----------------------------------------------------------------------------
		//---- Managed quantity dimension info
		//-----------------------------------------------------------------------------
		FuncParserNative::QuantityDimensionInfo * QuantityDimensionInfo::GetNativeQuantityDimensionInfo()
		{
			return _quantityDimensionInfo;
		}

		QuantityDimensionInfo::QuantityDimensionInfo(System::String^ quantityName,
			IDimensionInfo^ dimensionInfo)
		{
			OSPSuite::FuncParser::DimensionInfo^ dimensionInfoClass = (OSPSuite::FuncParser::DimensionInfo^)dimensionInfo;
			_quantityDimensionInfo = new FuncParserNative::QuantityDimensionInfo(
				NETToCPPConversions::MarshalString(quantityName),
				*dimensionInfoClass->GetNativeDimensionInfo());
		}

		// destructor
		QuantityDimensionInfo::~QuantityDimensionInfo()
		{
			if (_quantityDimensionInfo)
				delete _quantityDimensionInfo;
			_quantityDimensionInfo = NULL;
		}

		// finalizer
		QuantityDimensionInfo::!QuantityDimensionInfo()
		{
			if (_quantityDimensionInfo)
				delete _quantityDimensionInfo;
			_quantityDimensionInfo = NULL;
		}

		System::String^ QuantityDimensionInfo::QuantityName::get()
		{
			return CPPToNETConversions::MarshalString(_quantityDimensionInfo->GetQuantityName());
		}

		IDimensionInfo^ QuantityDimensionInfo::DimensionInfo::get()
		{
			FuncParserNative::DimensionInfo nativeDimensionInfo = _quantityDimensionInfo->GetDimensionInfo();

			IDimensionInfo^ dimensionInfo = gcnew OSPSuite::FuncParser::DimensionInfo(
				nativeDimensionInfo.GetLength(),
				nativeDimensionInfo.GetMass(),
				nativeDimensionInfo.GetTime(),
				nativeDimensionInfo.GetElectricCurrent(),
				nativeDimensionInfo.GetTemperature(),
				nativeDimensionInfo.GetAmount(),
				nativeDimensionInfo.GetLuminousIntensity());

			return dimensionInfo;
		}

		//-----------------------------------------------------------------------------
		//---- Managed dimension parser
		//-----------------------------------------------------------------------------
		FuncParserNative::DimensionParser * DimensionParser::GetNativeDimensionParser()
		{
			return _dimensionParser;
		}

		//constructor
		DimensionParser::DimensionParser()
		{
			_dimensionParser = new FuncParserNative::DimensionParser();
		}

		// destructor
		DimensionParser::~DimensionParser()
		{
			if (_dimensionParser)
				delete _dimensionParser;
			_dimensionParser = NULL;
		}

		// finalizer
		DimensionParser::!DimensionParser()
		{
			if (_dimensionParser)
				delete _dimensionParser;
			_dimensionParser = NULL;
		}

		IDimensionInfo^ DimensionParser::GetDimensionInfoFor(System::String^ formula,
			IEnumerable<IQuantityDimensionInfo^>^ quantityDimensions,
			IFuncParserErrorData^ ED)
		{
			IDimensionInfo^ dimensionInfo;

			if (ED == nullptr) // should never happen
				throw gcnew System::Exception("ErrorData argument not set in DimensionParser::GetDimensionInfoFor");

			try
			{
				std::vector<FuncParserNative::QuantityDimensionInfo> quantityDimensionsVec;
				FuncParserNative::FuncParserErrorData fpED;

				NETToCPPConversions::QuantityDimensionInfoEnumerableToVector(quantityDimensions, quantityDimensionsVec);

				FuncParserNative::DimensionInfo nativeDimensionInfo =
					_dimensionParser->GetDimensionInfoFor(NETToCPPConversions::MarshalString(formula),
						quantityDimensionsVec,
						fpED);

				if (fpED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
					throw fpED;

				dimensionInfo = gcnew DimensionInfo(nativeDimensionInfo);
			}
			catch (FuncParserNative::FuncParserErrorData & fpED)
			{
				((FuncParserErrorData^)ED)->SetupFrom(fpED);
			}
			catch (System::Exception^)
			{
				throw;
			}
			catch (...)
			{
				ExceptionHelper::ThrowExceptionFromUnknown();
			}

			return dimensionInfo;
		}
	}
}