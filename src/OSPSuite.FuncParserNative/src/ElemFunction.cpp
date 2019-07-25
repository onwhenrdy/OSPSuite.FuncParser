#include "FuncParser/ElemFunction.h"
#include "FuncParser/FuncNode.h"
#include "FuncParser/FuncParserErrorData.h"
#include "FuncParser/FuncParser.h"
#include <time.h>

#ifdef linux
#include <stdlib.h>
#endif

namespace FuncParserNative
{

// --------------------  ELEMFUNCTION -----------------------------------------------
ElemFunction::ElemFunction (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary)
{
	_type = pType;
	_funcString = pFuncString;
	_isLogical = pIsLogical;
	_isUnary = pIsUnary;
}

ElemFunction::~ElemFunction ()
{}

const std::string & ElemFunction::GetFuncString () const
{
    return _funcString;
}

ElemFunction::enmElemFunctionType ElemFunction::GetType () const
{
    return _type;
}

bool ElemFunction::IsLogical () const
{
    return _isLogical;
}

bool ElemFunction::IsUnary () const
{
    return _isUnary;
}

bool ElemFunction::IsRandomFunction ()
{
	return false;
}

// --------------------  ACOS ------------------------------------------------------
EF_ACOS::EF_ACOS (): ElemFunction(ElemFunction::EF_ACOS, "ACOS", false, true)
{}

double EF_ACOS::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return acos(FirstOperand);
}

double EF_ACOS::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (firstExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, "EF_ACOS::CalculateDimensionExponent",
	    	                      "Argument of ACOS-function must be dimensionless");

	return 0; //no dimension
}

// --------------------  AND ------------------------------------------------------
EF_AND::EF_AND (): ElemFunction(ElemFunction::EF_AND, FuncParser::conANDSymbol, true, false)
{}

double EF_AND::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return ( (FirstOperand == 1) && (SecondOperand == 1)) ? 1 : 0;
}

double EF_AND::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if ((firstExponent != 0.0) || (secondExponent != 0.0))
		throw FuncParserErrorData(FuncParserErrorData::err_DIMENSION, "EF_AND::CalculateDimensionExponent",
	    	                      "Arguments of AND-function must be dimensionless");

	return 0; //no dimension
}

// --------------------  ASIN ------------------------------------------------------
EF_ASIN::EF_ASIN () : ElemFunction(ElemFunction::EF_ASIN, "ASIN", false, true)
{}

double EF_ASIN::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return asin(FirstOperand);
}

double EF_ASIN::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (firstExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, "EF_ASIN::CalculateDimensionExponent",
	    	                      "Argument of ASIN-function must be dimensionless");

	return 0; //no dimension
}
// --------------------  ATAN ------------------------------------------------------
EF_ATAN::EF_ATAN () : ElemFunction(ElemFunction::EF_ATAN, "ATAN", false, true)
{}

double EF_ATAN::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return atan(FirstOperand);
}

double EF_ATAN::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (firstExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, "EF_ATAN::CalculateDimensionExponent",
	    	                      "Argument of ATAN-function must be dimensionless");

	return 0; //no dimension
}
// --------------------  COS ------------------------------------------------------
EF_COS::EF_COS () : ElemFunction(ElemFunction::EF_COS, "COS", false, true)
{}

double EF_COS::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return cos(FirstOperand);
}

double EF_COS::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (firstExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, "EF_COS::CalculateDimensionExponent",
	    	                      "Argument of COS-function must be dimensionless");

	return 0; //no dimension
}

// --------------------  COSH ------------------------------------------------------
EF_COSH::EF_COSH () : ElemFunction(ElemFunction::EF_COSH, "COSH", false, true)
{}

double EF_COSH::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return cosh(FirstOperand);
}

double EF_COSH::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (firstExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, "EF_COSH::CalculateDimensionExponent",
	    	                      "Argument of COSH-function must be dimensionless");

	return 0; //no dimension
}

// --------------------  DIV ------------------------------------------------------
EF_DIV::EF_DIV () : ElemFunction(ElemFunction::EF_DIV, "/", false, false)
{}

double EF_DIV::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return FirstOperand / SecondOperand;
}

double EF_DIV::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	return firstExponent - secondExponent; 
}
// --------------------  EQUAL ------------------------------------------------------
EF_EQUAL::EF_EQUAL (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary) 
                  : ElemFunction(pType, pFuncString, pIsLogical, pIsUnary)
{}

EF_EQUAL::EF_EQUAL () : ElemFunction(ElemFunction::EF_EQUAL, "=", true, false)
{}

double EF_EQUAL::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return (fabs(FirstOperand-SecondOperand) <= ComparisonTolerance) ? 1 : 0;
}

double EF_EQUAL::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                 double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (isFirstOperandConst || isSecondOperandConst)
		return 0; //e.g. "P1==2"

	if (firstExponent != secondExponent)
		throw FuncParserErrorData(FuncParserErrorData::err_DIMENSION, "EF_EQUAL::CalculateDimensionExponent",
	    	                      "Arguments of \"==\" function must have the same dimension");

	return 0; //no dimension
}

// --------------------  EQUAL_F ----------------------------------------------------
EF_EQUAL_F::EF_EQUAL_F () : EF_EQUAL(ElemFunction::EF_EQUAL_F, "EQ", true, false)
{}
// --------------------  EXP ------------------------------------------------------
EF_EXP::EF_EXP () : ElemFunction(ElemFunction::EF_EXP, "EXP", false, true)
{}

double EF_EXP::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return exp(FirstOperand);
}

double EF_EXP::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (firstExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, "EF_EXP::CalculateDimensionExponent",
	    	                      "Argument of EXP-function must be dimensionless");

	return 0; //no dimension
}
// --------------------  GREATER ----------------------------------------------------
EF_GREATER::EF_GREATER (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary) 
                      : ElemFunction(pType, pFuncString, pIsLogical, pIsUnary)
{}

EF_GREATER::EF_GREATER () : ElemFunction(ElemFunction::EF_GREATER, ">", true, false)
{}

double EF_GREATER::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return (FirstOperand - SecondOperand > ComparisonTolerance) ? 1 : 0;
}

double EF_GREATER::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                   double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (isFirstOperandConst || isSecondOperandConst)
		return 0; //e.g. "P1>2"

	if (firstExponent != secondExponent)
		throw FuncParserErrorData(FuncParserErrorData::err_DIMENSION, "EF_GREATER::CalculateDimensionExponent",
	    	                      "Arguments of \">\" function must have the same dimension");

	return 0; //no dimension
}
// --------------------  GREATER_F ---------------------------------------------------
EF_GREATER_F::EF_GREATER_F () : EF_GREATER(ElemFunction::EF_GREATER_F, "GT", true, false)
{}
// --------------------  GREATEREQUAL ------------------------------------------------
EF_GREATEREQUAL::EF_GREATEREQUAL (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary) 
                                : ElemFunction(pType, pFuncString, pIsLogical, pIsUnary)
{}

EF_GREATEREQUAL::EF_GREATEREQUAL () : ElemFunction(ElemFunction::EF_GREATEREQUAL, ">=", true, false)
{}

double EF_GREATEREQUAL::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return (FirstOperand - SecondOperand >= -ComparisonTolerance) ? 1 : 0;	
}

double EF_GREATEREQUAL::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                        double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (isFirstOperandConst || isSecondOperandConst)
		return 0; //e.g. "P1>=2"

	if (firstExponent != secondExponent)
		throw FuncParserErrorData(FuncParserErrorData::err_DIMENSION, "EF_GREATEREQUAL::CalculateDimensionExponent",
	    	                      "Arguments of \">=\" function must have the same dimension");

	return 0; //no dimension
}
// --------------------  GREATEREQUAL_F ----------------------------------------------
EF_GREATEREQUAL_F::EF_GREATEREQUAL_F () : EF_GREATEREQUAL(ElemFunction::EF_GREATEREQUAL_F, "GEQ", true, false)
{}
// --------------------  IF ------------------------------------------------------
double EF_IF::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	const char * ERROR_SOURCE = "EF_IF::Eval";
	//should not be called explicitly
	throw FuncParserErrorData(FuncParserErrorData::err_BADCALL, ERROR_SOURCE,
	    	                  "Function may not be called explicitely");
}

EF_IF::EF_IF () : ElemFunction(ElemFunction::EF_IF, "IF", true, false)
{}

double EF_IF::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                              double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	const char * ERROR_SOURCE = "EF_IF::CalculateDimensionExponent";
	//should not be called explicitly
	throw FuncParserErrorData(FuncParserErrorData::err_BADCALL, ERROR_SOURCE,
	    	                  "Function may not be called explicitely");
}
// --------------------  LESS ------------------------------------------------------
EF_LESS::EF_LESS (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary) 
                : ElemFunction(pType, pFuncString, pIsLogical, pIsUnary)
{}

EF_LESS::EF_LESS () : ElemFunction(ElemFunction::EF_LESS, "<", true, false)
{}

double EF_LESS::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return (FirstOperand - SecondOperand < -ComparisonTolerance) ? 1 : 0;
}

double EF_LESS::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (isFirstOperandConst || isSecondOperandConst)
		return 0; //e.g. "P1<2"

	if (firstExponent != secondExponent)
		throw FuncParserErrorData(FuncParserErrorData::err_DIMENSION, "EF_LESS::CalculateDimensionExponent",
	    	                      "Arguments of \"<\" function must have the same dimension");

	return 0; //no dimension
}

// --------------------  LESS_F ------------------------------------------------------
EF_LESS_F::EF_LESS_F () : EF_LESS(ElemFunction::EF_LESS_F, "LT", true, false)
{}
// --------------------  LESSEQUAL --------------------------------------------------
EF_LESSEQUAL::EF_LESSEQUAL (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary) 
                          : ElemFunction(pType, pFuncString, pIsLogical, pIsUnary)
{}

EF_LESSEQUAL::EF_LESSEQUAL () : ElemFunction(ElemFunction::EF_LESSEQUAL, "<=", true, false)
{}

double EF_LESSEQUAL::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return (FirstOperand - SecondOperand <= ComparisonTolerance) ? 1 : 0;
}

double EF_LESSEQUAL::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                     double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (isFirstOperandConst || isSecondOperandConst)
		return 0; //e.g. "P1<=2"

	if (firstExponent != secondExponent)
		throw FuncParserErrorData(FuncParserErrorData::err_DIMENSION, "EF_LESSEQUAL::CalculateDimensionExponent",
	    	                      "Arguments of \"<=\" function must have the same dimension");

	return 0; //no dimension
}

// --------------------  LESSEQUAL_F -------------------------------------------------
EF_LESSEQUAL_F::EF_LESSEQUAL_F () : EF_LESSEQUAL(ElemFunction::EF_LESSEQUAL_F, "LEQ", true, false)
{}
// --------------------  LN ------------------------------------------------------
EF_LN::EF_LN () : ElemFunction(ElemFunction::EF_LN, "LN", false, true)
{}

double EF_LN::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return log(FirstOperand);
}

double EF_LN::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                              double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (firstExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, "EF_LN::CalculateDimensionExponent",
	    	                      "Argument of LN-function must be dimensionless");

	return 0; //no dimension
}
// --------------------  LOG ------------------------------------------------------
EF_LOG::EF_LOG () : ElemFunction(ElemFunction::EF_LOG, "LOG", false, true)
{}

double EF_LOG::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return log(FirstOperand);
}

double EF_LOG::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (firstExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, "EF_LOG::CalculateDimensionExponent",
	    	                      "Argument of LOG-function must be dimensionless");

	return 0; //no dimension
}
// --------------------  LOG10 ------------------------------------------------------
EF_LOG10::EF_LOG10 () : ElemFunction(ElemFunction::EF_LOG10, "LOG10", false, true)
{}

double EF_LOG10::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return log10(FirstOperand);
}

double EF_LOG10::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                 double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (firstExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, "EF_LOG10::CalculateDimensionExponent",
	    	                      "Argument of LOG10-function must be dimensionless");

	return 0; //no dimension
}

// --------------------  MAX ------------------------------------------------------
EF_MAX::EF_MAX () : ElemFunction(ElemFunction::EF_MAX, "MAX", false, false)
{}

double EF_MAX::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return FirstOperand >= SecondOperand ? FirstOperand : SecondOperand;
}

double EF_MAX::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (isFirstOperandConst)
		return secondExponent; //e.g. max(7; P1)

	if (isSecondOperandConst)
		return firstExponent;  //e.g. max(P1; 7)

	if (firstExponent != secondExponent)
		throw FuncParserErrorData(FuncParserErrorData::err_DIMENSION, "EF_MAX::CalculateDimensionExponent",
	    	                      "Arguments of MAX-function must have the same dimension");

	return firstExponent;
}
// --------------------  MIN ------------------------------------------------------
EF_MIN::EF_MIN () : ElemFunction(ElemFunction::EF_MIN, "MIN", false, false)
{}

double EF_MIN::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return FirstOperand <= SecondOperand ? FirstOperand : SecondOperand;
}

double EF_MIN::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (isFirstOperandConst)
		return secondExponent; //e.g. min(7; P1)

	if (isSecondOperandConst)
		return firstExponent;  //e.g. min(P1; 7)

	if (firstExponent != secondExponent)
		throw FuncParserErrorData(FuncParserErrorData::err_DIMENSION, "EF_MIN::CalculateDimensionExponent",
	    	                      "Arguments of MIN-function must have the same dimension");

	return firstExponent;
}
// --------------------  MINUS ------------------------------------------------------
EF_MINUS::EF_MINUS () : ElemFunction(ElemFunction::EF_MINUS, "-", false, false)
{}

double EF_MINUS::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return FirstOperand - SecondOperand;
}

double EF_MINUS::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                 double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (isFirstOperandConst)
		return secondExponent; //e.g. 7-P1

	if (isSecondOperandConst)
		return firstExponent;  //e.g. P1-7

	if (firstExponent != secondExponent)
		throw FuncParserErrorData(FuncParserErrorData::err_DIMENSION, "EF_MINUS::CalculateDimensionExponent",
	    	                      "Arguments of MINUS-function must have the same dimension");

	return firstExponent;
}
// --------------------  MUL ------------------------------------------------------
EF_MUL::EF_MUL () : ElemFunction(ElemFunction::EF_MUL, "*", false, false)
{}

double EF_MUL::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return FirstOperand * SecondOperand;
}

double EF_MUL::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	return firstExponent + secondExponent;
}
// --------------------  NOT ------------------------------------------------------
EF_NOT::EF_NOT () : ElemFunction(ElemFunction::EF_NOT, FuncParser::conNOTSymbol, true, true)
{}

double EF_NOT::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return (FirstOperand == 1) ? 0 : 1;
}

double EF_NOT::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (firstExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_DIMENSION, "EF_NOT::CalculateDimensionExponent",
	    	                      "Argument of NOT-function must be dimensionless");

	return 0; //no dimension
}

// --------------------  OR ------------------------------------------------------
EF_OR::EF_OR () : ElemFunction(ElemFunction::EF_OR, FuncParser::conORSymbol, true, false)
{}

double EF_OR::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return ( (FirstOperand == 1) || (SecondOperand == 1)) ? 1 : 0;
}

double EF_OR::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                              double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if ((firstExponent != 0.0) || (secondExponent != 0.0))
		throw FuncParserErrorData(FuncParserErrorData::err_DIMENSION, "EF_OR::CalculateDimensionExponent",
	    	                      "Arguments of OR-function must be dimensionless");

	return 0; //no dimension
}
// --------------------  PLUS ------------------------------------------------------
EF_PLUS::EF_PLUS () : ElemFunction(ElemFunction::EF_PLUS, "+", false, false)
{}

double EF_PLUS::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return FirstOperand + SecondOperand;
}

double EF_PLUS::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (isFirstOperandConst)
		return secondExponent; //e.g. 7+P1

	if (isSecondOperandConst)
		return firstExponent;  //e.g. P1+7

	if (firstExponent != secondExponent)
		throw FuncParserErrorData(FuncParserErrorData::err_DIMENSION, "EF_PLUS::CalculateDimensionExponent",
	    	                      "Arguments of PLUS-function must have the same dimension");

	return firstExponent;
}
// --------------------  POWER ------------------------------------------------------
EF_POWER::EF_POWER (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary) 
                  : ElemFunction(pType, pFuncString, pIsLogical, pIsUnary)
{}

EF_POWER::EF_POWER () : ElemFunction(ElemFunction::EF_POWER, "^", false, false)
{}

double EF_POWER::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return pow(FirstOperand,SecondOperand);
}

double EF_POWER::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                 double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (secondExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, "EF_POWER::CalculateDimensionExponent",
	    	                      "Exponent of POWER-function must be dimensionless");

	//the only elementar function where second operand is used!
	return firstExponent * secondOperand; 
}

// --------------------  POWER_F -----------------------------------------------------
EF_POWER_F::EF_POWER_F () : EF_POWER(ElemFunction::EF_POWER_F, "POW", false, false)
{
	// nothing to do
}
// --------------------  RND ------------------------------------------------------
EF_RND::EF_RND (): ElemFunction(ElemFunction::EF_RND, "RND", false, true)
{}

EF_RND::EF_RND (enmElemFunctionType pType, const std::string & pFuncString): ElemFunction(pType, pFuncString, false, true)
{}

double EF_RND::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return (double)rand()/(double)RAND_MAX;
}

bool EF_RND::IsRandomFunction ()
{
	return true;
}

double EF_RND::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	return 0; //no dimension
}
// --------------------  SIN ------------------------------------------------------
EF_SIN::EF_SIN () : ElemFunction(ElemFunction::EF_SIN, "SIN", false, true)
{}

double EF_SIN::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return sin(FirstOperand);
}

double EF_SIN::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (firstExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, "EF_SIN::CalculateDimensionExponent",
	    	                      "Argument of SIN-function must be dimensionless");

	return 0; //no dimension
}
// --------------------  SINH ------------------------------------------------------
EF_SINH::EF_SINH () : ElemFunction(ElemFunction::EF_SINH, "SINH", false, true)
{}

double EF_SINH::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return sinh(FirstOperand);
}

double EF_SINH::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (firstExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, "EF_SINH::CalculateDimensionExponent",
	    	                      "Argument of SINH-function must be dimensionless");

	return 0; //no dimension
}

// --------------------  SQRT ------------------------------------------------------
EF_SQRT::EF_SQRT () : ElemFunction(ElemFunction::EF_ACOS, "SQRT", false, true)
{}

double EF_SQRT::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return sqrt(FirstOperand);
}

double EF_SQRT::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	return firstExponent / 2.0; 
}
// --------------------  SRND ------------------------------------------------------
EF_SRND::EF_SRND (): EF_RND(ElemFunction::EF_SRND, "SRND")
{}

double EF_SRND::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	static bool randomStartingPointSet = false;

	if (!randomStartingPointSet)
	{
		srand( (unsigned)time( NULL ) );
		randomStartingPointSet = true;
	}

	return EF_RND::Eval(FirstOperand, SecondOperand, ComparisonTolerance);
}
// --------------------  TAN ------------------------------------------------------
EF_TAN::EF_TAN () : ElemFunction(ElemFunction::EF_TAN, "TAN", false, true)
{}

double EF_TAN::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return tan(FirstOperand);
}

double EF_TAN::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (firstExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, "EF_TAN::CalculateDimensionExponent",
	    	                      "Argument of TAN-function must be dimensionless");

	return 0; //no dimension
}

// --------------------  TANH ------------------------------------------------------
EF_TANH::EF_TANH () : ElemFunction(ElemFunction::EF_TANH, "TANH", false, true)
{}

double EF_TANH::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return tanh(FirstOperand);
}

double EF_TANH::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (firstExponent != 0.0)
		throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, "EF_TANH::CalculateDimensionExponent",
	    	                      "Argument of TANH-function must be dimensionless");

	return 0; //no dimension
}
// --------------------  UNEQUAL -----------------------------------------------------
EF_UNEQUAL::EF_UNEQUAL (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary) 
                      : ElemFunction(pType, pFuncString, pIsLogical, pIsUnary)
{}

EF_UNEQUAL::EF_UNEQUAL () : ElemFunction(ElemFunction::EF_UNEQUAL, "<>", true, false)
{}

double EF_UNEQUAL::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return (fabs(FirstOperand-SecondOperand) > ComparisonTolerance) ? 1 : 0;
}

double EF_UNEQUAL::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                   double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (isFirstOperandConst || isSecondOperandConst)
		return 0; //e.g. "P1!=2"

	if (firstExponent != secondExponent)
		throw FuncParserErrorData(FuncParserErrorData::err_DIMENSION, "EF_UNEQUAL::CalculateDimensionExponent",
	    	                      "Arguments of \"<>\" function must have the same dimension");

	return 0; //no dimension
}

// --------------------  UNEQUAL2 ----------------------------------------------------
EF_UNEQUAL2::EF_UNEQUAL2 () : ElemFunction(ElemFunction::EF_UNEQUAL2, "!=", true, false)
{}

double EF_UNEQUAL2::Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const
{
	return (fabs(FirstOperand-SecondOperand) > ComparisonTolerance) ? 1 : 0;
}

double EF_UNEQUAL2::CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                    double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const
{
	if (isFirstOperandConst || isSecondOperandConst)
		return 0; //e.g. "P1!=2"

	if (firstExponent != secondExponent)
		throw FuncParserErrorData(FuncParserErrorData::err_DIMENSION, "EF_UNEQUAL2::CalculateDimensionExponent",
	    	                      "Arguments of \"<>\" function must have the same dimension");

	return 0; //no dimension
}
// --------------------  UNEQUAL_F ---------------------------------------------------
EF_UNEQUAL_F::EF_UNEQUAL_F () : EF_UNEQUAL(ElemFunction::EF_UNEQUAL_F, "NEQ", true, false)
{}


}//.. end "namespace FuncParserNative"
