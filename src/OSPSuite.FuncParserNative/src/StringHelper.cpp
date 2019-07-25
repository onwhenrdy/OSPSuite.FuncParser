#include "FuncParser/StringHelper.h"
#include <string>

namespace FuncParserNative
{

std::string StringHelper::Capitalize (const std::string & pInString)
{
	char * NewStr = new char[pInString.size()+1];
	const char *csource = pInString.c_str();
    for(size_t i=0; i<pInString.size(); i++)
		NewStr[i] = i==0 ? toupper(csource[i]) : tolower(csource[i]);
	NewStr[pInString.size()] = '\0';
	std::string newString = NewStr;
	delete[] NewStr;
	return newString;
}

}//.. end "namespace FuncParserNative"
