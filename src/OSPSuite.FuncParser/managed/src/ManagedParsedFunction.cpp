#include "FuncParserManaged/Conversions.h"
#include "FuncParserManaged/ManagedParsedFunction.h"
#include "FuncParserManaged/ExceptionHelper.h"

namespace OSPSuite
{
	namespace FuncParser
	{
		FuncParserNative::ParsedFunction * ParsedFunction::GetNativeParsedFunction()
		{
			return _parsedFunction;
		}


		//internal constructor
		void ParsedFunction::CopyFrom(ParsedFunction^ srcFunction)
		{
			CaseSensitive::set(srcFunction->CaseSensitive::get());
			VariableNames::set(srcFunction->VariableNames::get());
			ParameterNames::set(srcFunction->ParameterNames::get());
			ParameterValues::set(srcFunction->ParameterValues::get());
			SimplifyParametersAllowed::set(srcFunction->SimplifyParametersAllowed::get());
			LogicOperatorsAllowed::set(srcFunction->LogicOperatorsAllowed::get());
			StringToParse::set(srcFunction->StringToParse::get());
			ComparisonTolerance::set(srcFunction->ComparisonTolerance::get());
			LogicalNumericMixAllowed::set(srcFunction->LogicalNumericMixAllowed::get());
			ParametersNotToSimplify::set(srcFunction->ParametersNotToSimplify::get());
		}

		//constructor
		ParsedFunction::ParsedFunction()
		{
			_parsedFunction = new FuncParserNative::ParsedFunction();
		}

		// destructor
		ParsedFunction::~ParsedFunction()
		{
			if (_parsedFunction)
				delete _parsedFunction;
			_parsedFunction = NULL;
		}

		// finalizer
		ParsedFunction::!ParsedFunction()
		{
			if (_parsedFunction)
				delete _parsedFunction;
			_parsedFunction = NULL;
		}

		bool ParsedFunction::CaseSensitive::get()
		{
			FuncParserNative::FuncParserErrorData ED;
			bool ret = _parsedFunction->GetCaseSensitive(ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);

			return ret;
		}

		void ParsedFunction::CaseSensitive::set(bool caseSensitive)
		{
			FuncParserNative::FuncParserErrorData ED;
			_parsedFunction->SetCaseSensitive(caseSensitive, ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);
		}

		bool ParsedFunction::SimplifyParametersAllowed::get()
		{
			FuncParserNative::FuncParserErrorData ED;
			bool ret = _parsedFunction->GetSimplifyParametersAllowed(ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);

			return ret;
		}

		void ParsedFunction::SimplifyParametersAllowed::set(bool simplifyParametersAllowed)
		{
			FuncParserNative::FuncParserErrorData ED;
			_parsedFunction->SetSimplifyParametersAllowed(simplifyParametersAllowed, ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);
		}

		bool ParsedFunction::LogicOperatorsAllowed::get()
		{
			FuncParserNative::FuncParserErrorData ED;
			bool ret = _parsedFunction->GetLogicOperatorsAllowed(ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);

			return ret;
		}

		void ParsedFunction::LogicOperatorsAllowed::set(bool logicOperatorsAllowed)
		{
			FuncParserNative::FuncParserErrorData ED;
			_parsedFunction->SetLogicOperatorsAllowed(logicOperatorsAllowed, ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);
		}

		bool ParsedFunction::LogicalNumericMixAllowed::get()
		{
			FuncParserNative::FuncParserErrorData ED;
			bool ret = _parsedFunction->GetLogicalNumericMixAllowed(ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);

			return ret;
		}

		void ParsedFunction::LogicalNumericMixAllowed::set(bool logicalNumericMixAllowed)
		{
			FuncParserNative::FuncParserErrorData ED;
			_parsedFunction->SetLogicalNumericMixAllowed(logicalNumericMixAllowed, ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);
		}

		double ParsedFunction::ComparisonTolerance::get()
		{
			FuncParserNative::FuncParserErrorData ED;
			double ret = _parsedFunction->GetComparisonTolerance(ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);

			return ret;
		}

		void ParsedFunction::ComparisonTolerance::set(double comparisonTolerance)
		{
			FuncParserNative::FuncParserErrorData ED;
			_parsedFunction->SetComparisonTolerance(comparisonTolerance, ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);
		}

		System::String^ ParsedFunction::StringToParse::get()
		{
			FuncParserNative::FuncParserErrorData ED;
			System::String^ ret = CPPToNETConversions::MarshalString(_parsedFunction->GetStringToParse(ED));

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);

			return ret;
		}

		void ParsedFunction::StringToParse::set(System::String^ stringToParse)
		{
			FuncParserNative::FuncParserErrorData ED;
			_parsedFunction->SetStringToParse(NETToCPPConversions::MarshalString(stringToParse), ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);
		}

		System::String^ ParsedFunction::XMLString::get()
		{
			FuncParserNative::FuncParserErrorData ED;
			System::String^ ret = CPPToNETConversions::MarshalString(_parsedFunction->GetXMLString(ED));

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);

			return ret;
		}

		IEnumerable<System::String^>^ ParsedFunction::VariableNames::get()
		{
			FuncParserNative::FuncParserErrorData ED;
			IEnumerable<System::String^>^ ret = CPPToNETConversions::StringVectorToList(_parsedFunction->GetVariableNames(ED));

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);

			return ret;
		}

		void ParsedFunction::VariableNames::set(IEnumerable<System::String^>^ variableNames)
		{
			FuncParserNative::FuncParserErrorData ED;

			std::vector<std::string> variableNamesVec;
			NETToCPPConversions::StringEnumerableToVector(variableNames, variableNamesVec);

			_parsedFunction->SetVariableNames(variableNamesVec, ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);
		}

		IEnumerable<System::String^>^ ParsedFunction::ParameterNames::get()
		{
			FuncParserNative::FuncParserErrorData ED;
			IEnumerable<System::String^>^ ret = CPPToNETConversions::StringVectorToList(_parsedFunction->GetParameterNames(ED));

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);

			return ret;
		}

		void ParsedFunction::ParameterNames::set(IEnumerable<System::String^>^ parameterNames)
		{
			FuncParserNative::FuncParserErrorData ED;

			std::vector<std::string> parameterNamesVec;
			NETToCPPConversions::StringEnumerableToVector(parameterNames, parameterNamesVec);

			_parsedFunction->SetParameterNames(parameterNamesVec, ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);
		}

		IEnumerable<System::String^>^ ParsedFunction::ParametersNotToSimplify::get()
		{
			FuncParserNative::FuncParserErrorData ED;
			IEnumerable<System::String^>^ ret = CPPToNETConversions::StringVectorToList(_parsedFunction->GetParametersNotToSimplify(ED));

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);

			return ret;
		}

		void ParsedFunction::ParametersNotToSimplify::set(IEnumerable<System::String^>^ parametersNotToSimplify)
		{
			FuncParserNative::FuncParserErrorData ED;

			std::vector<std::string> parametersNotToSimplifyVec;
			NETToCPPConversions::StringEnumerableToVector(parametersNotToSimplify, parametersNotToSimplifyVec);

			_parsedFunction->SetParametersNotToSimplify(parametersNotToSimplifyVec, ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);
		}

		array<double>^ ParsedFunction::ParameterValues::get()
		{
			FuncParserNative::FuncParserErrorData ED;
			array<double>^ ret = CPPToNETConversions::DoubleVectorToArray(_parsedFunction->GetParameterValues(ED));

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);

			return ret;
		}

		void ParsedFunction::ParameterValues::set(array<double>^ parameterValues)
		{
			FuncParserNative::FuncParserErrorData ED;

			std::vector<double> parameterValuesVec;
			NETToCPPConversions::DoubleArrayToVector(parameterValues, parameterValuesVec);

			_parsedFunction->SetParameterValues(parameterValuesVec, ED);

			if (ED.GetNumber() != FuncParserNative::FuncParserErrorData::err_OK)
				ExceptionHelper::ThrowExceptionFrom(ED);
		}

		void ParsedFunction::Parse(IFuncParserErrorData^ ED)
		{
			if (ED == nullptr)
				throw gcnew System::Exception("ErrorData argument not set in ParsedFunction::Parse");

			FuncParserNative::FuncParserErrorData fpED;

			_parsedFunction->Parse(fpED);

			((FuncParserErrorData^)ED)->SetupFrom(fpED);
		}

		double ParsedFunction::CalcExpression(array<double>^ args, IFuncParserErrorData^ ED)
		{
			if (ED == nullptr)
				throw gcnew System::Exception("ErrorData argument not set in ParsedFunction::CalcExpression");

			FuncParserNative::FuncParserErrorData fpED;
			double ret;

			std::vector<double> argsVec;
			NETToCPPConversions::DoubleArrayToVector(args, argsVec);

			ret = _parsedFunction->CalcExpression(argsVec, fpED);

			((FuncParserErrorData^)ED)->SetupFrom(fpED);

			return ret;
		}
	}
}