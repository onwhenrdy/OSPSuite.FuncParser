#ifdef _WINDOWS_PRODUCTION
#pragma managed(push,off)
#endif

#include "FuncParser/DimensionInfo.h"
#include "FuncParser/ElemFunction.h"
#include <assert.h>

#ifdef _WINDOWS_PRODUCTION
#pragma managed(pop)
#endif

namespace FuncParserNative
{
	DimensionInfo::DimensionInfo()
	{
		Reset();
	}

	DimensionInfo::DimensionInfo(double length, double mass, double time, 
  							     double electricCurrent, double temperature, 
								 double amount, double luminousIntensity)
	{
		_length = length;
		_mass = mass;
		_time = time;
		_electricCurrent = electricCurrent;
		_temperature = temperature;
		_amount = amount;
		_luminousIntensity = luminousIntensity;
	}
	
	//set all exponents=0 (unitless dimension)
	void DimensionInfo::Reset()
	{
		_length = 0.0;
		_mass = 0.0;
		_time = 0.0;
		_electricCurrent = 0.0;
		_temperature = 0.0;
		_amount = 0.0;
		_luminousIntensity = 0.0;
	}

	double DimensionInfo::GetLength() const
	{
		return _length;
	}
	double DimensionInfo::GetMass() const
	{
		return _mass;
	}
	double DimensionInfo::GetTime() const
	{
		return _time;
	}
	double DimensionInfo::GetElectricCurrent() const
	{
		return _electricCurrent;
	}
	double DimensionInfo::GetTemperature() const
	{
		return _temperature;
	}
	double DimensionInfo::GetAmount() const
	{
		return _amount;
	}
	double DimensionInfo::GetLuminousIntensity() const
	{
		return _luminousIntensity;
	}

	void DimensionInfo::SetLength(double length)
	{
		_length = length;
	}
	void DimensionInfo::SetMass(double mass)
	{
		_mass = mass;
	}
	void DimensionInfo::SetTime(double time)
	{
		_time = time;
	}
	void DimensionInfo::SetElectricCurrent(double electricCurrent)
	{
		_electricCurrent = electricCurrent;
	}
	void DimensionInfo::SetTemperature(double temperature)
	{
		_temperature = temperature;
	}
	void DimensionInfo::SetAmount(double amount)
	{
		_amount = amount;
	}
	void DimensionInfo::SetLuminousIntensity(double luminousIntensity)
	{
		_luminousIntensity = luminousIntensity;
	}

	void DimensionInfo::TransformBy(const DimensionInfo & dimensionInfo,
						            ElemFunction * elemFunction,
									double secondOperand, 
									bool isFirstOperandConst, bool isSecondOperandConst)
	{
		_length = elemFunction->CalculateDimensionExponent(_length, 
			dimensionInfo.GetLength(), secondOperand, isFirstOperandConst, isSecondOperandConst);

		_mass = elemFunction->CalculateDimensionExponent(_mass, 
			dimensionInfo.GetMass(), secondOperand, isFirstOperandConst, isSecondOperandConst);

		_time = elemFunction->CalculateDimensionExponent(_time, 
			dimensionInfo.GetTime(), secondOperand, isFirstOperandConst, isSecondOperandConst);

		_electricCurrent = elemFunction->CalculateDimensionExponent(_electricCurrent, 
			dimensionInfo.GetElectricCurrent(), secondOperand, isFirstOperandConst, isSecondOperandConst);

		_temperature = elemFunction->CalculateDimensionExponent(_temperature, 
			dimensionInfo.GetTemperature(), secondOperand, isFirstOperandConst, isSecondOperandConst);

		_amount = elemFunction->CalculateDimensionExponent(_amount, 
			dimensionInfo.GetAmount(), secondOperand, isFirstOperandConst, isSecondOperandConst);

		_luminousIntensity = elemFunction->CalculateDimensionExponent(_luminousIntensity, 
			dimensionInfo.GetLuminousIntensity(), secondOperand, isFirstOperandConst, isSecondOperandConst);
	}

bool DimensionInfo::IsDimensionless(void)
{
	return 	(_length==0) && 
            (_mass==0) && 
            (_time==0) && 
            (_electricCurrent==0) && 
            (_temperature==0) && 
            (_amount==0) && 
            (_luminousIntensity==0);
}

bool DimensionInfo::operator == (const DimensionInfo & dimensionInfo)
{
	return (_length == dimensionInfo.GetLength()) && 
		   (_mass == dimensionInfo.GetMass()) && 
		   (_time == dimensionInfo.GetTime()) && 
		   (_electricCurrent == dimensionInfo.GetElectricCurrent()) && 
		   (_temperature == dimensionInfo.GetTemperature()) && 
		   (_amount == dimensionInfo.GetAmount()) && 
		   (_luminousIntensity == dimensionInfo.GetLuminousIntensity()) ;
}

}//.. end "namespace FuncParserNative"
