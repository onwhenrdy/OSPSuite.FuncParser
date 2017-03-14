#ifdef WIN32_PRODUCTION
#pragma managed(push,off)
#endif

#include "FuncParser/FuncParserErrorData.h"
#include "FuncParser/ParsedFunction.h"
#include "FuncParser/FuncNode.h"
#include "FuncParser/Math.h"
#include "FuncParser/FuncParser.h"
#include "FuncParser/StringHelper.h"

#ifdef WIN32_PRODUCTION
#pragma managed(pop)
#endif

namespace FuncParserNative
{

void ParsedFunction::ResetParsedState (bool SimplifiedStateOnly /*= false*/)
{
	m_ParametersSimplified = false;
	if (m_SimplifiedNode != NULL)
	{
		delete m_SimplifiedNode;
		m_SimplifiedNode = NULL;
	}
	if (SimplifiedStateOnly)
		return;

	m_Parsed = false;
	if (m_FuncNode != NULL)
	{
		delete m_FuncNode;
		m_FuncNode = NULL;
	}
}

ParsedFunction::DoubleVector ParsedFunction::GetFixedParametersIndexList (FuncParserErrorData & ED)
{
	const std::string ERROR_SOURCE = "ParsedFunction::GetFixedParametersIndexList";
	ED.Clear();

	DoubleVector IndexList;

	try
	{
        size_t i, Index, k;
		bool bNameValid, bAlreadyUsed;

		for(i=0; i<m_FixedParameters.size(); i++)
		{
			bNameValid = false;
			for(Index=0; Index<m_ParameterNames.size(); Index++)
			{
				if( ( m_CaseSensitive && (m_FixedParameters[i] == m_ParameterNames[Index])) ||
				    (!m_CaseSensitive && (FuncParser::ToUpper(m_FixedParameters[i]) ==
				                          FuncParser::ToUpper(m_ParameterNames[Index]))))
				{
					bNameValid = true;
					//check if fixed parameter name was already set (caller has used some parameter name twice or more
					//In this case: don't raise an error, just ignore it
					bAlreadyUsed = false;
					for(k=0; k<IndexList.size(); k++)
						if(IndexList[k] == Index)
						{
							bAlreadyUsed = true;
							break;
						}
					if(bAlreadyUsed)
						break;

					//Append an index of the fixed variable to the index list
					IndexList.push_back(Index);

					break;
				}
			}
		}
	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
		IndexList.clear();
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
		IndexList.clear();
	}

	return IndexList;
}

void ParsedFunction::SimplifyParameters (FuncParserErrorData & ED)
{
	const std::string ERROR_SOURCE = "ParsedFunction::SimplifyParameters";
	ED.Clear();

	try
	{
		DoubleVector IndexList;
		double Dummy;

		if (!m_ParametersSimplified)
		{
			m_SimplifiedNode = m_FuncNode->Clone();
			if (m_SimplifyParametersAllowed)
			{
				//get the indices of fixed parameters first
				IndexList = GetFixedParametersIndexList(ED);
				if (ED.GetNumber() != FuncParserErrorData::err_OK)
					return;

				//simplify WITH parameter values
				//if parameter values are not set by now, so NULL is passed as the last argument
				//Node simplifying routine checks for this and ignores parameter values in this case
				m_SimplifiedNode->SimplifyNode(Dummy, IndexList, m_ComparisonTolerance, m_ParameterValues);
			}
			else
			{
				//simplify WITHOUT parameter values
				m_SimplifiedNode->SimplifyNode(Dummy, IndexList, m_ComparisonTolerance);
			}
			m_ParametersSimplified = true;
		}

	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}
}

ParsedFunction::ParsedFunction ()
{
	m_CaseSensitive = true;
	m_FuncNode = NULL;
	m_LogicOperatorsAllowed = true;
	m_LogicalNumericMixAllowed = false;
	m_ParametersSimplified = false;
	m_ParameterValues = NULL;
	m_Parsed = false;
	m_SimplifiedNode = NULL;
	m_SimplifyParametersAllowed = true;
	m_ComparisonTolerance = 0.0;
	m_ParametersAvailable = false;
}

ParsedFunction::~ParsedFunction ()
{
	try
	{
		if (m_FuncNode != NULL)
		{
			delete m_FuncNode;
			m_FuncNode = NULL;
		}
		if (m_ParameterValues != NULL)
		{
			delete[] m_ParameterValues;
			m_ParameterValues = NULL;
		}
		if (m_SimplifiedNode != NULL)
		{
			delete m_SimplifiedNode;
			m_SimplifiedNode = NULL;
		}
	}
	catch(...){}
}

bool ParsedFunction::GetCaseSensitive (FuncParserErrorData & ED)
{
 	ED.Clear();
    return m_CaseSensitive;
}

void ParsedFunction::SetCaseSensitive (bool p_CaseSensitive, FuncParserErrorData & ED)
{
	const std::string ERROR_SOURCE = "ParsedFunction::SetCaseSensitive";
	ED.Clear();

	try
	{
	 	if (p_CaseSensitive == m_CaseSensitive)
	 		return;
	    m_CaseSensitive = p_CaseSensitive;
	    if (m_CaseSensitive)
	    	ResetParsedState();
	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}
}

const ParsedFunction::StringVector & ParsedFunction::GetVariableNames (FuncParserErrorData & ED) const
{
	ED.Clear();
    return m_VariableNames;
}

void ParsedFunction::SetVariableNames (const StringVector & p_VariableNames, FuncParserErrorData & ED)
{
	ED.Clear();
	const std::string ERROR_SOURCE = "ParsedFunction::SetVariableNames";

	try
	{
	    m_VariableNames = p_VariableNames;
		ResetParsedState();
	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}
}

const ParsedFunction::StringVector & ParsedFunction::GetParameterNames (FuncParserErrorData & ED) const
{
	ED.Clear();
    return m_ParameterNames;
}

void ParsedFunction::SetParameterNames (const StringVector & p_ParameterNames, FuncParserErrorData & ED)
{
	ED.Clear();
	const std::string ERROR_SOURCE = "ParsedFunction::SetParameterNames";

	try
	{
	    m_ParameterNames=p_ParameterNames;
	    m_ParametersAvailable = (m_ParameterNames.size()>0);
		ResetParsedState();
		m_FixedParameters.clear();
	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}
}

const ParsedFunction::DoubleVector ParsedFunction::GetParameterValues (FuncParserErrorData & ED) const
{
	ED.Clear();
	const std::string ERROR_SOURCE = "ParsedFunction::GetParameterValues";
    DoubleVector ParamValues;

	try
	{
        size_t i;

	    if (m_ParameterValues == NULL)
	    	throw FuncParserErrorData(FuncParserErrorData::err_BADCALL, ERROR_SOURCE,
	    	                          "Parameter values are not set");

	    for (i=0; i<m_ParameterNames.size(); i++)
	    	ParamValues.push_back(m_ParameterValues[i]);
	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}

    return ParamValues;

}

void ParsedFunction::SetParameterValues (const DoubleVector & p_ParameterValues, FuncParserErrorData & ED)
{
	ED.Clear();
	const std::string ERROR_SOURCE = "ParsedFunction::SetParameterValues";

	try
	{
        size_t i;

	    if (p_ParameterValues.size() != m_ParameterNames.size())
	    	throw FuncParserErrorData(FuncParserErrorData::err_BADARG, ERROR_SOURCE,
	    	                          "Number of parameter values differs from the number of parameter names");

		ResetParsedState(true);

		if (m_ParameterValues)
		{
			delete[] m_ParameterValues;
			m_ParameterValues = NULL;
		}
		m_ParameterValues = new double[p_ParameterValues.size()];
		for(i=0; i<p_ParameterValues.size(); i++)
			m_ParameterValues[i] = p_ParameterValues[i];
	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}

}

bool ParsedFunction::GetSimplifyParametersAllowed (FuncParserErrorData & ED)
{
	ED.Clear();
    return m_SimplifyParametersAllowed;
}

void ParsedFunction::SetSimplifyParametersAllowed (bool p_SimplifyParametersAllowed, FuncParserErrorData & ED)
{
	ED.Clear();
    m_SimplifyParametersAllowed=p_SimplifyParametersAllowed;
}

bool ParsedFunction::GetLogicOperatorsAllowed (FuncParserErrorData & ED)
{
	ED.Clear();
    return m_LogicOperatorsAllowed;
}

void ParsedFunction::SetLogicOperatorsAllowed (bool p_LogicOperatorsAllowed, FuncParserErrorData & ED)
{
	const std::string ERROR_SOURCE = "ParsedFunction::SetLogicOperatorsAllowed";
	ED.Clear();

	try
	{
	 	if (p_LogicOperatorsAllowed == m_LogicOperatorsAllowed)
	 		return;

	    m_LogicOperatorsAllowed = p_LogicOperatorsAllowed;

	    if (!m_LogicOperatorsAllowed)
	    	ResetParsedState();
	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}
}

bool ParsedFunction::GetLogicalNumericMixAllowed (FuncParserErrorData & ED)
{
	ED.Clear();
    return m_LogicalNumericMixAllowed;
}

void ParsedFunction::SetLogicalNumericMixAllowed (bool p_LogicalNumericMixAllowed, FuncParserErrorData & ED)
{
	const std::string ERROR_SOURCE = "ParsedFunction::SetLogicOperatorsAllowed";
	ED.Clear();

	try
	{
	 	if (p_LogicalNumericMixAllowed == m_LogicalNumericMixAllowed)
	 		return;

	    m_LogicalNumericMixAllowed = p_LogicalNumericMixAllowed;

	    if (m_LogicOperatorsAllowed && !m_LogicalNumericMixAllowed)
	    	ResetParsedState();
	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}
}

double ParsedFunction::GetComparisonTolerance (FuncParserErrorData & ED)
{
	ED.Clear();
    return m_ComparisonTolerance;
}

void ParsedFunction::SetComparisonTolerance (double p_ComparisonTolerance, FuncParserErrorData & ED)
{
	const std::string ERROR_SOURCE = "ParsedFunction::SetComparisonTolerance";
	ED.Clear();

	try
	{
		if (m_ComparisonTolerance == p_ComparisonTolerance)
			return;

		if (p_ComparisonTolerance < 0.0)
	    	throw FuncParserErrorData(FuncParserErrorData::err_BADARG, ERROR_SOURCE,
	    	                          "Tolerance may not be negative");

		m_ComparisonTolerance = p_ComparisonTolerance;

		//reset Simplified state (due to some comparison results might have been changed with the new tolerance)
		ResetParsedState(true);
	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}
}

const std::string ParsedFunction::GetStringToParse (FuncParserErrorData & ED) const
{
	ED.Clear();
    return m_StringToParse;
}

void ParsedFunction::SetStringToParse (const std::string & p_StringToParse, FuncParserErrorData & ED)
{
	ED.Clear();
	const std::string ERROR_SOURCE = "ParsedFunction::SetStringToParse";

	try
	{
		if (p_StringToParse == m_StringToParse)
			return;
		m_StringToParse = p_StringToParse;
		ResetParsedState();
	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}
}

void ParsedFunction::Parse (FuncParserErrorData & ED)
{
	ED.Clear();
	const std::string ERROR_SOURCE = "ParsedFunction::Parse";

	try
	{
	    if (m_Parsed) return;

		FuncParser funcParser;

	    m_FuncNode = funcParser.Parse(m_StringToParse, m_VariableNames, m_ParameterNames,
	                                  m_CaseSensitive, m_LogicOperatorsAllowed, 
									  m_ComparisonTolerance, m_LogicalNumericMixAllowed,
	                                  ED);
		if (ED.GetNumber() != FuncParserErrorData::err_OK)
			return;

		m_Parsed = true;
	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}
}

double ParsedFunction::CalcExpression (const DoubleVector & Arg, FuncParserErrorData & ED)
{
	ED.Clear();
	const std::string ERROR_SOURCE = "ParsedFunction::CalcExpression";

	double * dArgs = NULL;
	double dValue = Math::GetNaN();

	try
	{
        size_t i;

		//check arguments size
		if (Arg.size() != m_VariableNames.size())
			throw FuncParserErrorData(FuncParserErrorData::err_BADARG, ERROR_SOURCE,
	    	                          "Number of variable values differs from the number of variable names");

		//check if Parameter values are available
		if (m_ParameterNames.size() > 0)
			if (m_ParameterValues == NULL)
				throw FuncParserErrorData(FuncParserErrorData::err_BADARG, ERROR_SOURCE,
		    	                          "Parameter values are not set");

		if(!m_Parsed) //that is also checked in <Parse> itself. The check is done here
		              //once more just to prevent an additional function call (time!!)
		{
			Parse(ED);
			if (ED.GetNumber() != FuncParserErrorData::err_OK)
				return dValue;
		}

		dArgs = new double [Arg.size()];
		for(i=0; i<Arg.size(); i++)
		{
			dArgs[i] = Arg[i];
		}

		if (!m_ParametersSimplified) //that is also checked in <SimplifyParameters> itself. The check is done here
		                             //once more just to prevent an additional function call (time!!)
		{
			SimplifyParameters(ED);
			if (ED.GetNumber() != FuncParserErrorData::err_OK)
				throw FuncParserErrorData(ED);
		}

		dValue = m_SimplifiedNode->CalcNodeValue(dArgs, m_ParameterValues, m_ComparisonTolerance);

		delete[] dArgs;
		dArgs = NULL;

	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
		if (dArgs != NULL)
		{
			delete[] dArgs;
			dArgs = NULL;
		}
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
		if (dArgs != NULL)
		{
			delete[] dArgs;
			dArgs = NULL;
		}
	}

	return dValue;
}

std::string ParsedFunction::GetXMLString (FuncParserErrorData & ED, bool InSimplifiedState /*= true*/, const std::string & pContainerNodeName /*= "Rate"*/)
{
	const std::string ERROR_SOURCE = "ParsedFunction::GetXMLString";
	ED.Clear();

	std::string XMLString;

	try
	{
		if(!m_Parsed)
		{
			Parse(ED);
			if (ED.GetNumber() != FuncParserErrorData::err_OK)
				throw FuncParserErrorData(ED);
		}

		std::string RateString;
		FuncNode * Node2Expand = NULL;

		if (InSimplifiedState)
		{
			SimplifyParameters(ED);
			if (ED.GetNumber() != FuncParserErrorData::err_OK)
				throw FuncParserErrorData(ED);

			Node2Expand = m_SimplifiedNode;
		}
		else
			Node2Expand = m_FuncNode;

		assert(Node2Expand!=NULL);

		RateString = Node2Expand->XMLString(m_VariableNames, m_ParameterNames, ED);
		if (ED.GetNumber() != FuncParserErrorData::err_OK)
			throw FuncParserErrorData(ED);

		std::string StringToParseAttr;
		if ((m_StringToParse.find_first_of("<") == std::string::npos) &&
		    (m_StringToParse.find_first_of(">") == std::string::npos) &&
		    (m_StringToParse.find_first_of("&") == std::string::npos) &&
		    (m_StringToParse.find_first_of("'") == std::string::npos) &&
		    (m_StringToParse.find_first_of("\"") == std::string::npos))
			StringToParseAttr = " StringToParse=\"" + m_StringToParse + "\" ";
		else
			StringToParseAttr = "";
		XMLString = "<" + pContainerNodeName + StringToParseAttr + ">" +
					RateString + "</" + pContainerNodeName+">";

	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}

	return XMLString;
}

double ParsedFunction::CalcExpression (double * dArgs, FuncParserErrorData & ED)
{
	ED.Clear();
	const std::string ERROR_SOURCE = "ParsedFunction::CalcExpression";

	double dValue = Math::GetNaN();

	try
	{

		//check if Parameter values are available
		if (m_ParametersAvailable)
			if (m_ParameterValues == NULL)
				throw FuncParserErrorData(FuncParserErrorData::err_BADARG, ERROR_SOURCE,
		    	                          "Parameter values are not set");

		if(!m_Parsed) //that is also checked in <Parse> itself. The check is done here
		              //once more just to prevent an additional function call (time!!)
		{
			Parse(ED);
			if (ED.GetNumber() != FuncParserErrorData::err_OK)
				return dValue;
		}

		if (!m_ParametersSimplified) //that is also checked in <SimplifyParameters> itself. The check is done here
		                             //once more just to prevent a function call (time!!)
		{
			SimplifyParameters(ED);
			if (ED.GetNumber() != FuncParserErrorData::err_OK)
				throw FuncParserErrorData(ED);
		}

		dValue = m_SimplifiedNode->CalcNodeValue(dArgs, m_ParameterValues, m_ComparisonTolerance);
	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}

	return dValue;
}

void ParsedFunction::SetParametersNotToSimplify (const StringVector & p_ParameterNames, FuncParserErrorData & ED)
{
	ED.Clear();
	const std::string ERROR_SOURCE = "ParsedFunction::SetParametersNotToSimplify";

	try
	{
		m_FixedParameters = p_ParameterNames;
		ResetParsedState(true);
	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}
}

const ParsedFunction::StringVector & ParsedFunction::GetParametersNotToSimplify (FuncParserErrorData & ED) const
{
	ED.Clear();
	return m_FixedParameters;
}

DimensionInfo ParsedFunction::GetDimensionInfoFor(const std::string & formula,
												  const std::vector<QuantityDimensionInfo> & quantityDimensions,
								                  FuncParserErrorData & ED)
{
	ED.Clear();
	const char * ERROR_SOURCE = "ParsedFunction::GetDimensionInfoFor";

	DimensionInfo dimensionInfo;

	try
	{
		std::vector<std::string> variableNames;
		std::vector<std::string> parameterNames = ExtractQuantityNames(quantityDimensions);

		SetCaseSensitive(true, ED);
		if (ED.GetNumber() != FuncParserErrorData::err_OK) throw ED;

		SetLogicalNumericMixAllowed(true, ED);
		if (ED.GetNumber() != FuncParserErrorData::err_OK) throw ED;

		SetLogicOperatorsAllowed(true, ED);
		if (ED.GetNumber() != FuncParserErrorData::err_OK) throw ED;

		//pass all quantities as parameters
		SetParameterNames(parameterNames, ED);
		if (ED.GetNumber() != FuncParserErrorData::err_OK) throw ED;

		//mark all parameters not for simplifying but allow simplifying itself,
		//so only arithmetic expression like 2*sin(pi/3)+2
		//will be simplified
		SetParametersNotToSimplify(parameterNames, ED);
		if (ED.GetNumber() != FuncParserErrorData::err_OK) throw ED;

		SetSimplifyParametersAllowed(false, ED);
		if (ED.GetNumber() != FuncParserErrorData::err_OK) throw ED;

		SetStringToParse(formula, ED);
		if (ED.GetNumber() != FuncParserErrorData::err_OK) throw ED;

		Parse(ED);
		if (ED.GetNumber() != FuncParserErrorData::err_OK) throw ED;

		SimplifyParameters(ED);
		if (ED.GetNumber() != FuncParserErrorData::err_OK) throw ED;

		//pass dimension infos to the expression tree nodes
		for (unsigned int idx=0; idx<quantityDimensions.size(); idx++)
			m_SimplifiedNode->SetDimensionInfo(quantityDimensions[idx], 
			                                   variableNames, parameterNames);

		//finally get dimension for the formula
		dimensionInfo = m_SimplifiedNode->CalcNodeDimensionInfo();
	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
		ED.SetDescription(ED_.GetDescription()+" (Formula: "+formula+")");
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error (Formula: "+formula+")");
	}

	return dimensionInfo;
}

std::vector<std::string> ParsedFunction::ExtractQuantityNames(const std::vector<QuantityDimensionInfo> & quantityDimensions)
{
	std::vector<std::string> quantityNames;

	for(unsigned int i=0; i<quantityDimensions.size(); i++)
		quantityNames.push_back(quantityDimensions[i].GetQuantityName());

	return quantityNames;
}


}//.. end "namespace FuncParserNative"
