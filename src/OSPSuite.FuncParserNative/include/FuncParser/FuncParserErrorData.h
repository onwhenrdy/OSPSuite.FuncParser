#ifndef _FuncParserErrorData_H_
#define _FuncParserErrorData_H_

#include "FuncParser/FuncParserTypeDefs.h"

#include <string>

namespace FuncParserNative
{

class FuncParserErrorData
{
public:
    enum errNumber
    {
        err_OK,
        err_RUNTIME,
        err_PARSE,
        err_BADARG,
        err_BADCALL,
        err_NUMERIC,
        err_DIMENSION,
        err_CANNOTCALC_DIMENSION,
        err_ERROR
    };

private:
    std::string _source;
    std::string _description;
    errNumber _number;

public:
    FUNCPARSER_EXPORT FuncParserErrorData();
    FUNCPARSER_EXPORT FuncParserErrorData(errNumber Number,
                                          std::string Source,
                                          std::string Description);
    FUNCPARSER_EXPORT errNumber GetNumber() const;
    FUNCPARSER_EXPORT void SetNumber(errNumber number);
    FUNCPARSER_EXPORT const std::string &GetSource() const;
    FUNCPARSER_EXPORT void SetSource(std::string source);
    FUNCPARSER_EXPORT const std::string &GetDescription() const;
    FUNCPARSER_EXPORT void SetDescription(std::string description);
    FUNCPARSER_EXPORT void SetError(errNumber Number, std::string Source, std::string Description);
    FUNCPARSER_EXPORT void Clear();
};

}//.. end "namespace FuncParserNative"


#endif //_FuncParserErrorData_H_

