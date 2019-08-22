#ifndef _Constants_H_
#define _Constants_H_

#ifdef _WINDOWS
// Disable warning about truncation of type name to 255 characters
#pragma warning(disable:4786)
#endif

#include <map>
#include <string>

namespace FuncParserNative
{

class Constant;

class Constants
{
private:
    std::map<std::string, Constant *> _elemConstants;

public:
    Constants();
    ~Constants();
    const Constant *operator[](const std::string &Key) const;
    bool Exists(const std::string &Key) const;
    int GetCount() const;
    std::string GetList() const;
};

}//.. end "namespace FuncParserNative"


#endif //_Constants_H_

