#ifndef _Constant_H_
#define _Constant_H_

#include <string>

namespace FuncParserNative
{

class Constant
{	
	private:
        std::string _name;
        double _value;

    public:
        Constant(std::string Name, double Value);
        const std::string &GetName() const;
        double GetValue() const;
};

}//.. end "namespace FuncParserNative"


#endif //_Constant_H_

