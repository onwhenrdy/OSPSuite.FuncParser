#ifdef WIN32_PRODUCTION
#pragma managed(push,off)
#endif

#include "FuncParser/FuncNode.h"
#include "FuncParser/FuncParserErrorData.h"
#include "FuncParser/ElemFunction.h"
#include "FuncParser/FuncParser.h"
#include <deque>
#include "FuncParser/ElemFunctions.h"

#ifdef WIN32_PRODUCTION
#pragma managed(pop)
#endif

#ifdef WIN32
#pragma warning( disable : 4996)
#endif

#ifdef linux
#include <cstring>
#endif

namespace FuncParserNative
{

FuncNode::FuncNode ()
{
	m_NodeType = NT_INVALID;
	m_NodeFunction = NULL;
	m_FirstOperand = NULL;
	m_SecondOperand = NULL;
	m_BranchCondition = NULL;
	m_NodeValue = Math::GetNaN();
	m_VarOrParamIndex = VAR_INVALID_INDEX;
	m_VariableArgument = NULL;
}

FuncNode::FuncNode (const FuncNode * SourceNode)
{
	//copy without subtrees!!
	m_NodeType = SourceNode->GetNodeType();
	m_NodeFunction = (m_NodeType == NT_FUNCTION) ? SourceNode->GetNodeFunction() : NULL;
	m_NodeValue = (m_NodeType == NT_CONST) ? SourceNode->GetNodeValue() : Math::GetNaN();
	m_VarOrParamIndex = ((m_NodeType == NT_VARIABLE) || (m_NodeType == NT_PARAMETER)) ? SourceNode->GetVarOrParamIndex() : VAR_INVALID_INDEX;
	m_FirstOperand = NULL;
	m_SecondOperand = NULL;
	m_BranchCondition = NULL;
	m_VariableArgument = NULL;
}

FuncNode::~FuncNode ()
{
	try
	{
		if(m_FirstOperand != NULL)
		{
			delete m_FirstOperand;
			m_FirstOperand = NULL;
		}
		if(m_SecondOperand != NULL)
		{
			delete m_SecondOperand;
			m_SecondOperand = NULL;
		}
		if(m_BranchCondition != NULL)
		{
			delete m_BranchCondition;
			m_BranchCondition = NULL;
		}
		if(m_VariableArgument != NULL)
		{
			delete m_VariableArgument;
			m_VariableArgument = NULL;
		}
	}
	catch(...){}
}

const int FuncNode::GetVarOrParamIndex () const
{
    assert ((m_NodeType == NT_VARIABLE) || (m_NodeType == NT_PARAMETER));

    return m_VarOrParamIndex;
}

void FuncNode::SetVarOrParamIndex (int p_NodeVariableIndex)
{
    assert ((m_NodeType == NT_VARIABLE) || (m_NodeType == NT_PARAMETER));

    m_VarOrParamIndex=p_NodeVariableIndex;
}

FuncNode * FuncNode::GetFirstOperand () const
{
    assert (m_NodeType != NT_INVALID);

    return m_FirstOperand;
}

void FuncNode::SetFirstOperand (FuncNode * p_FirstOperand)
{
    assert (m_NodeType != NT_INVALID);

    m_FirstOperand=p_FirstOperand;
}

FuncNode * FuncNode::GetSecondOperand () const
{
    assert (m_NodeType != NT_INVALID);

    return m_SecondOperand;
}

void FuncNode::SetSecondOperand (FuncNode * p_SecondOperand)
{
    assert (m_NodeType != NT_INVALID);

    m_SecondOperand=p_SecondOperand;
}

FuncNode * FuncNode::GetBranchCondition ()
{
    assert (m_NodeType != NT_INVALID);

    return m_BranchCondition;
}

void FuncNode::SetBranchCondition (FuncNode * p_BranchCondition)
{
    assert (m_NodeType != NT_INVALID);

    m_BranchCondition=p_BranchCondition;
}

FuncNode * FuncNode::GetVariableArgument () const
{
    return m_VariableArgument;
}

void FuncNode::SetVariableArgument (FuncNode * p_VariableArgument)
{
    m_VariableArgument=p_VariableArgument;
}

const double FuncNode::GetNodeValue () const
{
	assert (m_NodeType == NT_CONST);

    return m_NodeValue;
}

void FuncNode::SetNodeValue (double p_NodeValue)
{
 	assert (m_NodeType != NT_INVALID);

    if (m_NodeType != NT_CONST)
    	throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME, "FuncNode::SetNodeValue",
    	                          "Cannot set constant value into non-constant node");
    m_NodeValue = p_NodeValue;
}

ElemFunction * FuncNode::GetNodeFunction () const
{
    assert(m_NodeType == NT_FUNCTION);

    return m_NodeFunction;
}

void FuncNode::SetNodeFunction (ElemFunction * p_NodeFunction)
{
    assert(m_NodeType == NT_FUNCTION);

    m_NodeFunction=p_NodeFunction;
}

const FuncNode::FuncNodeType FuncNode::GetNodeType () const
{
    return m_NodeType;
}

void FuncNode::SetNodeType (FuncNodeType p_NodeType)
{
	if (p_NodeType == m_NodeType)
		return; //nothing to do

    m_NodeType = p_NodeType;

    //reset all properties
    if (m_FirstOperand != NULL)
    {
    	delete m_FirstOperand;
    	m_FirstOperand = NULL;
    }
    if (m_SecondOperand != NULL)
    {
    	delete m_SecondOperand;
    	m_SecondOperand = NULL;
    }
    if (m_BranchCondition != NULL)
    {
    	delete m_BranchCondition;
    	m_BranchCondition = NULL;
    }
    if(m_VariableArgument != NULL)
    {
    	delete m_VariableArgument;
    	m_VariableArgument = NULL;
    }

    m_NodeFunction = NULL;
    m_NodeValue = Math::GetNaN();
    m_VarOrParamIndex  = VAR_INVALID_INDEX;
}

FuncNode * FuncNode::Clone ()
{
	assert (m_NodeType != NT_INVALID);

	FuncNode * newNode = new FuncNode(this); //copies all simple properties
	if(m_FirstOperand != NULL)
		newNode->SetFirstOperand(m_FirstOperand->Clone());
	if(m_SecondOperand != NULL)
		newNode->SetSecondOperand(m_SecondOperand->Clone());
	if(m_BranchCondition != NULL)
		newNode->SetBranchCondition(m_BranchCondition->Clone());

	if(m_VariableArgument != NULL)
		newNode->SetVariableArgument(m_VariableArgument->Clone());

	return newNode;
}

const double FuncNode::CalcNodeValue (double * Args, double * Params, double ComparisonTolerance)
{
	assert (m_NodeType != NT_INVALID);

	if (m_NodeType == NT_CONST)
	{
		return m_NodeValue;
	}
	if (m_NodeType == NT_PARAMETER)
	{
		assert(Params != NULL);
		return Params[this->GetVarOrParamIndex()];
	}
	if (m_NodeType == NT_VARIABLE)
	{
		if (m_VariableArgument != NULL)
			throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME,"FuncNode::CalcNodeValue",
		                             "Functions containing variable with argument cannot be evaluated");

		return Args[this->GetVarOrParamIndex()];
	}
	if (m_NodeType == NT_FUNCTION)
	{
		double FirstOperand = 0.0, SecondOperand = 0.0;

		if (m_BranchCondition != NULL)
		{
			double BranchValue = m_BranchCondition->CalcNodeValue(Args, Params, ComparisonTolerance);
			if (BranchValue == 1)
			{
				assert(m_FirstOperand!=NULL);
				return m_FirstOperand->CalcNodeValue(Args, Params, ComparisonTolerance);
			}
			else
			{
				assert(m_SecondOperand!=NULL);
				return m_SecondOperand->CalcNodeValue(Args, Params, ComparisonTolerance);
			}
		}

		if(m_FirstOperand!=NULL)
			FirstOperand = m_FirstOperand->CalcNodeValue(Args, Params, ComparisonTolerance);

		if (m_SecondOperand != NULL)
			SecondOperand = m_SecondOperand->CalcNodeValue(Args, Params, ComparisonTolerance);

		assert (m_NodeFunction != NULL);
		return m_NodeFunction->Eval(FirstOperand, SecondOperand, ComparisonTolerance);
	}

	throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME,"FuncNode::CalcNodeValue",
	                          "Node has unknown node type");

	return 0; //just to avoid compile warning
}

bool FuncNode::SimplifyNode (double & NewValue, const DoubleVector & FixedParametersIndexList, double ComparisonTolerance, double * ParameterValues /*= NULL*/)
{
	const std::string ERROR_SOURCE = "FuncNode::SimplifyNode";

	try
	{
		double FirstOpValue, SecondOpValue = 0.0;
		double VarArgValue;
		bool FirstNodeSimplified, SecondNodeSimplified;
        size_t i;

		const ElemFunctions * ElemFuncs = ElemFunctions::GetInstance();

		if (m_NodeType == NT_VARIABLE)
		{
			//simplify argument node if available
			if (m_VariableArgument != NULL)
				m_VariableArgument->SimplifyNode(VarArgValue, FixedParametersIndexList, ComparisonTolerance, ParameterValues);

			//variable node cannot be simplified
			return false;
		}

		if (m_NodeType == NT_CONST)
		{
			NewValue = m_NodeValue;
			return true;
		}

		if (m_NodeType == NT_PARAMETER)
		{
			if (ParameterValues == NULL) //No parameter VALUES given - so simplifying without parameters
				return false;

			//check if parameter may be simplified (is not fixed)
			for(i=0; i<FixedParametersIndexList.size(); i++)
				if (m_VarOrParamIndex == FixedParametersIndexList[i])
					return false;

			NewValue = ParameterValues[GetVarOrParamIndex()];
			//change node type to const
			m_NodeType = NT_CONST;
			m_NodeValue = NewValue;
			return true;
		}

		if (m_NodeType == NT_FUNCTION)
		{
			if (m_NodeFunction->IsRandomFunction())
				return false; //random function cannot be simplified

			assert(m_FirstOperand != NULL);

			FirstNodeSimplified = m_FirstOperand->SimplifyNode(FirstOpValue, FixedParametersIndexList,
			                                                   ComparisonTolerance, ParameterValues);
			if (m_SecondOperand != NULL)
				SecondNodeSimplified = m_SecondOperand->SimplifyNode(SecondOpValue, FixedParametersIndexList,
				                                                     ComparisonTolerance, ParameterValues);
			else
				SecondNodeSimplified = true;

			if (m_BranchCondition != NULL)
			{
				double BranchConditionValue;
				if (!m_BranchCondition->SimplifyNode(BranchConditionValue, FixedParametersIndexList,
			                                         ComparisonTolerance, ParameterValues))
			    	return false; //branch condition could not be simplified ==> cannot simplify node

			    //branch condition could be simplified
			    //So check if the corresponding sub node (1st or 2nd) was simplified
			    if (BranchConditionValue == 1)
			    {
			    	if (FirstNodeSimplified)
			    	{
			    		NewValue = FirstOpValue;
			    		this->SetNodeType(NT_CONST); //Resets all other properties, deletes Subtrees, etc.
			    		this->SetNodeValue(NewValue);
			    		return true;
			    	}
			    }
			    else
			    {
			    	if (SecondNodeSimplified)
			    	{
			    		assert(m_SecondOperand != NULL);
			    		NewValue = SecondOpValue;
			    		this->SetNodeType(NT_CONST); //Resets all other properties, deletes Subtrees, etc.
			    		this->SetNodeValue(NewValue);
			    		return true;
			    	}
			    }

			    return false; //node could not be simplified
			}

			if (FirstNodeSimplified && SecondNodeSimplified)
			{
				//best case: node can be completely simplified
				assert(m_NodeFunction!=NULL);
				NewValue = m_NodeFunction->Eval(FirstOpValue, SecondOpValue, ComparisonTolerance);
				this->SetNodeType(NT_CONST); //Resets all other properties, deletes Subtrees, etc.
				this->SetNodeValue(NewValue);
				return true;
			}

			//all further simplifications apply only to binary functions
			//so exit if we have any unary operation (return false - no simplification possible)
			if (m_SecondOperand==NULL)
				return false;

			//replace x/a with x*(1/a) and return false (node could not be simplified)
			if (m_NodeFunction->GetType() == ElemFunction::EF_DIV)
				if (SecondNodeSimplified)
				{
					m_SecondOperand->SetNodeValue(1.0 / m_SecondOperand->GetNodeValue());
					m_NodeFunction = (*ElemFuncs)[ElemFunction::EF_MUL];
					return false;
				}
			else
				return false;

			//replace x-a with x+(-a) and return false (node could not be simplified)
			if (m_NodeFunction->GetType() == ElemFunction::EF_MINUS)
				if (SecondNodeSimplified)
				{
					m_SecondOperand->SetNodeValue(-m_SecondOperand->GetNodeValue());
					m_NodeFunction = (*ElemFuncs)[ElemFunction::EF_PLUS];
					return false;
				}
			else
				return false;

			//for sum/products
			if ((m_NodeFunction->GetType() == ElemFunction::EF_PLUS) ||
			    (m_NodeFunction->GetType() == ElemFunction::EF_MUL))
			{
				std::deque <FuncNode *> SimplifiedNodes, NotSimplifiedNodes;
				FuncNode * LastOpNode = this, * pNode = NULL;

				//find last function node with the same function
				while ((LastOpNode->GetSecondOperand()->GetNodeType() == NT_FUNCTION) &&
				       (LastOpNode->GetSecondOperand()->GetNodeFunction()->GetType() == m_NodeFunction->GetType()))
				{
					LastOpNode = LastOpNode->GetSecondOperand();
				}

				//save pointers to the simplified/not simplified nodes in 2 vectors
				for(pNode = this; pNode != LastOpNode->GetSecondOperand(); pNode = pNode->GetSecondOperand())
				{
					if (pNode->GetFirstOperand()->GetNodeType() == NT_CONST)
						SimplifiedNodes.push_back(pNode->GetFirstOperand());
					else
						NotSimplifiedNodes.push_back(pNode->GetFirstOperand());
				}
				if (LastOpNode->GetSecondOperand()->GetNodeType() == NT_CONST)
					SimplifiedNodes.push_back(LastOpNode->GetSecondOperand());
				else
					NotSimplifiedNodes.push_back(LastOpNode->GetSecondOperand());

				//if <=1 of the nodes could be simplified - nothing to do, return false (node not simplified)
				if (SimplifiedNodes.size()<=1)
					return false;

				//now rearrange first operand nodes from current node to the <
				for(pNode = this; pNode != LastOpNode->GetSecondOperand(); pNode = pNode->GetSecondOperand())
				{
					//first, set the operands from not simplified nodes and then from simplified ones
					//so a+X+b+Y becomes X+Y+a+b
					if (!NotSimplifiedNodes.empty())
					{
						pNode->SetFirstOperand(*(NotSimplifiedNodes.begin()));
						NotSimplifiedNodes.pop_front();
					}
					else
					{
						assert(!SimplifiedNodes.empty());
						pNode->SetFirstOperand(*(SimplifiedNodes.begin()));
						SimplifiedNodes.pop_front();
					}
				}
				//at least one simplified node exists, so the last node pointer is saved in the queue of SIMPLIFIED nodes
				LastOpNode->SetSecondOperand(*(SimplifiedNodes.begin()));
				SimplifiedNodes.pop_front();

				//both queues MUST be empty now
				assert(SimplifiedNodes.empty() && NotSimplifiedNodes.empty());

				//my first operand may not be simplified (except it was an error in this algorithm)
				assert(m_FirstOperand->GetNodeType() != NT_CONST);

				//now find first node with both sub nodes simplified, simplified it, return false
				for(pNode = m_SecondOperand; pNode != LastOpNode->GetSecondOperand(); pNode = pNode->GetSecondOperand())
				{
					if (pNode->GetFirstOperand()->GetNodeType() == NT_CONST)
					{
						//both operands of the node must be simplified (const)
						assert(pNode->GetSecondOperand()->GetNodeType() == NT_CONST);
						pNode->SimplifyNode(SecondOpValue, FixedParametersIndexList, ComparisonTolerance, ParameterValues);
						return false;
					}
				}

				//previous for-loop must exit the function
				assert(false);
			}

			//in all other cases - node could not be simplified anymore - just return false
			return false;
		}
	}
	catch(...)
	{
		throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}

	return false; //just to avoid compile warning
}

bool FuncNode::IsNumericNode (FuncParserErrorData & ED)
{
	const std::string ERROR_SOURCE = "FuncNode::IsNumericNode";
	const std::string conERRIllegalMix = "Illegal mixture of numeric and logical operations";

	try
	{
		bool FirstOpNumeric, SecondOpNumeric=true;

		assert(m_NodeType!=NT_INVALID);

		if ((m_NodeType == NT_CONST) || (m_NodeType == NT_VARIABLE) || (m_NodeType == NT_PARAMETER))
			return true;

		assert(m_NodeType==NT_FUNCTION);
		assert(m_NodeFunction!=NULL);

		if (m_NodeFunction->IsRandomFunction())
			return true; //RND or SRND functions - always numeric

		assert(m_FirstOperand!=NULL);

		if (m_BranchCondition != NULL)
			return false; //IF .. THEN .. ELSE statement

		if(!m_NodeFunction->IsUnary() && (m_SecondOperand == NULL))
			throw FuncParserErrorData(FuncParserErrorData::err_PARSE, ERROR_SOURCE,
				                      "Binary function \""+m_NodeFunction->GetFuncString()+"\": second operand is missing");

		FirstOpNumeric = m_FirstOperand->IsNumericNode(ED);
		if (ED.GetNumber() != FuncParserErrorData::err_OK)
			throw FuncParserErrorData(ED);
		if (m_SecondOperand != NULL)
		{
			SecondOpNumeric = m_SecondOperand->IsNumericNode(ED);
			if (ED.GetNumber() != FuncParserErrorData::err_OK)
				throw FuncParserErrorData(ED);
		}

		if (!m_NodeFunction->IsLogical())               //node function is some numeric one
			if (FirstOpNumeric && SecondOpNumeric)
				return true;
			else
				throw FuncParserErrorData(FuncParserErrorData::err_PARSE, ERROR_SOURCE, conERRIllegalMix);

		//otherwise: node function is logical
		//return value is than always false (Logical) or ERROR is raised
		if (
		    //NOT: Logical <==> FirstOp is logical
		    ((m_NodeFunction->GetType() == ElemFunction::EF_NOT) && !FirstOpNumeric) ||
		    //AND,OR: Logical <==> FirstOp and SecondOp are both logical
		    (((m_NodeFunction->GetType() == ElemFunction::EF_AND) ||
		      (m_NodeFunction->GetType() == ElemFunction::EF_OR)) && !FirstOpNumeric && !SecondOpNumeric) ||
		    //otherwise: comparison operators (<,<=,>,>=,=,!=(<>))
		    //Logical <==> FirstOp and SecondOp are both numeric(!)
		    (FirstOpNumeric && SecondOpNumeric)
		   )
			return false;
		else
			throw FuncParserErrorData(FuncParserErrorData::err_PARSE, ERROR_SOURCE, conERRIllegalMix);

	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}

	//just to avoid compiler warning
	return true;
}

std::string FuncNode::XMLString (const std::vector < std::string > & VariableNames, const std::vector < std::string > & ParamNames, FuncParserErrorData & ED)
{
	std::string RateString;

	const char * ERROR_SOURCE = "FuncNode::XMLNode";

	try
	{
		assert(m_NodeType!=NT_INVALID);

		if (m_NodeType == NT_CONST)
		{
			RateString = "<Constant>"+Double2Str(m_NodeValue)+"</Constant>";
		}

		else if (m_NodeType == NT_PARAMETER)
		{
            assert(m_VarOrParamIndex<(long)ParamNames.size());
			RateString = "<Parameter>"+ParamNames[this->GetVarOrParamIndex()]+"</Parameter>";
		}

		else if (m_NodeType == NT_VARIABLE)
		{
            assert(m_VarOrParamIndex<(long)VariableNames.size());

			if (m_VariableArgument == NULL)
			{
				//simple variable
				RateString = "<Variable>"+VariableNames[this->GetVarOrParamIndex()]+"</Variable>";
			}
			else
			{
				//variable with argument
				RateString = "<VariableWithArgument>";
				RateString += "<Variable>"+VariableNames[this->GetVarOrParamIndex()]+"</Variable>";

				RateString += "<Argument>" + m_VariableArgument->XMLString(VariableNames, ParamNames, ED) + "</Argument>";
				if (ED.GetNumber() != FuncParserErrorData::err_OK)
					return "";

				RateString += "</VariableWithArgument>";
			}
		}

		else if (m_NodeType == NT_FUNCTION)
		{
			//If .. then ..else
			if (m_BranchCondition != NULL)
			{
				assert (m_FirstOperand != NULL);
				assert (m_SecondOperand != NULL);

				RateString = "<IF>";

				RateString += "<IfStatement>" + m_BranchCondition->XMLString(VariableNames, ParamNames, ED) + "</IfStatement>";
				if (ED.GetNumber() != FuncParserErrorData::err_OK)
					return "";

				RateString += "<ThenStatement>" + m_FirstOperand->XMLString(VariableNames, ParamNames, ED) + "</ThenStatement>";
				if (ED.GetNumber() != FuncParserErrorData::err_OK)
					return "";

				RateString += "<ElseStatement>" + m_SecondOperand->XMLString(VariableNames, ParamNames, ED) + "</ElseStatement>";
				if (ED.GetNumber() != FuncParserErrorData::err_OK)
					return "";

				RateString += "</IF>";

				return RateString;
			}

			assert(m_NodeFunction!=NULL);

			const char * conSimpleProduct = "SimpleProduct";
			char NodeName[100];
			strcpy(NodeName, "");

			if ((m_NodeFunction->GetType() == ElemFunction::EF_PLUS) ||
			    (m_NodeFunction->GetType() == ElemFunction::EF_MUL))
			{
				std::string SummandNode;
				FuncNode * Operand = this;
				std::deque <std::string> SubNodes;
				strcpy(NodeName, m_NodeFunction->GetType() == ElemFunction::EF_PLUS ?
					             "Sum" : "Product");

				//"Simple" product is a product node of the type k*Var_1*...*Var_n
				//where k is a constant and Var_i are variables
				bool SimpleProduct = (m_NodeFunction->GetType() == ElemFunction::EF_MUL) ? true : false;

				while(1)
				{
					assert(Operand!=NULL);
					if( (Operand->GetNodeType() != NT_FUNCTION) ||
					    (Operand->GetNodeFunction()->GetType() != m_NodeFunction->GetType()))
						break;

					//check if product (if so) is still "simple"
					if (SimpleProduct && !((Operand->GetFirstOperand()->GetNodeType() == NT_CONST) ||
										   (Operand->GetFirstOperand()->GetNodeType() == NT_VARIABLE)))
						SimpleProduct = false;

					if (Operand->GetFirstOperand()->GetVariableArgument() != NULL)
						SimpleProduct = false;  //Variable with argument - may not be a part of simple product

					SummandNode = Operand->GetFirstOperand()->XMLString(VariableNames,ParamNames,ED);
					if (ED.GetNumber() != FuncParserErrorData::err_OK)
						return "";
					SubNodes.push_back(SummandNode);

					Operand = Operand->GetSecondOperand();
				}

				//check if product (if so) is still "simple"
				if (SimpleProduct && !((Operand->GetNodeType() == NT_CONST) ||
									   (Operand->GetNodeType() == NT_VARIABLE)))
					SimpleProduct = false;

				if (Operand->GetVariableArgument() != NULL)
					SimpleProduct = false;  //Variable with argument - may not be a part of simple product

				SummandNode = Operand->XMLString(VariableNames,ParamNames,ED);
				if (ED.GetNumber() != FuncParserErrorData::err_OK)
					return "";
				SubNodes.push_back(SummandNode);

				if (SimpleProduct)
					strcpy(NodeName,conSimpleProduct);

				RateString = std::string("<")+NodeName+">";

				while (!SubNodes.empty())
				{
					RateString = RateString + *(SubNodes.begin());
					SubNodes.pop_front();
				}

			}
			else if ((m_NodeFunction->GetType() == ElemFunction::EF_MINUS) ||
			         (m_NodeFunction->GetType() == ElemFunction::EF_DIV)   ||
			         (m_NodeFunction->GetType() == ElemFunction::EF_POWER) ||
			         (m_NodeFunction->GetType() == ElemFunction::EF_POWER_F) ||
			         (m_NodeFunction->GetType() == ElemFunction::EF_MIN)   ||
			         (m_NodeFunction->GetType() == ElemFunction::EF_MAX) )
			{
				char FirstOpName[20], SecondOpName[20];
				if (m_NodeFunction->GetType() == ElemFunction::EF_MINUS)
				{
					strcpy(NodeName,"Diff");
					strcpy(FirstOpName,"Minuend");
					strcpy(SecondOpName,"Subtrahend");
				}
				else if (m_NodeFunction->GetType() == ElemFunction::EF_DIV)
				{
					strcpy(NodeName,"Div");
					strcpy(FirstOpName,"Numerator");
					strcpy(SecondOpName,"Denominator");
				}
				else if ((m_NodeFunction->GetType() == ElemFunction::EF_POWER) || (m_NodeFunction->GetType() == ElemFunction::EF_POWER_F))
				{
					strcpy(NodeName,"Power");
					strcpy(FirstOpName,"Base");
					strcpy(SecondOpName,"Exponent");
				}
				else if ((m_NodeFunction->GetType() == ElemFunction::EF_MIN) ||
				         (m_NodeFunction->GetType() == ElemFunction::EF_MAX) )
				{
					strcpy(NodeName, m_NodeFunction->GetType() == ElemFunction::EF_MIN ? "Min" : "Max");
					strcpy(FirstOpName,"FirstArgument");
					strcpy(SecondOpName,"SecondArgument");
				}

				std::string FirstOp = std::string("<")+FirstOpName+">";
				std::string SecondOp = std::string("<")+SecondOpName+">";
				std::string FirstOpRate = m_FirstOperand->XMLString(VariableNames, ParamNames, ED);
				if (ED.GetNumber() != FuncParserErrorData::err_OK)
					return "";
				std::string SecondOpRate = m_SecondOperand->XMLString(VariableNames, ParamNames, ED);
				if (ED.GetNumber() != FuncParserErrorData::err_OK)
					return "";
				FirstOp = FirstOp + FirstOpRate + "</" + FirstOpName + ">";
				SecondOp = SecondOp + SecondOpRate + "</" + SecondOpName + ">";

				RateString = std::string("<")+ NodeName +">"+ FirstOp + SecondOp;

			}
			else if (m_NodeFunction->IsLogical())
			{
				if ((m_NodeFunction->GetType() == ElemFunction::EF_LESS) ||
				    (m_NodeFunction->GetType() == ElemFunction::EF_LESS_F))
					strcpy(NodeName, "Less");
				else if ((m_NodeFunction->GetType() == ElemFunction::EF_LESSEQUAL) ||
				         (m_NodeFunction->GetType() == ElemFunction::EF_LESSEQUAL_F))
					strcpy(NodeName, "LessEqual");
				else if ((m_NodeFunction->GetType() == ElemFunction::EF_GREATER) ||
				         (m_NodeFunction->GetType() == ElemFunction::EF_GREATER_F))
					strcpy(NodeName, "Greater");
				else if ((m_NodeFunction->GetType() == ElemFunction::EF_GREATEREQUAL) ||
				         (m_NodeFunction->GetType() == ElemFunction::EF_GREATEREQUAL_F))
					strcpy(NodeName, "GreaterEqual");
				else if ((m_NodeFunction->GetType() == ElemFunction::EF_EQUAL) ||
				         (m_NodeFunction->GetType() == ElemFunction::EF_EQUAL_F))
					strcpy(NodeName, "Equal");
				else if ((m_NodeFunction->GetType() == ElemFunction::EF_UNEQUAL)  ||
				         (m_NodeFunction->GetType() == ElemFunction::EF_UNEQUAL2) ||
				         (m_NodeFunction->GetType() == ElemFunction::EF_UNEQUAL_F))
					strcpy(NodeName, "Unequal");
				else if (m_NodeFunction->GetType() == ElemFunction::EF_AND)
					strcpy(NodeName, "And");
				else if (m_NodeFunction->GetType() == ElemFunction::EF_OR)
					strcpy(NodeName, "Or");
				else if (m_NodeFunction->GetType() == ElemFunction::EF_NOT)
					strcpy(NodeName, "Not");

				if (strlen(NodeName) == 0)
					throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE,
 			                                  m_NodeFunction->GetFuncString() + " was not set into XML rate string");

				const char * FirstOpName = "FirstOperand", * SecondOpName = "SecondOperand";
				std::string FirstOp = std::string("<")+FirstOpName+">";
				std::string SecondOp = std::string("<")+SecondOpName+">";
				std::string FirstOpRate = m_FirstOperand->XMLString(VariableNames, ParamNames, ED);
				if (ED.GetNumber() != FuncParserErrorData::err_OK)
					return "";

				FirstOp = FirstOp + FirstOpRate + "</" + FirstOpName + ">";

				if (m_SecondOperand)
				{
					std::string SecondOpRate = m_SecondOperand->XMLString(VariableNames, ParamNames, ED);
					if (ED.GetNumber() != FuncParserErrorData::err_OK)
						return "";
					SecondOp = SecondOp + SecondOpRate + "</" + SecondOpName + ">";
				}
				else
					SecondOp = ""; //unary logical operator (e.g. NOT)

				RateString = std::string("<")+ NodeName +">"+ FirstOp + SecondOp;

			}
			else
			{
				//otherwise: some unary function
				assert(m_NodeFunction->IsUnary());

				strcpy(NodeName, StringHelper::Capitalize(m_NodeFunction->GetFuncString()).c_str());
				RateString = std::string("<")+NodeName+">";

				if (!m_NodeFunction->IsRandomFunction())
				{
					std::string RateFuncNode = m_FirstOperand->XMLString(VariableNames, ParamNames, ED);
					if (ED.GetNumber() != FuncParserErrorData::err_OK)
						return "";
					RateString = RateString + RateFuncNode;
				}
			}

			RateString = RateString + "</"+ NodeName +">";
		}

		else
			throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE,
			                          "Node has unknown node type");

	}
	catch(FuncParserErrorData & ED_)
	{
		ED = ED_;
	}
	catch(...)
	{
		ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
	}

	return RateString;
}

void FuncNode::SetDimensionInfo(const QuantityDimensionInfo & quantityDimensionInfo, 
								const std::vector < std::string > & VariableNames, 
								const std::vector < std::string > & ParameterNames)
{
	const char * ERROR_SOURCE = "FuncNode::SetDimensionInfo";

	switch(m_NodeType)
	{
	case NT_CONST:
		_dimensionInfo.Reset(); //unitless dimension
		break;
	case NT_VARIABLE:
		assert(m_VarOrParamIndex<(long)VariableNames.size());
		if (VariableNames[m_VarOrParamIndex] == quantityDimensionInfo.GetQuantityName())
			_dimensionInfo = quantityDimensionInfo.GetDimensionInfo();
		break;
	case NT_PARAMETER:
		assert(m_VarOrParamIndex<(long)ParameterNames.size());
		if (ParameterNames[m_VarOrParamIndex] == quantityDimensionInfo.GetQuantityName())
			_dimensionInfo = quantityDimensionInfo.GetDimensionInfo();
		break;
	case NT_FUNCTION:
		//---- special case: Random functions
		if (m_NodeFunction->IsRandomFunction())
			return; //nothing to do

		assert(m_FirstOperand!=NULL);
		m_FirstOperand->SetDimensionInfo(quantityDimensionInfo, 
										 VariableNames, 
										 ParameterNames);

		if (m_SecondOperand)
			m_SecondOperand->SetDimensionInfo(quantityDimensionInfo, 
										 VariableNames, 
										 ParameterNames);
		break;
	default:
		throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE,
											  "Node has unknown node type");
	}
}

DimensionInfo FuncNode::CalcNodeDimensionInfo()
{
	const char * ERROR_SOURCE = "FuncNode::CalcNodeDimensionInfo";
	DimensionInfo dimensionInfo, secondOperandDimensionInfo;
	double secondOperand;
	bool firstOperandConst,secondOperandConst;

	switch(m_NodeType)
	{
	case NT_CONST:
	case NT_VARIABLE:
	case NT_PARAMETER:
		return _dimensionInfo;
		break;
	case NT_FUNCTION:

		//---- special case: If-Function
		if (m_BranchCondition != NULL)
		{
			if (m_BranchCondition->GetNodeType() != NT_CONST)
			{
				//check if both operands have the same dimension. Error if not
				assert(m_FirstOperand!=NULL);
				assert(m_SecondOperand!=NULL);

				DimensionInfo dim1, dim2;
				dim1 = m_FirstOperand->CalcNodeDimensionInfo();
				dim2 = m_SecondOperand->CalcNodeDimensionInfo();

				if (dim1 == dim2)
					return dim1;

				//---- dimensions differ. Then, check if one of the operands is constant
				//in that case, return dimension of another operand
				//
				//for example, "Time>0 ? P1 : 0 " will return the dimension of P1

				if (m_FirstOperand->GetNodeType() == NT_CONST)
					return dim2;
				if (m_SecondOperand->GetNodeType() == NT_CONST)
					return dim1;

				//none of above apply=>Error, dimensions are inconsistent
				throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, ERROR_SOURCE,
					"Cannot determine units: IF-function has non-const condition and operands dimensions differ");
			}

			double BranchValue = m_BranchCondition->GetNodeValue();
			if (BranchValue == 1)
			{
				assert(m_FirstOperand!=NULL);
				return m_FirstOperand->CalcNodeDimensionInfo();
			}
			else
			{
				assert(m_SecondOperand!=NULL);
				return m_SecondOperand->CalcNodeDimensionInfo();
			}
		}

		//---- special case: random functions
		if (m_NodeFunction->IsRandomFunction())
			return dimensionInfo;

		//---- all other functions
		assert(m_FirstOperand!=NULL);
		dimensionInfo = m_FirstOperand->CalcNodeDimensionInfo();

		//the VALUE of 2nd operand only required by the power function
		secondOperand=Math::GetNaN();

		firstOperandConst  = (m_FirstOperand->GetNodeType()==NT_CONST);
		secondOperandConst = true;

		if (m_SecondOperand)
		{
			secondOperandConst = (m_SecondOperand->GetNodeType()==NT_CONST);

			if ((m_NodeFunction->GetType() == ElemFunction::EF_POWER) || (m_NodeFunction->GetType() == ElemFunction::EF_POWER_F))
			{
				if (!secondOperandConst && 
					!m_SecondOperand->CalcNodeDimensionInfo().IsDimensionless())
					throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, ERROR_SOURCE,
					      "Cannot determine units: POWER-function has non-const exponent");

				if (m_FirstOperand->CalcNodeDimensionInfo().IsDimensionless())
					secondOperand = 0;
				else
				{
					if (secondOperandConst)
						secondOperand=m_SecondOperand->GetNodeValue();
					else
					{
						throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, ERROR_SOURCE,
						      "Cannot determine units: cannot calculate exponent of a POWER-function");

						//TODO if the formula is X^P1, where P1 is a dimensionless parameter and X non-dimensionless,
						// we need the value of P1 for getting the dimension.
						//The exception above must be replaced in this case by the code line below:
                        //secondOperand = m_SecondOperand->CalcNodeValue(NULL, paramValues, 0.0)
					}
				}

			}

			//the dimension info of the second operand only required for binary functions
			secondOperandDimensionInfo = m_SecondOperand->CalcNodeDimensionInfo();
		}
		else
		{
			//unary function - <secondOperandDimensionInfo> passed to 
			//dimensionInfo.TransformBy will not be used
		}

		dimensionInfo.TransformBy(secondOperandDimensionInfo,
		                          m_NodeFunction, secondOperand,
								  firstOperandConst, secondOperandConst);

		return dimensionInfo;

	default:
		throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE,
											  "Node has unknown node type");
	}	
}

}//.. end "namespace FuncParserNative"
