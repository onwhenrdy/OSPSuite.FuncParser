#ifdef _WINDOWS
#pragma warning( disable : 4691)
#endif

#include "FuncParserManaged/Conversions.h"
#include "FuncParserManaged/ManagedParsedFunction.h"
#include "FuncParserManaged/ExceptionHelper.h"
#include "FuncParserManaged/ManagedFuncParserErrorData.h"
#include "FuncParser/Math.h"

#include <math.h>

namespace UnitTests
{
    using namespace OSPSuite::BDDHelper;
    using namespace OSPSuite::BDDHelper::Extensions;
    using namespace NUnit::Framework;
	using namespace System::Collections::Generic;
	using namespace System;

	public ref class concern_for_parsed_function abstract : ContextSpecification<OSPSuite::FuncParser::ParsedFunction^>
    {
    public:
		virtual void GlobalContext() override
		{}

	protected:
		double x, y, z, p1, p2;

        virtual void Context() override
        {
            sut=gcnew OSPSuite::FuncParser::ParsedFunction();
			
			x=1.0; y=2.0; z=3.0; p1=4.0; p2=5.0;

			sut->CaseSensitive::set(true);
			sut->LogicOperatorsAllowed::set(true);
			sut->LogicalNumericMixAllowed::set(false);
			sut->SimplifyParametersAllowed::set(true);

			IList<String^>^ varNames=gcnew System::Collections::Generic::List<String^>();
			varNames->Add("x");
			varNames->Add("y");
			varNames->Add("z");
			sut->VariableNames::set(varNames);

			IList<String^>^ paramNames=gcnew System::Collections::Generic::List<String^>();
			paramNames->Add("p1");
			paramNames->Add("p2");
			sut->ParameterNames::set(paramNames);

			array<double>^ paramValues = gcnew array<double>{p1,p2};
			sut->ParameterValues::set(paramValues);

        }
    };

	//---- Base class for most of test classes in this file
	//Every test defines math. expression to parse and calculates
	//its value directly (via C++ code)
	// After that, 3 ParsedFunction are defined: 
	//  1: SimplifyParametersAllowed = true (which is actually sut)
	//  2: SimplifyParametersAllowed = false
	//  3: SimplifyParametersAllowed = true but some parameters set to "not to be simplified"
	//(besides that, Parsed Functions are equal)
	// For every function, value is calculated via FuncParser and compared to
	// directly calculated value (within given math. rel. tolerance)
	public ref class when_calculating_expressions abstract : public concern_for_parsed_function
    {
	private:
		OSPSuite::FuncParser::ParsedFunction^ _notSimplifiedParsedFunction;
		OSPSuite::FuncParser::ParsedFunction^ _partlySimplifiedParsedFunction;
		double _calcedValueSimplified;       //value calced via FuncParser (all parameters simplified)
		double _calcedValueNotSimplified;    //value calced via FuncParser (parameters not simplified)
		double _calcedValuePartlySimplified; //value calced via FuncParser (some parameters simplified)

	protected:
		String^ _stringToParse;     //math expression to pe parsed (set by inherited test class)
		double _calcedValueDirect;  //value of math expression calculated directly
		                            //(via C++ code; set by inherited class)

		double CalcedValueSimplified()
		{
			return _calcedValueSimplified;
		}
		double CalcedValueNotSimplified()
		{
			return _calcedValueNotSimplified;
		}
		double CalcedValuePartlySimplified()
		{
			return _calcedValuePartlySimplified;
		}

		virtual void Because() override
        {
			OSPSuite::FuncParser::IFuncParserErrorData^ _fpED;
			_fpED = gcnew OSPSuite::FuncParser::FuncParserErrorData();

			sut->StringToParse::set(_stringToParse);
			array<double>^ args = gcnew array<double>{x,y,z}; 
			
			_calcedValueSimplified = sut->CalcExpression(args,_fpED);
			if (_fpED->ErrorNumber != OSPSuite::FuncParser::errNumber::err_OK)
				throw gcnew Exception(_fpED->Description+"\nin "+_fpED->Source);

			_notSimplifiedParsedFunction = gcnew OSPSuite::FuncParser::ParsedFunction();
			_notSimplifiedParsedFunction->CopyFrom(sut);
			_notSimplifiedParsedFunction->SimplifyParametersAllowed::set(false);
			_calcedValueNotSimplified = _notSimplifiedParsedFunction->CalcExpression(args,_fpED);
			if (_fpED->ErrorNumber != OSPSuite::FuncParser::errNumber::err_OK)
				throw gcnew Exception(_fpED->Description+"\nin "+_fpED->Source);

			_partlySimplifiedParsedFunction = gcnew OSPSuite::FuncParser::ParsedFunction();
			_partlySimplifiedParsedFunction->CopyFrom(sut);
			IList<String^>^ paramNamesNotToSimplify=gcnew System::Collections::Generic::List<String^>();
			paramNamesNotToSimplify->Add("p1");
			_partlySimplifiedParsedFunction->ParametersNotToSimplify::set(paramNamesNotToSimplify);
			_calcedValuePartlySimplified = _partlySimplifiedParsedFunction->CalcExpression(args,_fpED);
			if (_fpED->ErrorNumber != OSPSuite::FuncParser::errNumber::err_OK)
				throw gcnew Exception(_fpED->Description+"\nin "+_fpED->Source);

		}

        virtual void should_return_correct_value_when_simplified()
        {
			BDDExtensions::ShouldBeEqualTo(_calcedValueSimplified,_calcedValueDirect);
			//BDDExtensions::ShouldBeTrue(DeviationWithinTolerance(_calcedValueSimplified,_calcedValueDirect));
        }

        virtual void should_return_correct_value_when_not_simplified()
        {
			BDDExtensions::ShouldBeEqualTo(_calcedValueNotSimplified,_calcedValueDirect);
			//BDDExtensions::ShouldBeTrue(DeviationWithinTolerance(_calcedValueNotSimplified,_calcedValueDirect));
        }

        virtual void should_return_correct_value_when_partly_simplified()
        {
			BDDExtensions::ShouldBeEqualTo(_calcedValuePartlySimplified,_calcedValueDirect);
        }

	private:

		bool DeviationWithinTolerance(double value1, double value2)
		{
			const double relTol = 1e-15;
			
			double absMinValue = fabs(value1) <= fabs(value2) ? fabs(value1) : fabs(value2);

			if (absMinValue == 0.0)
			{
				return value1==value2;
			}

			double deviation = fabs(value1-value2)/absMinValue;

			return (deviation < relTol);
		}	

    };

	public ref class when_calculating_expr01 : public when_calculating_expressions
	{
	protected:
		virtual void Because() override
        {
			_stringToParse = "(x+z)^(x+z)*z+p1-x+y+z+p1*p2-(x+y-z)^2";
			_calcedValueDirect = pow((x+z),(x+z))*z+p1-x+y+z+p1*p2-pow((x+y-z),2);

			when_calculating_expressions::Because();
        }

	public:
        [TestAttribute]
        virtual void should_return_correct_value_when_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_not_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_not_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_partly_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_partly_simplified();
        }
	};

	public ref class when_calculating_expr02 : public when_calculating_expressions
	{
	protected:
		virtual void Because() override
        {
			_stringToParse = "(x+z)^(x+z)*z+p1-x-y+z+p1*p2-(x+y-z)^2";
			_calcedValueDirect = pow((x+z),(x+z))*z+p1-x-y+z+p1*p2-pow((x+y-z),2);

			when_calculating_expressions::Because();
        }

	public:
        [TestAttribute]
        virtual void should_return_correct_value_when_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_not_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_not_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_partly_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_partly_simplified();
        }
	};

	public ref class when_calculating_expr03 : public when_calculating_expressions
	{
	protected:
		virtual void Because() override
        {
			_stringToParse = "(x+z)^(x+z)*z+p1-x-y+z+p1*p2-(x-y-z)^2";
			_calcedValueDirect = pow((x+z),(x+z))*z+p1-x-y+z+p1*p2-pow((x-y-z),2);

			when_calculating_expressions::Because();
        }

	public:
        [TestAttribute]
        virtual void should_return_correct_value_when_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_not_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_not_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_partly_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_partly_simplified();
        }
	};

	public ref class when_calculating_expr04 : public when_calculating_expressions
	{
	protected:
		virtual void Because() override
        {
			_stringToParse = "(x+z)^(x+z)*z+p1-x-y-z+p1*p2-(x-y-z)^2";
			_calcedValueDirect = pow((x+z),(x+z))*z+p1-x-y-z+p1*p2-pow((x-y-z),2);

			when_calculating_expressions::Because();
        }

	public:
        [TestAttribute]
        virtual void should_return_correct_value_when_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_not_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_not_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_partly_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_partly_simplified();
        }
	};

	public ref class when_calculating_expr05 : public when_calculating_expressions
	{
	protected:
		virtual void Because() override
        {
			_stringToParse = "(x+z)^(x+z)*z+p1-x-y-z-p1*p2-(x-y-z)^2";
			_calcedValueDirect = pow((x+z),(x+z))*z+p1-x-y-z-p1*p2-pow((x-y-z),2);

			when_calculating_expressions::Because();
        }

	public:
        [TestAttribute]
        virtual void should_return_correct_value_when_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_not_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_not_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_partly_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_partly_simplified();
        }
	};

	public ref class when_calculating_expr06 : public when_calculating_expressions
	{
	protected:
		virtual void Because() override
        {
			_stringToParse = "(x+z)^(x+z)*z+p1-x+y-z-p1*p2-(x-y-z)^2";
			_calcedValueDirect = pow((x+z),(x+z))*z+p1-x+y-z-p1*p2-pow((x-y-z),2);

			when_calculating_expressions::Because();
        }

	public:
        [TestAttribute]
        virtual void should_return_correct_value_when_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_not_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_not_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_partly_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_partly_simplified();
        }
	};

	public ref class when_calculating_expr07 : public when_calculating_expressions
	{
	protected:
		virtual void Because() override
        {
			_stringToParse = "(x+z)^(x+z)*z+p1-x+y-z+p1*p2-(x-y-z)^2";
			_calcedValueDirect = pow((x+z),(x+z))*z+p1-x+y-z+p1*p2-pow((x-y-z),2);

			when_calculating_expressions::Because();
        }

	public:
        [TestAttribute]
        virtual void should_return_correct_value_when_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_not_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_not_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_partly_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_partly_simplified();
        }
	};

	public ref class when_calculating_expr08 : public when_calculating_expressions
	{
	protected:
		virtual void Because() override
        {
			_stringToParse = "p1*p2+p1-p2-x-y-z";
			_calcedValueDirect = p1*p2+p1-p2-x-y-z;

			when_calculating_expressions::Because();
        }

	public:
        [TestAttribute]
        virtual void should_return_correct_value_when_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_not_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_not_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_partly_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_partly_simplified();
        }
	};

	public ref class when_calculating_expr09 : public when_calculating_expressions
	{
	protected:
		virtual void Because() override
        {
			_stringToParse = "sin(x)*cos(y+z-p1/(2.12+3.16-log(z+p1^p2)))";
			_calcedValueDirect = sin(x)*cos(y+z-p1/(2.12+3.16-log(z+pow(p1,p2))));

			when_calculating_expressions::Because();
        }

	public:
        [TestAttribute]
        virtual void should_return_correct_value_when_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_not_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_not_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_partly_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_partly_simplified();
        }
	};

	public ref class when_calculating_expr10 : public when_calculating_expressions
	{
	protected:
		virtual void Because() override
        {
			_stringToParse = "sin(x)*cos(y+z-p1/(2.12+3.16-tan(z+p1^p2)))";
			_calcedValueDirect = sin(x)*cos(y+z-p1/(2.12+3.16-tan(z+pow(p1,p2))));

			when_calculating_expressions::Because();
        }

	public:
        [TestAttribute]
        virtual void should_return_correct_value_when_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_not_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_not_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_partly_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_partly_simplified();
        }
	};

	public ref class when_calculating_expr11 : public when_calculating_expressions
	{
	protected:
		virtual void Because() override
        {
			_stringToParse = "sin(x*p2/(p1-x*y+z-p1-p2)/cos(z)+tan(x^exp(y-asin(z/10)*acos(-0.345)+atan(exp(log(2)+log(1))))))*(sinh(cosh(tanh(sqrt(p1+p2-z)))))";
			_calcedValueDirect = sin(x*p2/(p1-x*y+z-p1-p2)/cos(z)+tan(pow(x,exp(y-asin(z/10)*acos(-0.345)+atan(exp(log(2.0)+log(1.0)))))))*(sinh(cosh(tanh(sqrt(p1+p2-z)))));

			when_calculating_expressions::Because();
        }

	public:
        [TestAttribute]
        virtual void should_return_correct_value_when_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_not_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_not_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_partly_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_partly_simplified();
        }
	};

	public ref class when_calculating_expr12 : public when_calculating_expressions
	{
	protected:
		virtual void Because() override
        {
			const double pI = 2.0 * asin (1.0);
			const double Pi = pI;
			const double E = exp(1.0);

			_stringToParse = "(x-(1.2-(2.3/(4.5/(12.13*2/7))/2)-3)*sin(pI*log(E^2)))*(x+13-12-3-4-5-6-7/(1+1/(1+1/(1+1/atan(Pi/6)))))";
			_calcedValueDirect = (x-(1.2-(2.3/(4.5/(12.13*2/7))/2)-3)*sin(pI*log(E*E)))*(x+13-12-3-4-5-6-7/(1+1/(1+1/(1+1/atan(Pi/6)))));

			when_calculating_expressions::Because();
        }

	public:
        [TestAttribute]
        virtual void should_return_correct_value_when_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_not_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_not_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_partly_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_partly_simplified();
        }
	};
	
	public ref class when_calculating_expr13 : public when_calculating_expressions
	{
	protected:
		virtual void Because() override
        {
			_stringToParse = "(y>z or z>y) and not (p1=p2)";
			_calcedValueDirect = (y>z || z>y) && !(p1==p2) ? 1.0 : 0.0;

			when_calculating_expressions::Because();
        }

	public:
        [TestAttribute]
        virtual void should_return_correct_value_when_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_not_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_not_simplified();
        }

        [TestAttribute]
        virtual void should_return_correct_value_when_partly_simplified() override
        {
			when_calculating_expressions::should_return_correct_value_when_partly_simplified();
        }
	};

	public ref class when_calculating_invalid_expression : public concern_for_parsed_function
	{
	protected:
		OSPSuite::FuncParser::IFuncParserErrorData^ _fpED;
		virtual void Context() override
		{
			concern_for_parsed_function::Context();
			_fpED = gcnew OSPSuite::FuncParser::FuncParserErrorData();
		}

	public:

		[TestAttribute]
		virtual void should_set_error_when_parsing_empty_string()
		{
			sut->StringToParse::set(" ");
			sut->Parse(_fpED);

			BDDExtensions::ShouldNotBeEqualTo(_fpED->ErrorNumber, OSPSuite::FuncParser::errNumber::err_OK);
		}

		[TestAttribute]
		virtual void should_set_error_and_return_nan_when_calculating_empty_string()
		{
			sut->StringToParse::set(" ");
			array<double>^ args = gcnew array<double>{x,y,z};

			double d = sut->CalcExpression(args, _fpED);
			
			BDDExtensions::ShouldBeEqualTo(d, FuncParserNative::Math::GetNaN());
			BDDExtensions::ShouldNotBeEqualTo(_fpED->ErrorNumber, OSPSuite::FuncParser::errNumber::err_OK);
		}

	};

	public ref class when_calculating_random_function : public when_calculating_expressions
	{
	protected:

		virtual void Because() override
		{
			_stringToParse = "x*RND()+y*Rnd()+SRND()";

			when_calculating_expressions::Because();
		}

	public:

		[TestAttribute]
		virtual void should_return_value_between_0_and_1()
		{
			//---- 0<Rnd, Srnd<1 ==>  0 <= x*rnd+y*rnd+srnd <= x+y+1
			double value;

			value = CalcedValueNotSimplified();
			BDDExtensions::ShouldBeTrue((value>=0)&&(value<=x+y+1));

			value = CalcedValuePartlySimplified();
			BDDExtensions::ShouldBeTrue((value>=0)&&(value<=x+y+1));

			value = CalcedValueSimplified();
			BDDExtensions::ShouldBeTrue((value>=0)&&(value<=x+y+1));
		}

	};
}