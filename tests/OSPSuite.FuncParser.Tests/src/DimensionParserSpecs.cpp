#ifdef _WINDOWS
#pragma warning( disable : 4691)
#endif

#include "FuncParserManaged/Conversions.h"
#include "FuncParserManaged/ManagedParsedFunction.h"
#include "FuncParserManaged/ExceptionHelper.h"
#include "FuncParserManaged/ManagedFuncParserErrorData.h"
#include "FuncParser/Math.h"
#include "FuncParserManaged/ManagedDimensionParser.h"

#include <math.h>

namespace UnitTests
{
    using namespace OSPSuite::BDDHelper;
    using namespace OSPSuite::BDDHelper::Extensions;
    using namespace NUnit::Framework;
    using namespace System::Collections::Generic;
    using namespace System;
    using namespace OSPSuite::FuncParser;

    public ref class concern_for_dimension_parser abstract : ContextSpecification<DimensionParser^>
    {
    public:
        virtual void GlobalContext() override
        {}

    protected:
        IQuantityDimensionInfo ^p1, ^p2, ^p3, ^p4, ^p5;
        IList<IQuantityDimensionInfo^>^ _quantityDimensionInfos;
        IDimensionInfo^ _result;
        String^ _formula;
        IFuncParserErrorData^ _fpED;

        virtual void Context() override
        {
            _quantityDimensionInfos=gcnew System::Collections::Generic::List<IQuantityDimensionInfo^>();
            _fpED=gcnew FuncParserErrorData();

            p1 = gcnew QuantityDimensionInfo(gcnew String("p1"),
                                             gcnew DimensionInfo(1,1,1,1,1,1,1));
            p2 = gcnew QuantityDimensionInfo(gcnew String("p2"),
                                             gcnew DimensionInfo(2,2,2,2,2,2,2));
            p3 = gcnew QuantityDimensionInfo(gcnew String("p3"),
                                             gcnew DimensionInfo(0,0,0,0,0,0,0));
            p4 = gcnew QuantityDimensionInfo(gcnew String("p4"),
                                             gcnew DimensionInfo(-3,-2, -1, 0, 1, 2, 3));
            p5 = gcnew QuantityDimensionInfo(gcnew String("p5"),
                                             gcnew DimensionInfo(1,  2,  3, 0, 3, 4, 5));
            _quantityDimensionInfos->Add(p1);
            _quantityDimensionInfos->Add(p2);
            _quantityDimensionInfos->Add(p3);
            _quantityDimensionInfos->Add(p4);
            _quantityDimensionInfos->Add(p5);

            sut=gcnew DimensionParser();
        }

        virtual void Because() override
        {
            _result = sut->GetDimensionInfoFor(_formula, _quantityDimensionInfos, _fpED);
        }

		//compare dimension exponents with tolerance zero
        void ParsedDimensionShouldBeEqualTo(double length, double mass, double time, 
                                            double electricCurrent, double temperature, 
                                            double amount, double luminousIntensity)
        {
			ParsedDimensionShouldBeEqualTo(length, mass, time, 
                                           electricCurrent, temperature, 
                                           amount, luminousIntensity, 0.0);
        }

		//compare dimension exponents with given tolerance
		void ParsedDimensionShouldBeEqualTo(double length, double mass, double time, 
                                            double electricCurrent, double temperature, 
                                            double amount, double luminousIntensity,
											double relTol)
        {
            if (_fpED->ErrorNumber != errNumber::err_OK)
			    throw gcnew Exception(_fpED->Description);

            BDDExtensions::ShouldBeEqualTo(_result->Length,length,relTol);
            BDDExtensions::ShouldBeEqualTo(_result->Mass,mass,relTol);
            BDDExtensions::ShouldBeEqualTo(_result->Time,time,relTol);
            BDDExtensions::ShouldBeEqualTo(_result->ElectricCurrent,electricCurrent,relTol);
            BDDExtensions::ShouldBeEqualTo(_result->Temperature,temperature,relTol);
            BDDExtensions::ShouldBeEqualTo(_result->Amount,amount,relTol);
            BDDExtensions::ShouldBeEqualTo(_result->LuminousIntensity,luminousIntensity,relTol);
        }

		//compare dimension exponents with given value within tolerance
		void AllParsedDimensionsShouldBeEqualTo(double value, double relTol)
		{
			ParsedDimensionShouldBeEqualTo(value,value,value,value,value,value,value,relTol);
		}

		//compare dimension exponents with given value with tolerance zero
		void AllParsedDimensionsShouldBeEqualTo(double value)
		{
			AllParsedDimensionsShouldBeEqualTo(value,0.0);
		}

		//check if error is set to BadArg (for invalid unit parsing, e.g. [kg]+[ml]
		void ShouldSetErrorToErrDimension()
		{
			BDDExtensions::ShouldBeTrue((_fpED->ErrorNumber==errNumber::err_DIMENSION) || 
				                        (_fpED->ErrorNumber==errNumber::err_CANNOTCALC_DIMENSION));
		}

    };


    public ref class when_getting_dimension_mul : public concern_for_dimension_parser
    {
    protected:

        virtual void Context() override
        {
            //p1={1,  1,  1, 1, 1, 1, 1}
            //p2={2,  2,  2, 2, 2, 2, 2}
            //p3={0,  0,  0, 0, 0, 0, 0}
            //p4={-3,-2, -1, 0, 1, 2, 3}
            //p5={1,  2,  3, 0, 3, 4, 5}
            concern_for_dimension_parser::Context();

            _formula="p1*p2*p3";
        }

    public:
        [TestAttribute]
        virtual void should_return_correct_parsed_dimension()
        {
            AllParsedDimensionsShouldBeEqualTo(3);
        }
    };

    public ref class when_getting_dimension_mul_div : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p1={1,  1,  1, 1, 1, 1, 1}
          //p2={2,  2,  2, 2, 2, 2, 2}
          //p3={0,  0,  0, 0, 0, 0, 0}
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();

          _formula="p1*p4/p5";
      }

    public:
        [TestAttribute]
        virtual void should_return_correct_parsed_dimension()
        {
          ParsedDimensionShouldBeEqualTo(-3,-3,-3,1,-1,-1,-1);
        }
    };

    public ref class when_getting_dimension_plus_div_power : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p1={1,  1,  1, 1, 1, 1, 1}
          //p2={2,  2,  2, 2, 2, 2, 2}
          //p3={0,  0,  0, 0, 0, 0, 0}
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();

          _formula="(p1+p2/p1)^17";
      }

    public:
        [TestAttribute]
        virtual void should_return_correct_parsed_dimension()
        {
          AllParsedDimensionsShouldBeEqualTo(17);
        }
    };

    public ref class when_getting_dimension_if_min_minus : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p1={1,  1,  1, 1, 1, 1, 1}
          //p2={2,  2,  2, 2, 2, 2, 2}
          //p3={0,  0,  0, 0, 0, 0, 0}
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();

		  _formula="2>1 ? min(p1-2*p1; p2/p1) : 99";
      }

    public:
        [TestAttribute]
        virtual void should_return_correct_parsed_dimension()
        {
          AllParsedDimensionsShouldBeEqualTo(1);
        }
    };

    public ref class when_getting_dimension_logical_ops : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p1={1,  1,  1, 1, 1, 1, 1}
          //p2={2,  2,  2, 2, 2, 2, 2}
          //p3={0,  0,  0, 0, 0, 0, 0}
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();

          _formula="p3*p1>p2/p1 and p3*p1>=p2/p1 or p3*p1<p2/p1 and not p3*p1<=p2/p1 or p3*p1<>p2/p1";
      }

    public:
        [TestAttribute]
        virtual void should_return_correct_parsed_dimension()
        {
          AllParsedDimensionsShouldBeEqualTo(0);
        }
    };

    public ref class when_getting_dimension_if_max_minus : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p1={1,  1,  1, 1, 1, 1, 1}
          //p2={2,  2,  2, 2, 2, 2, 2}
          //p3={0,  0,  0, 0, 0, 0, 0}
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();

		  _formula="2<1 ? 33 : max(p1-2*p1; p2/p1)";
      }

    public:
        [TestAttribute]
        virtual void should_return_correct_parsed_dimension()
        {
          AllParsedDimensionsShouldBeEqualTo(1);
        }
    };

    public ref class when_getting_dimension_sqrt_exp_log : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p1={1,  1,  1, 1, 1, 1, 1}
          //p2={2,  2,  2, 2, 2, 2, 2}
          //p3={0,  0,  0, 0, 0, 0, 0}
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();

		  _formula="sqrt(p4)*exp(p3)*ln(p3)*log(p3)*log10(p3)";
      }

    public:
        [TestAttribute]
        virtual void should_return_correct_parsed_dimension()
        {
          ParsedDimensionShouldBeEqualTo(-3/2.0,-2/2.0,-1/2.0, 0, 1/2.0,2/2.0,3/2.0);
        }
    };

    public ref class when_getting_dimension_trigonometric : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p1={1,  1,  1, 1, 1, 1, 1}
          //p2={2,  2,  2, 2, 2, 2, 2}
          //p3={0,  0,  0, 0, 0, 0, 0}
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();

		  _formula="acos(p3)+asin(p3)+atan(p3)+cos(p3)+cosh(p3)+sin(p3)+sinh(p3)+tan(p3)+tanh(p3)";
      }

    public:
        [TestAttribute]
        virtual void should_return_correct_parsed_dimension()
        {
          AllParsedDimensionsShouldBeEqualTo(0);
        }
    };

    public ref class when_getting_dimension_random : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p1={1,  1,  1, 1, 1, 1, 1}
          //p2={2,  2,  2, 2, 2, 2, 2}
          //p3={0,  0,  0, 0, 0, 0, 0}
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();

		  _formula="(Rnd()*p1*p1-Srnd()*p2)/(p1*p1)+Rnd()+Srnd()+Rnd()*Srnd()";
      }

    public:
        [TestAttribute]
        virtual void should_return_correct_parsed_dimension()
        {
          AllParsedDimensionsShouldBeEqualTo(0);
        }
    };

	//---- FAILURE Tests
    public ref class when_getting_dimension_which_should_fail_acos : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
		  _formula="acos(p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_asin : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
		  _formula="asin(p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_atan : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
		  _formula="atan(p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_cos : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
		  _formula="cos(p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_cosh : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
		  _formula="cosh(p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_and : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
		  _formula="p5 and 2";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_exp : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
		  _formula="exp(p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_ln : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="ln(p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_log : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="log(p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_log10 : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="log10(p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_power : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="2^p5";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_sin : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="sin(p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_sinh : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="sinh(p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_tan : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="tan(p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_tanh : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="tanh(p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_greater : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="p4>p5";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_greaterequal : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="p4>=p5";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_less : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="p4<p5";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_lessequal : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="p4<=p5";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_unequal : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="p4<>p5";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_max : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="max(p4;p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_min : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="min(p4;p5)";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_minus : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="p4-p5";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_equal : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="p4=p5";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_not : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="NOT p4";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_or : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="p4 OR p5";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_which_should_fail_plus : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();
          _formula="p4+p5";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };

    public ref class when_getting_dimension_if_nonconst_condition_but_same_dimensions : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p1={1,  1,  1, 1, 1, 1, 1}
          //p2={2,  2,  2, 2, 2, 2, 2}
          //p3={0,  0,  0, 0, 0, 0, 0}
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();

		  _formula="p1>p2 ? p1-2*p1 : p2/p1";
      }

    public:
        [TestAttribute]
        virtual void should_return_correct_parsed_dimension()
        {
          AllParsedDimensionsShouldBeEqualTo(1);
        }
    };

    public ref class when_getting_dimension_if_nonconst_condition_and_different_dimensions : public concern_for_dimension_parser
    {
    protected:

      virtual void Context() override
      {
          //p1={1,  1,  1, 1, 1, 1, 1}
          //p2={2,  2,  2, 2, 2, 2, 2}
          //p3={0,  0,  0, 0, 0, 0, 0}
          //p4={-3,-2, -1, 0, 1, 2, 3}
          //p5={1,  2,  3, 0, 3, 4, 5}
          concern_for_dimension_parser::Context();

		  _formula="p1>p2 ? p1-2*p1 : p2";
      }

    public:
        [TestAttribute]
        virtual void should_set_error_data()
        {
            ShouldSetErrorToErrDimension();
        }
    };
}