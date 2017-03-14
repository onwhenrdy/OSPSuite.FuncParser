#ifndef _StringHelper_H_
#define _StringHelper_H_

#ifdef WIN32
// Disable warning about truncation of type name to 255 characters
#pragma warning(disable:4786)
#endif

#include <string>

namespace FuncParserNative
{

class StringHelper
{	
	public:
		static std::string Capitalize (const std::string & pInString);
};

}//.. end "namespace FuncParserNative"


#endif //_StringHelper_H_

