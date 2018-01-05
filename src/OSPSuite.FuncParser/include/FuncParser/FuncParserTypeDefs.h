#ifndef _FuncParserTypeDefs_H_
#define _FuncParserTypeDefs_H_

#ifdef _WINDOWS
// Disable warning about truncation of type name to 255 characters
#pragma warning(disable:4786)
#endif

#include <vector>
#include <string>

#ifdef _WINDOWS
#define FUNCPARSER_EXPORT __declspec(dllexport)
#endif
#ifdef linux
#define FUNCPARSER_EXPORT 
#endif


namespace FuncParserNative
{

typedef std::vector < std::string > StringVector;

typedef std::vector < std::string >::iterator StringVectorIterator;

typedef std::vector < double > DoubleVector;

}//.. end "namespace FuncParserNative"


#endif //_FuncParserTypeDefs_H_

