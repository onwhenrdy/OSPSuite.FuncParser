#ifndef _Managed_FuncParser_H_
#define _Managed_FuncParser_H_

#include <string>
#include "FuncParserManaged/ManagedFuncParserErrorData.h"
#include "FuncParser/FuncParser.h"

namespace OSPSuite
{
	namespace FuncParser
	{
		public interface class IFuncParser
		{
			bool IsValidVariableOrParameterName(System::String^ name, IFuncParserErrorData^ ED);
		};


		public ref class FuncParser : IFuncParser
		{
		private:
			FuncParserNative::FuncParser * _funcParser;
		internal:
		public:

			//constructor
			FuncParser();

			// destructor
			~FuncParser();

			// finalizer
			!FuncParser();

			//check if given name is valid
			virtual bool IsValidVariableOrParameterName(System::String^ name, IFuncParserErrorData^ ED);
		};

	}//.. end "namespace FuncParser"
}//.. end "namespace OSPSuite"


#endif //_Managed_FuncParser_H_

