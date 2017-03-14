#include "FuncParserManaged/ExceptionHelper.h"

using namespace std;

namespace OSPSuite
{
	namespace FuncParser
	{
		void ExceptionHelper::ThrowExceptionFrom(FuncParserNative::FuncParserErrorData & ED)
		{
			string message = "C++ Exception in " + ED.GetSource() + ":\n" + ED.GetDescription();
			throw gcnew System::Exception(gcnew System::String(message.c_str()));
		}

		void ExceptionHelper::ThrowExceptionFromUnknown(string source)
		{
			string message = "C++ unknown Exception";
			if (source != "")
				message += " in" + source;

			ExceptionHelper::ThrowExceptionFrom(message);
		}

		void ExceptionHelper::ThrowExceptionFromUnknown()
		{
			ExceptionHelper::ThrowExceptionFromUnknown("");
		}

		void ExceptionHelper::ThrowExceptionFrom(const string message)
		{
			throw gcnew System::Exception(gcnew System::String(message.c_str()));
		}
	}
}