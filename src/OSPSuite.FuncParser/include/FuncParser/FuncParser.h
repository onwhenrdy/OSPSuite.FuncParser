#ifndef _FuncParser_H_
#define _FuncParser_H_

#include "FuncParser/Constants.h"
#include "FuncParser/ElemFunctions.h"
#include <vector>
#include <assert.h>
#include "FuncParser/FuncParserTypeDefs.h"
#include "FuncParser/FuncNode.h"

namespace FuncParserNative
{

class FuncNode;
class FuncParserErrorData;

class FuncParser
{	
	private:
		
		//Is used for error message only! Will be set at the beginning of the Parse function
		std::string m_StringToParse;
	
	public:
		const static std::string conNOTSymbol;
		const static std::string conORSymbol;
		const static std::string conANDSymbol;
		const static std::string conListDelimiter;
	
	private:
		Constants m_Constants;
		ElemFunctions * m_ElemFunctions;
		enum enmLevelOfAbstraction 
		{
			LOA_IF = 1,
			LOA_OR,
			LOA_AND,
			LOA_NOT,
			LOA_Comparison,
			LOA_PLUSMINUS,
			LOA_MULDIV,
			LOA_FACTOR
		};
		std::vector < std::string > m_VariableNames;
		std::vector < std::string > m_ParameterNames;
		bool IsNumeric (const std::string & str);
		double StringToDouble (const std::string & str);
	
	public:
		static std::string ToUpper (const std::string & Source);
	
	private:
		std::string StringReplace (const std::string & source, const char * find, const char * replace, bool CaseSensitive = true);
		std::string StringReplace (const std::string & source, const std::string & find, const std::string & replace, bool CaseSensitive = true);
		std::string ParsedStringToDisplayString (const std::string & str);
	
	public:
		FuncParser ();
		~FuncParser ();
		const Constants & GetConstants () const;
	
	private:
		bool IsBracketed (const std::string & SubExpr);
		void RemoveBrackets (std::string & SubExpr);
	
	public:
		FuncNode * Parse (const std::string & ParsedString, const std::vector < std::string > & VariableNames, const std::vector < std::string > & ParameterNames, bool CaseSensitive, bool LogicOperatorsAllowed, double ComparisonTolerance, bool LogicalNumericMixAllowed, FuncParserErrorData & ED);
	
	private:
		void EvalExpression (FuncNode * SubNode, std::string SubExpr, enmLevelOfAbstraction LevelOfAbstraction, FuncParserErrorData & ED);
		void EvalNOTOperand (FuncNode * SubNode, std::string SubExpr, FuncParserErrorData & ED);
		void EvalComparison (FuncNode * SubNode, std::string SubExpr, FuncParserErrorData & ED);
		void EvalFactor (FuncNode * SubNode, std::string SubExpr, FuncParserErrorData & ED);
		void EvalIF (FuncNode * SubNode, std::string SubExpr, FuncParserErrorData & ED);
		bool IsScientificNumber (const std::string & SubExpr, long OpPos);
	
	public:
		void CheckVarParamNames (const std::vector < std::string > & VariableNames, const std::vector < std::string > & ParameterNames, FuncParserErrorData & ED);

		//Moves terms of the expression so that max. one occurrence of Op2 appears:
		//e.g. a-b+c-d => a+c-(b+d)
		//        x/y/z/t => x/(y*z*t)
		void RearrangeTerms (std::string & SubExpr, enmLevelOfAbstraction LevelOfAbstraction, std::string Op1, std::string Op2, FuncParserErrorData & ED);
		
		//Parses the string <SubExpr> starting at <FirstPos> till <Op1> or <Op2> is found (or the end of the string is reached) and returns next term in the expression .
		//Returns empty string if no more terms available.
		//
		//<NewOp> returns reached operand (<Op1> or <Op2>) or empty string for last term.
		//
		//<FirstPos> is adjusted to the start position of the next term at the end of the function
		std::string GetNextTerm (const std::string & SubExpr, enmLevelOfAbstraction LevelOfAbstraction, std::string Op1, std::string Op2, long & FirstPos, std::string & NewOp, FuncParserErrorData & ED);
};

}//.. end "namespace FuncParserNative"


#endif //_FuncParser_H_

