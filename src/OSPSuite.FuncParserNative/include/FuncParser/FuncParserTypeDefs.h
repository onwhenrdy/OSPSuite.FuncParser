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
using StringVector = std::vector<std::string>;
using StringVectorIterator = std::vector<std::string>::iterator;
using DoubleVector = std::vector<double>;
using IndexVector = std::vector<size_t>;

} // namespace FuncParserNative

#endif //_FuncParserTypeDefs_H_
