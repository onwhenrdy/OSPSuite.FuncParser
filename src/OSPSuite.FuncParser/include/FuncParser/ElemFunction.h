#ifndef _ElemFunction_H_
#define _ElemFunction_H_

#include <string>
#include "FuncParser/FuncParserTypeDefs.h"
#include "FuncParser/FuncParserErrorData.h"
#include "FuncParser/Math.h"
#include <cmath>

namespace FuncParserNative
{

class FuncNode;

class ElemFunction
{	
	public:
		enum enmElemFunctionType 
		{
			EF_EQUAL = 1,
			EF_GREATER,
			EF_GREATEREQUAL,
			EF_LESS,
			EF_LESSEQUAL,
			EF_UNEQUAL,
			EF_UNEQUAL2,
			EF_NOT,
			EF_AND,
			EF_OR,
			EF_PLUS,
			EF_MINUS,
			EF_MUL,
			EF_DIV,
			EF_POWER,
			EF_SIN,
			EF_COS,
			EF_TAN,
			EF_ASIN,
			EF_ACOS,
			EF_ATAN,
			EF_EXP,
			EF_LN,
			EF_LOG,
			EF_LOG10,
			EF_SINH,
			EF_COSH,
			EF_TANH,
			EF_SQRT,
			EF_MIN,
			EF_MAX,
			EF_IF,
			EF_EQUAL_F,
			EF_GREATER_F,
			EF_GREATEREQUAL_F,
			EF_LESS_F,
			EF_LESSEQUAL_F,
			EF_UNEQUAL_F,
			EF_POWER_F,
			EF_RND,
			EF_SRND
		};
	
	private:
		enmElemFunctionType m_Type;
		std::string m_FuncString;
		bool m_IsLogical;
		bool m_IsUnary;
	
	public:
		ElemFunction (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary);
		virtual ~ElemFunction ();
		const std::string & GetFuncString () const;
		enmElemFunctionType GetType () const;
		bool IsLogical () const;
		bool IsUnary () const;
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const= 0;

		//second operand required only for power-function
		virtual double CalculateDimensionExponent (double firstExponent, 
			                                       double secondExponent,
			                                       double secondOperand, 
												   bool isFirstOperandConst, 
												   bool isSecondOperandConst) const= 0;

		virtual bool IsRandomFunction ();
};

class EF_ACOS : 	
	public ElemFunction
{
	public:
		EF_ACOS ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;

		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                       double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_AND : 	
	public ElemFunction
{
	public:
		EF_AND ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;

		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                       double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_ASIN : 	
	public ElemFunction
{
	public:
		EF_ASIN ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;

		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                       double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_ATAN : 	
	public ElemFunction
{
	public:
		EF_ATAN ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;

		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                       double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_COS : 	
	public ElemFunction
{
	public:
		EF_COS ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;

		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                       double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_COSH : 	
	public ElemFunction
{
	public:
		EF_COSH ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;

		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                       double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_DIV : 	
	public ElemFunction
{
	public:
		EF_DIV ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;

		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                       double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_EQUAL : 	
	public ElemFunction
{
	protected:
		EF_EQUAL (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary);
	
	public:
		EF_EQUAL ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;

		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                       double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_EQUAL_F : 	
	public EF_EQUAL
{
	public:
		EF_EQUAL_F ();
};

class EF_EXP : 	
	public ElemFunction
{
	public:
		EF_EXP ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;

		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
			                                       double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;

};

class EF_GREATER : 	
	public ElemFunction
{
	protected:
		EF_GREATER (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary);
	
	public:
		EF_GREATER ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_GREATER_F : 	
	public EF_GREATER
{
	public:
		EF_GREATER_F ();
};

class EF_GREATEREQUAL : 	
	public ElemFunction
{
	protected:
		EF_GREATEREQUAL (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary);
	
	public:
		EF_GREATEREQUAL ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_GREATEREQUAL_F : 	
	public EF_GREATEREQUAL
{
	public:
		EF_GREATEREQUAL_F ();
};

class EF_IF : 	
	public ElemFunction
{
	public:
		double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		EF_IF ();
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_LESS : 	
	public ElemFunction
{
	protected:
		EF_LESS (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary);
	
	public:
		EF_LESS ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_LESS_F : 	
	public EF_LESS
{
	public:
		EF_LESS_F ();
};

class EF_LESSEQUAL : 	
	public ElemFunction
{
	protected:
		EF_LESSEQUAL (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary);
	
	public:
		EF_LESSEQUAL ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_LESSEQUAL_F : 	
	public EF_LESSEQUAL
{
	public:
		EF_LESSEQUAL_F ();
};

class EF_LN : 	
	public ElemFunction
{
	public:
		EF_LN ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_LOG : 	
	public ElemFunction
{
	public:
		EF_LOG ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_LOG10 : 	
	public ElemFunction
{
	public:
		EF_LOG10 ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_MAX : 	
	public ElemFunction
{
	public:
		EF_MAX ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_MIN : 	
	public ElemFunction
{
	public:
		EF_MIN ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_MINUS : 	
	public ElemFunction
{
	public:
		EF_MINUS ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_MUL : 	
	public ElemFunction
{
	public:
		EF_MUL ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_NOT : 	
	public ElemFunction
{
	public:
		EF_NOT ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_OR : 	
	public ElemFunction
{
	public:
		EF_OR ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_PLUS : 	
	public ElemFunction
{
	public:
		EF_PLUS ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_POWER : 	
	public ElemFunction
{
	protected:
		EF_POWER (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary);
	
	public:
		EF_POWER ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_POWER_F : 	
	public EF_POWER
{
	public:
		EF_POWER_F ();
};

class EF_RND :
	public ElemFunction
{
	protected:
		EF_RND (enmElemFunctionType pType, const std::string & pFuncString);

	public:
		EF_RND ();

		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		bool IsRandomFunction ();

		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
														   double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_SIN : 	
	public ElemFunction
{
	public:
		EF_SIN ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_SINH : 	
	public ElemFunction
{
	public:
		EF_SINH ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_SQRT : 	
	public ElemFunction
{
	public:
		EF_SQRT ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_SRND :
	public EF_RND
{
	public:
		EF_SRND ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;

};

class EF_TAN : 	
	public ElemFunction
{
	public:
		EF_TAN ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_TANH : 	
	public ElemFunction
{
	public:
		EF_TANH ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_UNEQUAL : 	
	public ElemFunction
{
	protected:
		EF_UNEQUAL (enmElemFunctionType pType, const std::string & pFuncString, bool pIsLogical, bool pIsUnary);
	
	public:
		EF_UNEQUAL ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_UNEQUAL2 : 	
	public ElemFunction
{
	public:
		EF_UNEQUAL2 ();
		virtual double Eval (double FirstOperand, double SecondOperand, double ComparisonTolerance) const;
		virtual double CalculateDimensionExponent (double firstExponent, double secondExponent,
	                                               double secondOperand, bool isFirstOperandConst, bool isSecondOperandConst) const;
};

class EF_UNEQUAL_F : 	
	public EF_UNEQUAL
{
	public:
		EF_UNEQUAL_F ();
};


}//.. end "namespace FuncParserNative"


#endif //_ElemFunction_H_

