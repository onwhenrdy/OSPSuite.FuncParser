#ifdef WIN32_PRODUCTION
#pragma managed(push,off)
#endif

#include "FuncParser/Constants.h"
#include "FuncParser/FuncParserErrorData.h"
#include "FuncParser/Constant.h"
#include <vector>

#ifdef WIN32
#include <math.h>
#endif

#ifdef WIN32_PRODUCTION
#pragma managed(pop)
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
		m_ElemConstants[pConst->GetName()] = pConst;
	}
}

Constants::~Constants ()
{
	try
	{
		for(ElemConstIterator iter = m_ElemConstants.begin(); iter != m_ElemConstants.end(); iter++)
		{
			delete iter->second;
		}
		m_ElemConstants.clear();
	}
	catch(...)
	{}

}

const Constant * Constants::operator [ ] (const std::string & Key) const
{
	ElemConstIteratorConst iter = m_ElemConstants.find(Key);

	return iter!=m_ElemConstants.end() ? iter->second : NULL;
}

bool Constants::Exists (const std::string & Key)
{
	return (m_ElemConstants.find(Key) != m_ElemConstants.end());
}

int Constants::GetCount ()
{
	return m_ElemConstants.size();
}

const std::string Constants::GetList () const
{
	std::string ConstList="";
	
	for(ElemConstIteratorConst iter=m_ElemConstants.begin(); iter!=m_ElemConstants.end(); iter++)
	{
		Constant * ElemConst=iter->second;
		ConstList = ConstList + ElemConst->GetName() +" ";
	}
		
	return ConstList;
}


}//.. end "namespace FuncParserNative"
