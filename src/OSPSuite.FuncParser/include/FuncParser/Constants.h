#ifndef _Constants_H_
#define _Constants_H_

#ifdef WIN32
// Disable warning about truncation of type name to 255 characters
#pragma warning(disable:4786)
#endif

#include <map>
#include <string>
#include "FuncParser/Constant.h"
#ifdef WIN32
#include <ymath.h>

#endif
#ifdef linux
#include <cmath>
#endif

#include "FuncParser/FuncParserTypeDefs.h"

namespace FuncParserNative
{

class Constant;

class Constants
{	
	private:
		typedef std::map < std::string , Constant * >::iterator ElemConstIterator ;
		typedef std::map < std::string , Constant * >::const_iterator ElemConstIteratorConst ;
		std::map < std::string , Constant * > m_ElemConstants;
	
	public:
		Constants ();
		~Constants ();
		const Constant * operator [ ] (const std::string & Key) const;
		bool Exists (const std::string & Key);
		int GetCount ();
		const std::string GetList () const;
};

}//.. end "namespace FuncParserNative"


#endif //_Constants_H_

