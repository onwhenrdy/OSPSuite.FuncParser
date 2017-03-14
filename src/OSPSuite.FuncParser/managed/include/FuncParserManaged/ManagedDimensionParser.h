#ifndef _Managed_DimensionParser_H_
#define _Managed_DimensionParser_H_

#include <string>
#include "FuncParserManaged/ManagedFuncParserErrorData.h"
#include "FuncParser/DimensionParser.h"

namespace OSPSuite
{
	namespace FuncParser
	{
		using namespace System::Collections::Generic;

		//-----------------------------------------------------------------------------
		//---- Managed dimension info
		//-----------------------------------------------------------------------------
		public interface class IDimensionInfo
		{
			property double Length
			{
				double get();
				void set(double value);
			}
			property double Mass
			{
				double get();
				void set(double value);
			}
			property double Time
			{
				double get();
				void set(double value);
			}
			property double ElectricCurrent
			{
				double get();
				void set(double value);
			}
			property double Temperature
			{
				double get();
				void set(double value);
			}
			property double Amount
			{
				double get();
				void set(double value);
			}
			property double LuminousIntensity
			{
				double get();
				void set(double value);
			}
		};

		public ref class DimensionInfo : IDimensionInfo
		{
		private:
			FuncParserNative::DimensionInfo * _dimensionInfo;
		internal:
			FuncParserNative::DimensionInfo * GetNativeDimensionInfo();
			DimensionInfo(FuncParserNative::DimensionInfo * dimensionInfo);
			DimensionInfo(const FuncParserNative::DimensionInfo & dimensionInfo);
		public:

			//constructors
			DimensionInfo();
			DimensionInfo(double length, double mass, double time,
				double electricCurrent, double temperature,
				double amount, double luminousIntensity);

			// destructor
			~DimensionInfo();

			// finalizer
			!DimensionInfo();

			property double Length
			{
				virtual double get();
				virtual void set(double value);
			}
			property double Mass
			{
				virtual double get();
				virtual void set(double value);
			}
			property double Time
			{
				virtual double get();
				virtual void set(double value);
			}
			property double ElectricCurrent
			{
				virtual double get();
				virtual void set(double value);
			}
			property double Temperature
			{
				virtual double get();
				virtual void set(double value);
			}
			property double Amount
			{
				virtual double get();
				virtual void set(double value);
			}
			property double LuminousIntensity
			{
				virtual double get();
				virtual void set(double value);
			}

		};

		//-----------------------------------------------------------------------------
		//---- Managed quantity dimension info
		//-----------------------------------------------------------------------------
		public interface class IQuantityDimensionInfo
		{
			property System::String^ QuantityName
			{
				System::String^ get();
				//void set(System::String^ value);
			}

			property IDimensionInfo^ DimensionInfo
			{
				IDimensionInfo^ get();
				//void set(IDimensionInfo^ value);
			}

		};

		public ref class QuantityDimensionInfo : IQuantityDimensionInfo
		{
		private:
			FuncParserNative::QuantityDimensionInfo * _quantityDimensionInfo;
		internal:
			FuncParserNative::QuantityDimensionInfo * GetNativeQuantityDimensionInfo();
		public:

			//constructors
			QuantityDimensionInfo(System::String^ quantityName,
				IDimensionInfo^ dimensionInfo);

			// destructor
			~QuantityDimensionInfo();

			// finalizer
			!QuantityDimensionInfo();

			property System::String^ QuantityName
			{
				virtual System::String^ get();
			}

			property IDimensionInfo^ DimensionInfo
			{
				virtual IDimensionInfo^ get();
			}
		};

		//-----------------------------------------------------------------------------
		//---- Managed dimension parser
		//-----------------------------------------------------------------------------
		public interface class IDimensionParser
		{
			IDimensionInfo^ GetDimensionInfoFor(System::String^ formula,
				IEnumerable<IQuantityDimensionInfo^>^ quantityDimensions,
				IFuncParserErrorData^ ED);
		};


		public ref class DimensionParser : IDimensionParser
		{
		private:
			FuncParserNative::DimensionParser * _dimensionParser;
		internal:
			FuncParserNative::DimensionParser * GetNativeDimensionParser();
		public:

			//constructor
			DimensionParser();

			// destructor
			~DimensionParser();

			// finalizer
			!DimensionParser();

			virtual IDimensionInfo^ GetDimensionInfoFor(System::String^ formula,
				IEnumerable<IQuantityDimensionInfo^>^ quantityDimensions,
				IFuncParserErrorData^ ED);

		};

	}//.. end "namespace FuncParser"
}//.. end "namespace OSPSuite"


#endif //_Managed_DimensionParser_H_

