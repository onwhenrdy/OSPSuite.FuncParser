#ifndef _Managed_FuncParserErrorData_H_
#define _Managed_FuncParserErrorData_H_

#include <string>
#include "FuncParser/FuncParserErrorData.h"

namespace OSPSuite
{
	namespace FuncParser
	{
		public enum class errNumber
		{
			err_OK,
			err_RUNTIME,
			err_PARSE,
			err_BADARG,
			err_BADCALL,
			err_NUMERIC,
			err_DIMENSION,
			err_CANNOTCALC_DIMENSION
		};

		public interface class IFuncParserErrorData
		{
			property errNumber ErrorNumber
			{
				errNumber get();
			}

			property System::String^ Source
			{
				System::String^ get();
			}

			property System::String^ Description
			{
				System::String^ get();
			}
		};

		public ref class FuncParserErrorData : public IFuncParserErrorData
		{
		private:
			errNumber       _errorNumber;
			System::String^ _source;
			System::String^ _description;
		internal:
			void SetupFrom(const FuncParserNative::FuncParserErrorData & ED);
		public:
			FuncParserErrorData();

			property errNumber ErrorNumber
			{
				virtual errNumber get();
			}

			property System::String^ Source
			{
				virtual System::String^ get();
			}

			property System::String^ Description
			{
				virtual System::String^ get();
			}
		};

	}//.. end "namespace FuncParserNET"
}//.. end "namespace OSPSuite"


#endif //_Managed_FuncParserErrorData_H_

