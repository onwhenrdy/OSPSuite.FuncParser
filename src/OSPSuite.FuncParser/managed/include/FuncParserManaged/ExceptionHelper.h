#ifndef _ExceptionHelper_H_
#define _ExceptionHelper_H_

#include "FuncParser/FuncParserErrorData.h"

namespace OSPSuite
{
	namespace FuncParser
	{
		ref class ExceptionHelper
		{
		public:
			static void ThrowExceptionFrom(FuncParserNative::FuncParserErrorData & ED);
			static void ThrowExceptionFrom(const std::string message);
			static void ThrowExceptionFromUnknown(std::string source);
			static void ThrowExceptionFromUnknown();
		};
	}
}

#endif //_ExceptionHelper_H_

