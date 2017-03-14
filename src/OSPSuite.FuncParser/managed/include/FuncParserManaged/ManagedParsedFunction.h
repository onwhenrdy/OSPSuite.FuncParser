#ifndef _Managed_ParsedFunction_H_
#define _Managed_ParsedFunction_H_

#include <string>
#include "FuncParserManaged/ManagedFuncParserErrorData.h"
#include "FuncParser/ParsedFunction.h"

namespace OSPSuite
{
	namespace FuncParser
	{
		public interface class IParsedFunction
		{
			property bool CaseSensitive
			{
				bool get();
				void set(bool caseSensitive);
			}

			property IEnumerable<System::String^>^ VariableNames
			{
				IEnumerable<System::String^>^ get();
				void set(IEnumerable<System::String^>^ variableNames);
			}

			property IEnumerable<System::String^>^ ParameterNames
			{
				IEnumerable<System::String^>^ get();
				void set(IEnumerable<System::String^>^ parameterNames);
			}

			property array<double>^ ParameterValues
			{
				array<double>^ get();
				void set(array<double>^ parameterValues);
			}

			property bool SimplifyParametersAllowed
			{
				bool get();
				void set(bool simplifyParametersAllowed);
			}

			property bool LogicOperatorsAllowed
			{
				bool get();
				void set(bool logicOperatorsAllowed);
			}

			property System::String^ StringToParse
			{
				System::String^ get();
				void set(System::String^ stringToParse);
			}

			property double ComparisonTolerance
			{
				double get();
				void set(double comparisonTolerance);
			}

			property System::String^ XMLString
			{
				System::String^ get();
			}

			property bool LogicalNumericMixAllowed
			{
				bool get();
				void set(bool logicalNumericMixAllowed);
			}

			property IEnumerable<System::String^>^ ParametersNotToSimplify
			{
				IEnumerable<System::String^>^ get();
				void set(IEnumerable<System::String^>^ parametersNotToSimplify);
			}

			void Parse(IFuncParserErrorData^ ED);

			double CalcExpression(array<double>^ args, IFuncParserErrorData^ ED);

		};


		public ref class ParsedFunction : IParsedFunction
		{
		private:
			FuncParserNative::ParsedFunction * _parsedFunction;
		internal:
			void CopyFrom(ParsedFunction^ srcFunction);
			FuncParserNative::ParsedFunction * GetNativeParsedFunction();
		public:

			//constructor
			ParsedFunction();

			// destructor
			~ParsedFunction();

			// finalizer
			!ParsedFunction();

			property bool CaseSensitive
			{
				virtual bool get();
				virtual void set(bool caseSensitive);
			}

			property IEnumerable<System::String^>^ VariableNames
			{
				virtual IEnumerable<System::String^>^ get();
				virtual void set(IEnumerable<System::String^>^ variableNames);
			}

			property IEnumerable<System::String^>^ ParameterNames
			{
				virtual IEnumerable<System::String^>^ get();
				virtual void set(IEnumerable<System::String^>^ parameterNames);
			}

			property array<double>^ ParameterValues
			{
				virtual array<double>^ get();
				virtual void set(array<double>^ parameterValues);
			}

			property bool SimplifyParametersAllowed
			{
				virtual bool get();
				virtual void set(bool simplifyParametersAllowed);
			}

			property bool LogicOperatorsAllowed
			{
				virtual bool get();
				virtual void set(bool logicOperatorsAllowed);
			}

			property System::String^ StringToParse
			{
				virtual System::String^ get();
				virtual void set(System::String^ stringToParse);
			}

			property double ComparisonTolerance
			{
				virtual double get();
				virtual void set(double comparisonTolerance);
			}

			property System::String^ XMLString
			{
				virtual System::String^ get();
			}

			property bool LogicalNumericMixAllowed
			{
				virtual bool get();
				virtual void set(bool logicalNumericMixAllowed);
			}

			property IEnumerable<System::String^>^ ParametersNotToSimplify
			{
				virtual IEnumerable<System::String^>^ get();
				virtual void set(IEnumerable<System::String^>^ parametersNotToSimplify);
			}

			virtual void Parse(IFuncParserErrorData^ ED);

			virtual double CalcExpression(array<double>^ args, IFuncParserErrorData^ ED);

		};

	}//.. end "namespace FuncParserNET"
}//.. end "namespace OSPSuite"


#endif //_Managed_ParsedFunction_H_

