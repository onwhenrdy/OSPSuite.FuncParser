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
		int _varOrParamIndex;
		FuncNode * _firstOperand;
		FuncNode * _secondOperand;
		FuncNode * _branchCondition;
		double _nodeValue;
		ElemFunction * _nodeFunction;
		FuncNodeType _nodeType;
		FuncNode * _variableArgument;
		inline std::string Double2Str (double dValue);

		DimensionInfo _dimensionInfo;
	
	public:
		FuncNode ();
		FuncNode (const FuncNode * SourceNode);
		~FuncNode ();
		const int GetVarOrParamIndex () const;
		void SetVarOrParamIndex (int nodeVariableIndex);
		FuncNode * GetFirstOperand () const;
		void SetFirstOperand (FuncNode * firstOperand);
		FuncNode * GetSecondOperand () const;
		void SetSecondOperand (FuncNode * secondOperand);
		FuncNode * GetBranchCondition ();
		void SetBranchCondition (FuncNode * branchCondition);
		FuncNode * GetVariableArgument () const;
		void SetVariableArgument (FuncNode * variableArgument);
		const double GetNodeValue () const;
		void SetNodeValue (double nodeValue);
		ElemFunction * GetNodeFunction () const;
		void SetNodeFunction (ElemFunction * nodeFunction);
		const FuncNodeType GetNodeType () const;
		void SetNodeType (FuncNodeType nodeType);
		FuncNode * Clone ();
		const double CalcNodeValue (double * Args, double * Params, double ComparisonTolerance);
		bool SimplifyNode (double & NewValue, const IndexVector & FixedParametersIndexVector, double ComparisonTolerance, double * ParameterValues = NULL);
		bool IsNumericNode ();
		std::string XMLString (const std::vector < std::string > & VariableNames, const std::vector < std::string > & ParamNames);

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

