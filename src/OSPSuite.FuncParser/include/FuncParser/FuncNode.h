#ifndef _FuncNode_H_
#define _FuncNode_H_

#include <string>
#include "FuncParser/Math.h"
#include "FuncParser/FuncParserTypeDefs.h"
#include "FuncParser/StringHelper.h"
#include "FuncParser/DimensionInfo.h"
#include <sstream>
#include "FuncParser/QuantityDimensionInfo.h"

namespace FuncParserNative
{

class ElemFunction;
class FuncParserErrorData;

class FuncNode
{	
	public:
		enum FuncNodeType 
		{
			NT_INVALID = 0,
			NT_FUNCTION = 1,
			NT_VARIABLE,
			NT_CONST,
			NT_PARAMETER
		};
	
	private:
		typedef std::vector < double > DoubleVector ;
		int m_VarOrParamIndex;
		FuncNode * m_FirstOperand;
		FuncNode * m_SecondOperand;
		FuncNode * m_BranchCondition;
		double m_NodeValue;
		ElemFunction * m_NodeFunction;
		FuncNodeType m_NodeType;
		FuncNode * m_VariableArgument;
		inline std::string Double2Str (double dValue);

		DimensionInfo _dimensionInfo;
	
	public:
		FuncNode ();
		FuncNode (const FuncNode * SourceNode);
		~FuncNode ();
		const int GetVarOrParamIndex () const;
		void SetVarOrParamIndex (int p_NodeVariableIndex);
		FuncNode * GetFirstOperand () const;
		void SetFirstOperand (FuncNode * p_FirstOperand);
		FuncNode * GetSecondOperand () const;
		void SetSecondOperand (FuncNode * p_SecondOperand);
		FuncNode * GetBranchCondition ();
		void SetBranchCondition (FuncNode * p_BranchCondition);
		FuncNode * GetVariableArgument () const;
		void SetVariableArgument (FuncNode * p_VariableArgument);
		const double GetNodeValue () const;
		void SetNodeValue (double p_NodeValue);
		ElemFunction * GetNodeFunction () const;
		void SetNodeFunction (ElemFunction * p_NodeFunction);
		const FuncNodeType GetNodeType () const;
		void SetNodeType (FuncNodeType p_NodeType);
		FuncNode * Clone ();
		const double CalcNodeValue (double * Args, double * Params, double ComparisonTolerance);
		bool SimplifyNode (double & NewValue, const DoubleVector & FixedParametersIndexList, double ComparisonTolerance, double * ParameterValues = NULL);
		bool IsNumericNode (FuncParserErrorData & ED);
		std::string XMLString (const std::vector < std::string > & VariableNames, const std::vector < std::string > & ParamNames, FuncParserErrorData & ED);

		void SetDimensionInfo(const QuantityDimensionInfo & quantityDimensionInfo,
 							  const std::vector < std::string > & VariableNames, 
							  const std::vector < std::string > & ParameterNames);
		DimensionInfo CalcNodeDimensionInfo();
};

std::string FuncNode::Double2Str (double dValue)
{
	std::ostringstream out;
	out.precision(16);
	out<<dValue;
	
	return out.str();
}

}//.. end "namespace FuncParserNative"


#endif //_FuncNode_H_

