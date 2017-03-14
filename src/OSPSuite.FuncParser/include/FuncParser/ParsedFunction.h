#ifndef _ParsedFunction_H_
#define _ParsedFunction_H_

#include "FuncParser/QuantityDimensionInfo.h"
#include "FuncParser/FuncParserTypeDefs.h"

namespace FuncParserNative
{

class FuncNode;
class FuncParserErrorData;

class ParsedFunction
{	
	private:
		typedef std::vector < std::string > StringVector ;
		typedef std::vector < std::string >::iterator StringVectorIterator ;
		typedef std::vector < double > DoubleVector ;
		FuncNode * m_FuncNode;
		FuncNode * m_SimplifiedNode;
		bool m_CaseSensitive;
		StringVector m_VariableNames;
		StringVector m_ParameterNames;
		StringVector m_FixedParameters;
		double * m_ParameterValues;
		bool m_SimplifyParametersAllowed;
		bool m_LogicOperatorsAllowed;
		bool m_LogicalNumericMixAllowed;
		bool m_Parsed;
		double m_ComparisonTolerance;
		bool m_ParametersSimplified;
		std::string m_StringToParse;
		bool m_ParametersAvailable;
		void ResetParsedState (bool SimplifiedStateOnly = false);
		DoubleVector GetFixedParametersIndexList (FuncParserErrorData & ED);
		void SimplifyParameters (FuncParserErrorData & ED);
		std::vector<std::string> ExtractQuantityNames(const std::vector<QuantityDimensionInfo> & quantityDimensions);

	public:
		FUNCPARSER_EXPORT ParsedFunction ();
		FUNCPARSER_EXPORT ~ParsedFunction ();
		FUNCPARSER_EXPORT bool GetCaseSensitive (FuncParserErrorData & ED);
		FUNCPARSER_EXPORT void SetCaseSensitive (bool p_CaseSensitive, FuncParserErrorData & ED);
		FUNCPARSER_EXPORT const StringVector & GetVariableNames (FuncParserErrorData & ED) const;
		FUNCPARSER_EXPORT void SetVariableNames (const StringVector & p_VariableNames, FuncParserErrorData & ED);
		FUNCPARSER_EXPORT const StringVector & GetParameterNames (FuncParserErrorData & ED) const;
		FUNCPARSER_EXPORT void SetParameterNames (const StringVector & p_ParameterNames, FuncParserErrorData & ED);
		FUNCPARSER_EXPORT const DoubleVector GetParameterValues (FuncParserErrorData & ED) const;
		FUNCPARSER_EXPORT void SetParameterValues (const DoubleVector & p_ParameterValues, FuncParserErrorData & ED);
		FUNCPARSER_EXPORT bool GetSimplifyParametersAllowed (FuncParserErrorData & ED);
		FUNCPARSER_EXPORT void SetSimplifyParametersAllowed (bool p_SimplifyParametersAllowed, FuncParserErrorData & ED);
		FUNCPARSER_EXPORT bool GetLogicOperatorsAllowed (FuncParserErrorData & ED);
		FUNCPARSER_EXPORT void SetLogicOperatorsAllowed (bool p_LogicOperatorsAllowed, FuncParserErrorData & ED);
		FUNCPARSER_EXPORT bool GetLogicalNumericMixAllowed (FuncParserErrorData & ED);
		FUNCPARSER_EXPORT void SetLogicalNumericMixAllowed (bool p_LogicalNumericMixAllowed, FuncParserErrorData & ED);
		FUNCPARSER_EXPORT double GetComparisonTolerance (FuncParserErrorData & ED);
		FUNCPARSER_EXPORT void SetComparisonTolerance (double p_ComparisonTolerance, FuncParserErrorData & ED);
		FUNCPARSER_EXPORT const std::string GetStringToParse (FuncParserErrorData & ED) const;
		FUNCPARSER_EXPORT void SetStringToParse (const std::string & p_StringToParse, FuncParserErrorData & ED);
		FUNCPARSER_EXPORT void Parse (FuncParserErrorData & ED);
		FUNCPARSER_EXPORT double CalcExpression (const DoubleVector & Arg, FuncParserErrorData & ED);
		FUNCPARSER_EXPORT std::string GetXMLString (FuncParserErrorData & ED, bool InSimplifiedState = true, const std::string & pContainerNodeName = "Rate");
		FUNCPARSER_EXPORT double CalcExpression (double * dArgs, FuncParserErrorData & ED);
		FUNCPARSER_EXPORT void SetParametersNotToSimplify (const StringVector & p_ParameterNames, FuncParserErrorData & ED);
		FUNCPARSER_EXPORT const StringVector & GetParametersNotToSimplify (FuncParserErrorData & ED) const;

		DimensionInfo GetDimensionInfoFor(const std::string & formula,
			                              const std::vector<QuantityDimensionInfo> & quantityDimensions,
										  FuncParserErrorData & ED);
};

}//.. end "namespace FuncParserNative"


#endif //_ParsedFunction_H_

