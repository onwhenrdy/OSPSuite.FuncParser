#include "FuncParser/ElemFunctions.h"
#include "FuncParser/ElemFunction.h"
#include "FuncParser/FuncParserErrorData.h"

#include <vector>
#include <assert.h>

#ifdef _WINDOWS
#pragma warning( disable : 4996)
#endif

#ifdef linux
#include <cstring>
#endif

namespace FuncParserNative
{
ElemFunctions *ElemFunctions::_elemFunctionsInstance;
int ElemFunctions::_numberOfElemFunctions;
ElemFunction ** ElemFunctions::_elemFunctions;
char ** ElemFunctions::_elemFunctionKeys;
ElemFunction::enmElemFunctionType * ElemFunctions::_elemFunctionTypes;

ElemFunctions * ElemFunctions::GetInstance ()
{
    if (_elemFunctionsInstance)
        return _elemFunctionsInstance; //already created

    _elemFunctionsInstance = new ElemFunctions();

    std::vector<ElemFunction *> tmpElemFunctions;
    tmpElemFunctions.reserve(41);

    tmpElemFunctions.push_back(new EF_ACOS());
    tmpElemFunctions.push_back(new EF_AND());
    tmpElemFunctions.push_back(new EF_ASIN());
    tmpElemFunctions.push_back(new EF_ATAN());
    tmpElemFunctions.push_back(new EF_COS());
    tmpElemFunctions.push_back(new EF_COSH());
    tmpElemFunctions.push_back(new EF_DIV());
    tmpElemFunctions.push_back(new EF_EQUAL());
    tmpElemFunctions.push_back(new EF_EXP());
    tmpElemFunctions.push_back(new EF_GREATER());
    tmpElemFunctions.push_back(new EF_GREATEREQUAL());
    tmpElemFunctions.push_back(new EF_LESS());
    tmpElemFunctions.push_back(new EF_LESSEQUAL());
    tmpElemFunctions.push_back(new EF_LN());
    tmpElemFunctions.push_back(new EF_LOG());
    tmpElemFunctions.push_back(new EF_LOG10());
	tmpElemFunctions.push_back(new EF_MINUS());
	tmpElemFunctions.push_back(new EF_MUL());
	tmpElemFunctions.push_back(new EF_NOT());
	tmpElemFunctions.push_back(new EF_OR());
	tmpElemFunctions.push_back(new EF_PLUS());
	tmpElemFunctions.push_back(new EF_POWER());
	tmpElemFunctions.push_back(new EF_SIN());
	tmpElemFunctions.push_back(new EF_SINH());
	tmpElemFunctions.push_back(new EF_SQRT());
	tmpElemFunctions.push_back(new EF_TAN());
	tmpElemFunctions.push_back(new EF_TANH());
    tmpElemFunctions.push_back(new EF_UNEQUAL());
    tmpElemFunctions.push_back(new EF_UNEQUAL2());
    tmpElemFunctions.push_back(new EF_MIN());
    tmpElemFunctions.push_back(new EF_MAX());
    tmpElemFunctions.push_back(new EF_IF());

    tmpElemFunctions.push_back(new EF_EQUAL_F());
    tmpElemFunctions.push_back(new EF_GREATER_F());
    tmpElemFunctions.push_back(new EF_GREATEREQUAL_F());
    tmpElemFunctions.push_back(new EF_LESS_F());
    tmpElemFunctions.push_back(new EF_LESSEQUAL_F());
    tmpElemFunctions.push_back(new EF_UNEQUAL_F());
    tmpElemFunctions.push_back(new EF_POWER_F());

    tmpElemFunctions.push_back(new EF_RND());
    tmpElemFunctions.push_back(new EF_SRND());

    _numberOfElemFunctions = (unsigned int) tmpElemFunctions.size();

    _elemFunctions = new ElemFunction *[_numberOfElemFunctions];
    _elemFunctionKeys = new char *[_numberOfElemFunctions];
    _elemFunctionTypes = new ElemFunction::enmElemFunctionType[_numberOfElemFunctions];

    int i = 0;
    for (auto pElemFunc : tmpElemFunctions)
    {
        const auto &key = pElemFunc->GetFuncString();

        _elemFunctions[i] = pElemFunc;
        _elemFunctionKeys[i] = new char[key.size() + 1];
        strcpy(_elemFunctionKeys[i], key.c_str());
        _elemFunctionTypes[i] = pElemFunc->GetType();
        ++i;
    }

    return _elemFunctionsInstance;
}

ElemFunctions::ElemFunctions ()
{
    _elemFunctionsInstance = nullptr;
}

ElemFunctions::~ElemFunctions ()
{
    try
    {
        delete[] _elemFunctions;
        _elemFunctions = nullptr;

        delete[] _elemFunctionKeys;
        _elemFunctionKeys = nullptr;

        delete[] _elemFunctionTypes;
        _elemFunctionTypes = nullptr;

        if (_elemFunctionsInstance)
        {
            delete _elemFunctionsInstance;
            _elemFunctionsInstance = nullptr;
        }

    } catch (...)
    {}
}

ElemFunction * ElemFunctions::operator [ ] (ElemFunction::enmElemFunctionType FuncType) const
{
	for(int i=0; i<_numberOfElemFunctions; i++)
	{
        if (_elemFunctionTypes[i] == FuncType)
            return _elemFunctions[i];
    }

    //not found
    return nullptr;
}

ElemFunction * ElemFunctions::operator [ ] (const std::string & Key) const
{
	const char * key = Key.c_str();

	for(int i=0; i<_numberOfElemFunctions; i++)
	{
        if (!strcmp(_elemFunctionKeys[i], key))
            return _elemFunctions[i];
    }

    return nullptr;
}

bool ElemFunctions::Exists (ElemFunction::enmElemFunctionType FuncType)
{
	for(int i=0; i<_numberOfElemFunctions; i++)
	{
		if (_elemFunctionTypes[i] == FuncType)
			return true;
	}

	return false;
}

const bool ElemFunctions::Exists (const std::string & Key)
{
	const char * key = Key.c_str();

	for(int i=0; i<_numberOfElemFunctions; i++)
	{
		if (!strcmp(_elemFunctionKeys[i], key))
			return true;
	}

	return false;
}

const int ElemFunctions::GetCount ()
{
	return _numberOfElemFunctions;
}

std::string ElemFunctions::GetList(bool pLogical /*= false*/, bool bBinary /*= false*/) const
{
    std::string FuncList;
    // approximation to avoid too much allocation overhead
    FuncList.reserve(_numberOfElemFunctions * 7);

    for (int i = 0; i < _numberOfElemFunctions; i++)
    {
        ElemFunction * ElemFunc = _elemFunctions[i];

		if ((!ElemFunc->IsLogical() || pLogical) && 
		    ( ElemFunc->IsUnary()   || bBinary))
		FuncList = FuncList + ElemFunc->GetFuncString() +" ";
    }

    return FuncList;
}

} // namespace FuncParserNative
