#include "FuncParserManaged/Conversions.h"
#include "FuncParserManaged/ManagedFuncParser.h"
#include "FuncParserManaged/ExceptionHelper.h"

namespace OSPSuite
{
	namespace FuncParser
	{
		//constructor
		FuncParser::FuncParser()
		{
			_funcParser = new FuncParserNative::FuncParser();
		}

		// destructor
		FuncParser::~FuncParser()
		{
			if (_funcParser)
				delete _funcParser;
			_funcParser = NULL;
		}

		// finalizer
		FuncParser::!FuncParser()
		{
			if (_funcParser)
				delete _funcParser;
			_funcParser = NULL;
		}

		bool FuncParser::IsValidVariableOrParameterName(System::String^ name, IFuncParserErrorData^ ED)
		{
			if (ED == nullptr)
				throw gcnew System::Exception("ErrorData argument not set in FuncParser::Parse");

			std::vector<std::string> varNames, paramNames;
			varNames.push_back(NETToCPPConversions::MarshalString(name));

			FuncParserNative::FuncParserErrorData fpED;

			_funcParser->CheckVarParamNames(varNames, paramNames, fpED);

			((FuncParserErrorData^)ED)->SetupFrom(fpED);

			return (fpED.GetNumber() == FuncParserNative::FuncParserErrorData::err_OK);
		}
	}
}