#ifdef WIN32
#pragma warning( disable : 4691)
#endif

#include "FuncParserManaged/Conversions.h"
#include "FuncParserManaged/ManagedFuncParser.h"
#include "FuncParserManaged/ExceptionHelper.h"
#include "FuncParserManaged/ManagedFuncParserErrorData.h"

namespace UnitTests
{
    using namespace OSPSuite::BDDHelper;
    using namespace OSPSuite::BDDHelper::Extensions;
    using namespace NUnit::Framework;
	using namespace System::Collections::Generic;
	using namespace System;
	using namespace OSPSuite::FuncParser;

	public ref class concern_for_func_parser abstract : ContextSpecification<OSPSuite::FuncParser::FuncParser^>
    {
    public:
		virtual void GlobalContext() override
		{}

	protected:

        virtual void Context() override
        {
            sut=gcnew OSPSuite::FuncParser::FuncParser();
        }
    };


	public ref class when_checking_if_variable_name_is_valid : public concern_for_func_parser
	{
	protected:
		virtual void Because() override
        {
        }

	public:
        [TestAttribute]
        virtual void should_return_coorect_result_checking_name_01()
        {
			IFuncParserErrorData^ ED = gcnew FuncParserErrorData();

			BDDExtensions::ShouldBeTrue(sut->IsValidVariableOrParameterName("x",ED));
			BDDExtensions::ShouldBeEqualTo(ED->ErrorNumber::get(), errNumber::err_OK);
			BDDExtensions::ShouldBeTrue(String::IsNullOrEmpty(ED->Description::get()));
        }

        [TestAttribute]
        virtual void should_return_coorect_result_checking_name_02()
        {
			IFuncParserErrorData^ ED = gcnew FuncParserErrorData();

			BDDExtensions::ShouldBeFalse(sut->IsValidVariableOrParameterName("",ED));
			BDDExtensions::ShouldNotBeEqualTo(ED->ErrorNumber::get(), errNumber::err_OK);
			BDDExtensions::ShouldBeFalse(String::IsNullOrEmpty(ED->Description::get()));
        }

        [TestAttribute]
        virtual void should_return_coorect_result_checking_name_03()
        {
			IFuncParserErrorData^ ED = gcnew FuncParserErrorData();

			BDDExtensions::ShouldBeFalse(sut->IsValidVariableOrParameterName("a+b",ED));
			BDDExtensions::ShouldNotBeEqualTo(ED->ErrorNumber::get(), errNumber::err_OK);
			BDDExtensions::ShouldBeFalse(String::IsNullOrEmpty(ED->Description::get()));
        }

        [TestAttribute]
        virtual void should_return_coorect_result_checking_name_04()
        {
			IFuncParserErrorData^ ED = gcnew FuncParserErrorData();

			BDDExtensions::ShouldBeFalse(sut->IsValidVariableOrParameterName("pi",ED));
			BDDExtensions::ShouldNotBeEqualTo(ED->ErrorNumber::get(), errNumber::err_OK);
			BDDExtensions::ShouldBeFalse(String::IsNullOrEmpty(ED->Description::get()));
        }

        [TestAttribute]
        virtual void should_return_coorect_result_checking_name_05()
        {
			IFuncParserErrorData^ ED = gcnew FuncParserErrorData();

			BDDExtensions::ShouldBeFalse(sut->IsValidVariableOrParameterName("e",ED));
			BDDExtensions::ShouldNotBeEqualTo(ED->ErrorNumber::get(), errNumber::err_OK);
			BDDExtensions::ShouldBeFalse(String::IsNullOrEmpty(ED->Description::get()));
        }

        [TestAttribute]
        virtual void should_return_coorect_result_checking_name_06()
        {
			IFuncParserErrorData^ ED = gcnew FuncParserErrorData();

			BDDExtensions::ShouldBeFalse(sut->IsValidVariableOrParameterName("sin",ED));
			BDDExtensions::ShouldNotBeEqualTo(ED->ErrorNumber::get(), errNumber::err_OK);
			BDDExtensions::ShouldBeFalse(String::IsNullOrEmpty(ED->Description::get()));
        }

        [TestAttribute]
        virtual void should_return_coorect_result_checking_name_07()
        {
			IFuncParserErrorData^ ED = gcnew FuncParserErrorData();

			BDDExtensions::ShouldBeFalse(sut->IsValidVariableOrParameterName("12",ED));
			BDDExtensions::ShouldNotBeEqualTo(ED->ErrorNumber::get(), errNumber::err_OK);
			BDDExtensions::ShouldBeFalse(String::IsNullOrEmpty(ED->Description::get()));
        }

        [TestAttribute]
        virtual void should_return_coorect_result_checking_name_08()
        {
			IFuncParserErrorData^ ED = gcnew FuncParserErrorData();

			BDDExtensions::ShouldBeTrue(sut->IsValidVariableOrParameterName("a12b",ED));
			BDDExtensions::ShouldBeEqualTo(ED->ErrorNumber::get(), errNumber::err_OK);
			BDDExtensions::ShouldBeTrue(String::IsNullOrEmpty(ED->Description::get()));
        }


	};

}