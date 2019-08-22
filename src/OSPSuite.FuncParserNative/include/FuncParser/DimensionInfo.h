#ifndef _EF_DIMENSIONINFO_H_
#define _EF_DIMENSIONINFO_H_

#include "FuncParser/ElemFunction.h"

namespace FuncParserNative
{

class DimensionInfo
{
protected:
    double _length;
    double _mass;
    double _time;
    double _electricCurrent;
    double _temperature;
    double _amount;
    double _luminousIntensity;

public:
	DimensionInfo();
	FUNCPARSER_EXPORT DimensionInfo(double length, double mass, double time, 
									double electricCurrent, double temperature, 
									double amount, double luminousIntensity);
	
	FUNCPARSER_EXPORT double GetLength() const ;
	FUNCPARSER_EXPORT double GetMass() const ;
	FUNCPARSER_EXPORT double GetTime() const ;
	FUNCPARSER_EXPORT double GetElectricCurrent() const ;
	FUNCPARSER_EXPORT double GetTemperature() const ;
	FUNCPARSER_EXPORT double GetAmount() const ;
	FUNCPARSER_EXPORT double GetLuminousIntensity() const ;

	FUNCPARSER_EXPORT void SetLength(double length);
	FUNCPARSER_EXPORT void SetMass(double mass);
	FUNCPARSER_EXPORT void SetTime(double time);
	FUNCPARSER_EXPORT void SetElectricCurrent(double electricCurrent);
	FUNCPARSER_EXPORT void SetTemperature(double temperature);
	FUNCPARSER_EXPORT void SetAmount(double amount);
	FUNCPARSER_EXPORT void SetLuminousIntensity(double luminousIntensity);

	void Reset();

	//Perform math operation given by <elemFunction> using
	//current instance (this) as first argument and
	// <dimensionInfo> as a second argument
	// <secondOperand> is only used by power-function
	// 
	void TransformBy(const DimensionInfo & dimensionInfo,
					 ElemFunction * elemFunction,
					 double secondOperand, 
					 bool isFirstOperandConst, 
					 bool isSecondOperandConst);

	bool operator == (const DimensionInfo & dimensionInfo);
	
	//returns true if all exponents are = 0
	bool IsDimensionless(void);
};

}//.. end "namespace FuncParserNative"


#endif //_EF_DIMENSIONINFO_H_

