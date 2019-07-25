#include "FuncParser/Constants.h"
#include "FuncParser/FuncParserErrorData.h"
#include "FuncParser/Constant.h"
#include <vector>

#ifdef _WINDOWS
#include <math.h>
#endif

namespace FuncParserNative
{

Constants::Constants ()
{
	std::vector <Constant *> tmpConstants;

	tmpConstants.push_back(new Constant("PI", 2*asin(1.0))); 
	tmpConstants.push_back(new Constant("E", exp(1.0)));
	
	for (std::vector <Constant *>::iterator iter = tmpConstants.begin(); iter != tmpConstants.end(); iter++)
	{
		Constant * pConst = *iter;
		_elemConstants[pConst->GetName()] = pConst;
	}
}

Constants::~Constants ()
{
	try
	{
		for(ElemConstIterator iter = _elemConstants.begin(); iter != _elemConstants.end(); iter++)
		{
			delete iter->second;
		}
		_elemConstants.clear();
	}
	catch(...)
	{}

}

const Constant * Constants::operator [ ] (const std::string & Key) const
{
	ElemConstIteratorConst iter = _elemConstants.find(Key);

	return iter!=_elemConstants.end() ? iter->second : NULL;
}

bool Constants::Exists (const std::string & Key)
{
	return (_elemConstants.find(Key) != _elemConstants.end());
}

int Constants::GetCount ()
{
	return (unsigned int)_elemConstants.size();
}

const std::string Constants::GetList () const
{
	std::string ConstList="";
	
	for(ElemConstIteratorConst iter=_elemConstants.begin(); iter!=_elemConstants.end(); iter++)
	{
		Constant * ElemConst=iter->second;
		ConstList = ConstList + ElemConst->GetName() +" ";
	}
		
	return ConstList;
}


}//.. end "namespace FuncParserNative"
