#include "FuncParserManaged/ManagedFuncParserErrorData.h"

namespace OSPSuite
{
	namespace FuncParser
	{
		FuncParserErrorData::FuncParserErrorData()
		{
			_errorNumber = errNumber::err_OK;
			_source = gcnew System::String("");
			_description = gcnew System::String("");
		}

		errNumber FuncParserErrorData::ErrorNumber::get()
		{
			return _errorNumber;
		}

		System::String^ FuncParserErrorData::Source::get()
		{
			return _source;
		}

		System::String^ FuncParserErrorData::Description::get()
		{
			return _description;
		}

		void FuncParserErrorData::SetupFrom(const FuncParserNative::FuncParserErrorData & ED)
		{
			switch (ED.GetNumber())
			{
			case FuncParserNative::FuncParserErrorData::err_OK:
				_errorNumber = errNumber::err_OK;
				break;
			case FuncParserNative::FuncParserErrorData::err_RUNTIME:
				_errorNumber = errNumber::err_RUNTIME;
				break;
			case FuncParserNative::FuncParserErrorData::err_PARSE:
				_errorNumber = errNumber::err_PARSE;
				break;
			case FuncParserNative::FuncParserErrorData::err_BADARG:
				_errorNumber = errNumber::err_BADARG;
				break;
			case FuncParserNative::FuncParserErrorData::err_BADCALL:
				_errorNumber = errNumber::err_BADCALL;
				break;
			case FuncParserNative::FuncParserErrorData::err_NUMERIC:
				_errorNumber = errNumber::err_NUMERIC;
				break;
			case FuncParserNative::FuncParserErrorData::err_DIMENSION:
				_errorNumber = errNumber::err_DIMENSION;
				break;
			case FuncParserNative::FuncParserErrorData::err_CANNOTCALC_DIMENSION:
				_errorNumber = errNumber::err_CANNOTCALC_DIMENSION;
				break;
			default:
				throw gcnew System::Exception(gcnew System::String("Invalid FuncParser error number detected"));
			}
			_source = gcnew  System::String(ED.GetSource().c_str());
			_description = gcnew  System::String(ED.GetDescription().c_str());
		}
	}
}