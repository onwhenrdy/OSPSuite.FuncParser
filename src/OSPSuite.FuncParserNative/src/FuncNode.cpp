#include "FuncParser/FuncNode.h"
#include "FuncParser/FuncParserErrorData.h"
#include "FuncParser/ElemFunction.h"
#include "FuncParser/FuncParser.h"
#include <deque>
#include "FuncParser/ElemFunctions.h"

#ifdef _WINDOWS
#pragma warning( disable : 4996)
#endif

#ifdef linux
#include <cstring>
#endif

namespace FuncParserNative
{

   FuncNode::FuncNode()
   {
      _nodeType = NT_INVALID;
      _nodeFunction = NULL;
      _firstOperand = NULL;
      _secondOperand = NULL;
      _branchCondition = NULL;
      _nodeValue = Math::GetNaN();
      _varOrParamIndex = VAR_INVALID_INDEX;
      _variableArgument = NULL;
   }

   FuncNode::FuncNode(const FuncNode* SourceNode)
   {
      //copy without subtrees!!
      _nodeType = SourceNode->GetNodeType();
      _nodeFunction = (_nodeType == NT_FUNCTION) ? SourceNode->GetNodeFunction() : NULL;
      _nodeValue = (_nodeType == NT_CONST) ? SourceNode->GetNodeValue() : Math::GetNaN();
      _varOrParamIndex = ((_nodeType == NT_VARIABLE) || (_nodeType == NT_PARAMETER)) ? SourceNode->GetVarOrParamIndex() : VAR_INVALID_INDEX;
      _firstOperand = NULL;
      _secondOperand = NULL;
      _branchCondition = NULL;
      _variableArgument = NULL;
   }

   FuncNode::~FuncNode()
   {
      try
      {
         if (_firstOperand != NULL)
         {
            delete _firstOperand;
            _firstOperand = NULL;
         }
         if (_secondOperand != NULL)
         {
            delete _secondOperand;
            _secondOperand = NULL;
         }
         if (_branchCondition != NULL)
         {
            delete _branchCondition;
            _branchCondition = NULL;
         }
         if (_variableArgument != NULL)
         {
            delete _variableArgument;
            _variableArgument = NULL;
         }
      }
      catch (...) {}
   }

   const int FuncNode::GetVarOrParamIndex() const
   {
      assert((_nodeType == NT_VARIABLE) || (_nodeType == NT_PARAMETER));

      return _varOrParamIndex;
   }

   void FuncNode::SetVarOrParamIndex(int nodeVariableIndex)
   {
      assert((_nodeType == NT_VARIABLE) || (_nodeType == NT_PARAMETER));

      _varOrParamIndex = nodeVariableIndex;
   }

   FuncNode* FuncNode::GetFirstOperand() const
   {
      assert(_nodeType != NT_INVALID);

      return _firstOperand;
   }

   void FuncNode::SetFirstOperand(FuncNode* firstOperand)
   {
      assert(_nodeType != NT_INVALID);

      _firstOperand = firstOperand;
   }

   FuncNode* FuncNode::GetSecondOperand() const
   {
      assert(_nodeType != NT_INVALID);

      return _secondOperand;
   }

   void FuncNode::SetSecondOperand(FuncNode* secondOperand)
   {
      assert(_nodeType != NT_INVALID);

      _secondOperand = secondOperand;
   }

   FuncNode* FuncNode::GetBranchCondition()
   {
      assert(_nodeType != NT_INVALID);

      return _branchCondition;
   }

   void FuncNode::SetBranchCondition(FuncNode* branchCondition)
   {
      assert(_nodeType != NT_INVALID);

      _branchCondition = branchCondition;
   }

   FuncNode* FuncNode::GetVariableArgument() const
   {
      return _variableArgument;
   }

   void FuncNode::SetVariableArgument(FuncNode* variableArgument)
   {
      _variableArgument = variableArgument;
   }

   const double FuncNode::GetNodeValue() const
   {
      assert(_nodeType == NT_CONST);

      return _nodeValue;
   }

   void FuncNode::SetNodeValue(double nodeValue)
   {
      assert(_nodeType != NT_INVALID);

      if (_nodeType != NT_CONST)
         throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME, "FuncNode::SetNodeValue",
            "Cannot set constant value into non-constant node");
      _nodeValue = nodeValue;
   }

   ElemFunction* FuncNode::GetNodeFunction() const
   {
      assert(_nodeType == NT_FUNCTION);

      return _nodeFunction;
   }

   void FuncNode::SetNodeFunction(ElemFunction* nodeFunction)
   {
      assert(_nodeType == NT_FUNCTION);

      _nodeFunction = nodeFunction;
   }

   const FuncNode::FuncNodeType FuncNode::GetNodeType() const
   {
      return _nodeType;
   }

   void FuncNode::SetNodeType(FuncNodeType nodeType)
   {
      if (nodeType == _nodeType)
         return; //nothing to do

      _nodeType = nodeType;

      //reset all properties
      if (_firstOperand != NULL)
      {
         delete _firstOperand;
         _firstOperand = NULL;
      }
      if (_secondOperand != NULL)
      {
         delete _secondOperand;
         _secondOperand = NULL;
      }
      if (_branchCondition != NULL)
      {
         delete _branchCondition;
         _branchCondition = NULL;
      }
      if (_variableArgument != NULL)
      {
         delete _variableArgument;
         _variableArgument = NULL;
      }

      _nodeFunction = NULL;
      _nodeValue = Math::GetNaN();
      _varOrParamIndex = VAR_INVALID_INDEX;
   }

   FuncNode* FuncNode::Clone()
   {
      assert(_nodeType != NT_INVALID);

      FuncNode* newNode = new FuncNode(this); //copies all simple properties
      if (_firstOperand != NULL)
         newNode->SetFirstOperand(_firstOperand->Clone());
      if (_secondOperand != NULL)
         newNode->SetSecondOperand(_secondOperand->Clone());
      if (_branchCondition != NULL)
         newNode->SetBranchCondition(_branchCondition->Clone());

      if (_variableArgument != NULL)
         newNode->SetVariableArgument(_variableArgument->Clone());

      return newNode;
   }

   const double FuncNode::CalcNodeValue(const double *Args,
                                        double *Params,
                                        double ComparisonTolerance)
   {
      assert(_nodeType != NT_INVALID);

      if (_nodeType == NT_CONST)
      {
         return _nodeValue;
      }
      if (_nodeType == NT_PARAMETER)
      {
         assert(Params != NULL);
         return Params[this->GetVarOrParamIndex()];
      }
      if (_nodeType == NT_VARIABLE)
      {
         if (_variableArgument != NULL)
            throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME, "FuncNode::CalcNodeValue",
               "Functions containing variable with argument cannot be evaluated");

         return Args[this->GetVarOrParamIndex()];
      }
      if (_nodeType == NT_FUNCTION)
      {
         double FirstOperand = 0.0, SecondOperand = 0.0;

         if (_branchCondition != NULL)
         {
            double BranchValue = _branchCondition->CalcNodeValue(Args, Params, ComparisonTolerance);
            if (BranchValue == 1)
            {
               assert(_firstOperand != NULL);
               return _firstOperand->CalcNodeValue(Args, Params, ComparisonTolerance);
            }
            else
            {
               assert(_secondOperand != NULL);
               return _secondOperand->CalcNodeValue(Args, Params, ComparisonTolerance);
            }
         }

         if (_firstOperand != NULL)
            FirstOperand = _firstOperand->CalcNodeValue(Args, Params, ComparisonTolerance);

         if (_secondOperand != NULL)
            SecondOperand = _secondOperand->CalcNodeValue(Args, Params, ComparisonTolerance);

         assert(_nodeFunction != NULL);
         return _nodeFunction->Eval(FirstOperand, SecondOperand, ComparisonTolerance);
      }

      throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME, "FuncNode::CalcNodeValue",
         "Node has unknown node type");

      return 0; //just to avoid compile warning
   }

   bool FuncNode::SimplifyNode(double& NewValue, const IndexVector& FixedParametersIndexVector, double ComparisonTolerance, double* ParameterValues /*= NULL*/)
   {
      const std::string ERROR_SOURCE = "FuncNode::SimplifyNode";

      try
      {
         double FirstOpValue, SecondOpValue = 0.0;
         double VarArgValue;
         bool FirstNodeSimplified, SecondNodeSimplified;
         size_t i;

         const ElemFunctions* ElemFuncs = ElemFunctions::GetInstance();

         if (_nodeType == NT_VARIABLE)
         {
            //simplify argument node if available
            if (_variableArgument != NULL)
               _variableArgument->SimplifyNode(VarArgValue, FixedParametersIndexVector, ComparisonTolerance, ParameterValues);

            //variable node cannot be simplified
            return false;
         }

         if (_nodeType == NT_CONST)
         {
            NewValue = _nodeValue;
            return true;
         }

         if (_nodeType == NT_PARAMETER)
         {
            if (ParameterValues == NULL) //No parameter VALUES given - so simplifying without parameters
               return false;

            //check if parameter may be simplified (is not fixed)
            for (i = 0; i < FixedParametersIndexVector.size(); i++)
               if (_varOrParamIndex == FixedParametersIndexVector[i])
                  return false;

            NewValue = ParameterValues[GetVarOrParamIndex()];
            //change node type to const
            _nodeType = NT_CONST;
            _nodeValue = NewValue;
            return true;
         }

         if (_nodeType == NT_FUNCTION)
         {
            if (_nodeFunction->IsRandomFunction())
               return false; //random function cannot be simplified

            assert(_firstOperand != NULL);

            FirstNodeSimplified = _firstOperand->SimplifyNode(FirstOpValue, FixedParametersIndexVector,
               ComparisonTolerance, ParameterValues);
            if (_secondOperand != NULL)
               SecondNodeSimplified = _secondOperand->SimplifyNode(SecondOpValue, FixedParametersIndexVector,
                  ComparisonTolerance, ParameterValues);
            else
               SecondNodeSimplified = true;

            if (_branchCondition != NULL)
            {
               double BranchConditionValue;
               if (!_branchCondition->SimplifyNode(BranchConditionValue, FixedParametersIndexVector,
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
                     assert(_secondOperand != NULL);
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
               assert(_nodeFunction != NULL);
               NewValue = _nodeFunction->Eval(FirstOpValue, SecondOpValue, ComparisonTolerance);
               this->SetNodeType(NT_CONST); //Resets all other properties, deletes Subtrees, etc.
               this->SetNodeValue(NewValue);
               return true;
            }

            //all further simplifications apply only to binary functions
            //so exit if we have any unary operation (return false - no simplification possible)
            if (_secondOperand == NULL)
               return false;

            //replace x/a with x*(1/a) and return false (node could not be simplified)
            if (_nodeFunction->GetType() == ElemFunction::EF_DIV)
               if (SecondNodeSimplified)
               {
                  _secondOperand->SetNodeValue(1.0 / _secondOperand->GetNodeValue());
                  _nodeFunction = (*ElemFuncs)[ElemFunction::EF_MUL];
                  return false;
               }
               else
                  return false;

            //replace x-a with x+(-a) and return false (node could not be simplified)
            if (_nodeFunction->GetType() == ElemFunction::EF_MINUS)
               if (SecondNodeSimplified)
               {
                  _secondOperand->SetNodeValue(-_secondOperand->GetNodeValue());
                  _nodeFunction = (*ElemFuncs)[ElemFunction::EF_PLUS];
                  return false;
               }
               else
                  return false;

            //for sum/products
            if ((_nodeFunction->GetType() == ElemFunction::EF_PLUS) ||
               (_nodeFunction->GetType() == ElemFunction::EF_MUL))
            {
               std::deque <FuncNode*> SimplifiedNodes, NotSimplifiedNodes;
               FuncNode* LastOpNode = this, * pNode = NULL;

               //find last function node with the same function
               while ((LastOpNode->GetSecondOperand()->GetNodeType() == NT_FUNCTION) &&
                  (LastOpNode->GetSecondOperand()->GetNodeFunction()->GetType() == _nodeFunction->GetType()))
               {
                  LastOpNode = LastOpNode->GetSecondOperand();
               }

               //save pointers to the simplified/not simplified nodes in 2 vectors
               for (pNode = this; pNode != LastOpNode->GetSecondOperand(); pNode = pNode->GetSecondOperand())
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
               if (SimplifiedNodes.size() <= 1)
                  return false;

               //now rearrange first operand nodes from current node to the <
               for (pNode = this; pNode != LastOpNode->GetSecondOperand(); pNode = pNode->GetSecondOperand())
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
               assert(_firstOperand->GetNodeType() != NT_CONST);

               //now find first node with both sub nodes simplified, simplified it, return false
               for (pNode = _secondOperand; pNode != LastOpNode->GetSecondOperand(); pNode = pNode->GetSecondOperand())
               {
                  if (pNode->GetFirstOperand()->GetNodeType() == NT_CONST)
                  {
                     //both operands of the node must be simplified (const)
                     assert(pNode->GetSecondOperand()->GetNodeType() == NT_CONST);
                     pNode->SimplifyNode(SecondOpValue, FixedParametersIndexVector, ComparisonTolerance, ParameterValues);
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
      catch (...)
      {
         throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
      }

      return false; //just to avoid compile warning
   }

   bool FuncNode::IsNumericNode()
   {
      const std::string ERROR_SOURCE = "FuncNode::IsNumericNode";
      const std::string conERRIllegalMix = "Illegal mixture of numeric and logical operations";

      bool FirstOpNumeric, SecondOpNumeric = true;

      assert(_nodeType != NT_INVALID);

      if ((_nodeType == NT_CONST) || (_nodeType == NT_VARIABLE) || (_nodeType == NT_PARAMETER))
         return true;

      assert(_nodeType == NT_FUNCTION);
      assert(_nodeFunction != NULL);

      if (_nodeFunction->IsRandomFunction())
         return true; //RND or SRND functions - always numeric

      assert(_firstOperand != NULL);

      if (_branchCondition != NULL)
         return false; //IF .. THEN .. ELSE statement

      if (!_nodeFunction->IsUnary() && (_secondOperand == NULL))
         throw FuncParserErrorData(FuncParserErrorData::err_PARSE, ERROR_SOURCE,
            "Binary function \"" + _nodeFunction->GetFuncString() + "\": second operand is missing");

      FirstOpNumeric = _firstOperand->IsNumericNode();

      if (_secondOperand != NULL)
      {
         SecondOpNumeric = _secondOperand->IsNumericNode();
      }

      if (!_nodeFunction->IsLogical())               //node function is some numeric one
         if (FirstOpNumeric && SecondOpNumeric)
            return true;
         else
            throw FuncParserErrorData(FuncParserErrorData::err_PARSE, ERROR_SOURCE, conERRIllegalMix);

      //otherwise: node function is logical
      //return value is than always false (Logical) or ERROR is raised
      if (
         //NOT: Logical <==> FirstOp is logical
         ((_nodeFunction->GetType() == ElemFunction::EF_NOT) && !FirstOpNumeric) ||
         //AND,OR: Logical <==> FirstOp and SecondOp are both logical
         (((_nodeFunction->GetType() == ElemFunction::EF_AND) ||
         (_nodeFunction->GetType() == ElemFunction::EF_OR)) && !FirstOpNumeric && !SecondOpNumeric) ||
         //otherwise: comparison operators (<,<=,>,>=,=,!=(<>))
         //Logical <==> FirstOp and SecondOp are both numeric(!)
            (FirstOpNumeric && SecondOpNumeric)
         )
         return false;
      else
         throw FuncParserErrorData(FuncParserErrorData::err_PARSE, ERROR_SOURCE, conERRIllegalMix);
   }

   std::string FuncNode::XMLString(const std::vector < std::string >& VariableNames, const std::vector < std::string >& ParamNames)
   {
      std::string RateString;

      const char* ERROR_SOURCE = "FuncNode::XMLNode";

      assert(_nodeType != NT_INVALID);

      if (_nodeType == NT_CONST)
      {
         RateString = "<Constant>" + Double2Str(_nodeValue) + "</Constant>";
      }

      else if (_nodeType == NT_PARAMETER)
      {
         assert(_varOrParamIndex < (long)ParamNames.size());
         RateString = "<Parameter>" + ParamNames[this->GetVarOrParamIndex()] + "</Parameter>";
      }

      else if (_nodeType == NT_VARIABLE)
      {
         assert(_varOrParamIndex < (long)VariableNames.size());

         if (_variableArgument == NULL)
         {
            //simple variable
            RateString = "<Variable>" + VariableNames[this->GetVarOrParamIndex()] + "</Variable>";
         }
         else
         {
            //variable with argument
            RateString = "<VariableWithArgument>";
            RateString += "<Variable>" + VariableNames[this->GetVarOrParamIndex()] + "</Variable>";

            RateString += "<Argument>" + _variableArgument->XMLString(VariableNames, ParamNames) + "</Argument>";

            RateString += "</VariableWithArgument>";
         }
      }

      else if (_nodeType == NT_FUNCTION)
      {
         //If .. then ..else
         if (_branchCondition != NULL)
         {
            assert(_firstOperand != NULL);
            assert(_secondOperand != NULL);

            RateString = "<IF>";

            RateString += "<IfStatement>" + _branchCondition->XMLString(VariableNames, ParamNames) + "</IfStatement>";

            RateString += "<ThenStatement>" + _firstOperand->XMLString(VariableNames, ParamNames) + "</ThenStatement>";

            RateString += "<ElseStatement>" + _secondOperand->XMLString(VariableNames, ParamNames) + "</ElseStatement>";

            RateString += "</IF>";

            return RateString;
         }

         assert(_nodeFunction != NULL);

         const char* conSimpleProduct = "SimpleProduct";
         char NodeName[100];
         strcpy(NodeName, "");

         if ((_nodeFunction->GetType() == ElemFunction::EF_PLUS) ||
            (_nodeFunction->GetType() == ElemFunction::EF_MUL))
         {
            std::string SummandNode;
            FuncNode* Operand = this;
            std::deque <std::string> SubNodes;
            strcpy(NodeName, _nodeFunction->GetType() == ElemFunction::EF_PLUS ?
               "Sum" : "Product");

            //"Simple" product is a product node of the type k*Var_1*...*Var_n
            //where k is a constant and Var_i are variables
            bool SimpleProduct = (_nodeFunction->GetType() == ElemFunction::EF_MUL) ? true : false;

            while (1)
            {
               assert(Operand != NULL);
               if ((Operand->GetNodeType() != NT_FUNCTION) ||
                  (Operand->GetNodeFunction()->GetType() != _nodeFunction->GetType()))
                  break;

               //check if product (if so) is still "simple"
               if (SimpleProduct && !((Operand->GetFirstOperand()->GetNodeType() == NT_CONST) ||
                  (Operand->GetFirstOperand()->GetNodeType() == NT_VARIABLE)))
                  SimpleProduct = false;

               if (Operand->GetFirstOperand()->GetVariableArgument() != NULL)
                  SimpleProduct = false;  //Variable with argument - may not be a part of simple product

               SummandNode = Operand->GetFirstOperand()->XMLString(VariableNames, ParamNames);
               SubNodes.push_back(SummandNode);

               Operand = Operand->GetSecondOperand();
            }

            //check if product (if so) is still "simple"
            if (SimpleProduct && !((Operand->GetNodeType() == NT_CONST) ||
               (Operand->GetNodeType() == NT_VARIABLE)))
               SimpleProduct = false;

            if (Operand->GetVariableArgument() != NULL)
               SimpleProduct = false;  //Variable with argument - may not be a part of simple product

            SummandNode = Operand->XMLString(VariableNames, ParamNames);
            SubNodes.push_back(SummandNode);

            if (SimpleProduct)
               strcpy(NodeName, conSimpleProduct);

            RateString = std::string("<") + NodeName + ">";

            while (!SubNodes.empty())
            {
               RateString = RateString + *(SubNodes.begin());
               SubNodes.pop_front();
            }

         }
         else if ((_nodeFunction->GetType() == ElemFunction::EF_MINUS) ||
            (_nodeFunction->GetType() == ElemFunction::EF_DIV) ||
            (_nodeFunction->GetType() == ElemFunction::EF_POWER) ||
            (_nodeFunction->GetType() == ElemFunction::EF_POWER_F) ||
            (_nodeFunction->GetType() == ElemFunction::EF_MIN) ||
            (_nodeFunction->GetType() == ElemFunction::EF_MAX))
         {
            char FirstOpName[20], SecondOpName[20];
            if (_nodeFunction->GetType() == ElemFunction::EF_MINUS)
            {
               strcpy(NodeName, "Diff");
               strcpy(FirstOpName, "Minuend");
               strcpy(SecondOpName, "Subtrahend");
            }
            else if (_nodeFunction->GetType() == ElemFunction::EF_DIV)
            {
               strcpy(NodeName, "Div");
               strcpy(FirstOpName, "Numerator");
               strcpy(SecondOpName, "Denominator");
            }
            else if ((_nodeFunction->GetType() == ElemFunction::EF_POWER) || (_nodeFunction->GetType() == ElemFunction::EF_POWER_F))
            {
               strcpy(NodeName, "Power");
               strcpy(FirstOpName, "Base");
               strcpy(SecondOpName, "Exponent");
            }
            else if ((_nodeFunction->GetType() == ElemFunction::EF_MIN) ||
               (_nodeFunction->GetType() == ElemFunction::EF_MAX))
            {
               strcpy(NodeName, _nodeFunction->GetType() == ElemFunction::EF_MIN ? "Min" : "Max");
               strcpy(FirstOpName, "FirstArgument");
               strcpy(SecondOpName, "SecondArgument");
            }

            std::string FirstOp = std::string("<") + FirstOpName + ">";
            std::string SecondOp = std::string("<") + SecondOpName + ">";
            std::string FirstOpRate = _firstOperand->XMLString(VariableNames, ParamNames);
            std::string SecondOpRate = _secondOperand->XMLString(VariableNames, ParamNames);
            FirstOp = FirstOp + FirstOpRate + "</" + FirstOpName + ">";
            SecondOp = SecondOp + SecondOpRate + "</" + SecondOpName + ">";

            RateString = std::string("<") + NodeName + ">" + FirstOp + SecondOp;

         }
         else if (_nodeFunction->IsLogical())
         {
            if ((_nodeFunction->GetType() == ElemFunction::EF_LESS) ||
               (_nodeFunction->GetType() == ElemFunction::EF_LESS_F))
               strcpy(NodeName, "Less");
            else if ((_nodeFunction->GetType() == ElemFunction::EF_LESSEQUAL) ||
               (_nodeFunction->GetType() == ElemFunction::EF_LESSEQUAL_F))
               strcpy(NodeName, "LessEqual");
            else if ((_nodeFunction->GetType() == ElemFunction::EF_GREATER) ||
               (_nodeFunction->GetType() == ElemFunction::EF_GREATER_F))
               strcpy(NodeName, "Greater");
            else if ((_nodeFunction->GetType() == ElemFunction::EF_GREATEREQUAL) ||
               (_nodeFunction->GetType() == ElemFunction::EF_GREATEREQUAL_F))
               strcpy(NodeName, "GreaterEqual");
            else if ((_nodeFunction->GetType() == ElemFunction::EF_EQUAL) ||
               (_nodeFunction->GetType() == ElemFunction::EF_EQUAL_F))
               strcpy(NodeName, "Equal");
            else if ((_nodeFunction->GetType() == ElemFunction::EF_UNEQUAL) ||
               (_nodeFunction->GetType() == ElemFunction::EF_UNEQUAL2) ||
               (_nodeFunction->GetType() == ElemFunction::EF_UNEQUAL_F))
               strcpy(NodeName, "Unequal");
            else if (_nodeFunction->GetType() == ElemFunction::EF_AND)
               strcpy(NodeName, "And");
            else if (_nodeFunction->GetType() == ElemFunction::EF_OR)
               strcpy(NodeName, "Or");
            else if (_nodeFunction->GetType() == ElemFunction::EF_NOT)
               strcpy(NodeName, "Not");

            if (strlen(NodeName) == 0)
               throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE,
                  _nodeFunction->GetFuncString() + " was not set into XML rate string");

            const char* FirstOpName = "FirstOperand", * SecondOpName = "SecondOperand";
            std::string FirstOp = std::string("<") + FirstOpName + ">";
            std::string SecondOp = std::string("<") + SecondOpName + ">";
            std::string FirstOpRate = _firstOperand->XMLString(VariableNames, ParamNames);

            FirstOp = FirstOp + FirstOpRate + "</" + FirstOpName + ">";

            if (_secondOperand)
            {
               std::string SecondOpRate = _secondOperand->XMLString(VariableNames, ParamNames);
               SecondOp = SecondOp + SecondOpRate + "</" + SecondOpName + ">";
            }
            else
               SecondOp = ""; //unary logical operator (e.g. NOT)

            RateString = std::string("<") + NodeName + ">" + FirstOp + SecondOp;

         }
         else
         {
            //otherwise: some unary function
            assert(_nodeFunction->IsUnary());

            strcpy(NodeName, StringHelper::Capitalize(_nodeFunction->GetFuncString()).c_str());
            RateString = std::string("<") + NodeName + ">";

            if (!_nodeFunction->IsRandomFunction())
            {
               std::string RateFuncNode = _firstOperand->XMLString(VariableNames, ParamNames);
               RateString = RateString + RateFuncNode;
            }
         }

         RateString = RateString + "</" + NodeName + ">";
      }

      else
         throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE,
            "Node has unknown node type");

      return RateString;
   }

   void FuncNode::SetDimensionInfo(const QuantityDimensionInfo& quantityDimensionInfo,
      const std::vector < std::string >& VariableNames,
      const std::vector < std::string >& ParameterNames)
   {
      const char* ERROR_SOURCE = "FuncNode::SetDimensionInfo";

      switch (_nodeType)
      {
      case NT_CONST:
         _dimensionInfo.Reset(); //unitless dimension
         break;
      case NT_VARIABLE:
         assert(_varOrParamIndex < (long)VariableNames.size());
         if (VariableNames[_varOrParamIndex] == quantityDimensionInfo.GetQuantityName())
            _dimensionInfo = quantityDimensionInfo.GetDimensionInfo();
         break;
      case NT_PARAMETER:
         assert(_varOrParamIndex < (long)ParameterNames.size());
         if (ParameterNames[_varOrParamIndex] == quantityDimensionInfo.GetQuantityName())
            _dimensionInfo = quantityDimensionInfo.GetDimensionInfo();
         break;
      case NT_FUNCTION:
         //---- special case: Random functions
         if (_nodeFunction->IsRandomFunction())
            return; //nothing to do

         assert(_firstOperand != NULL);
         _firstOperand->SetDimensionInfo(quantityDimensionInfo,
            VariableNames,
            ParameterNames);

         if (_secondOperand)
            _secondOperand->SetDimensionInfo(quantityDimensionInfo,
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
      const char* ERROR_SOURCE = "FuncNode::CalcNodeDimensionInfo";
      DimensionInfo dimensionInfo, secondOperandDimensionInfo;
      double secondOperand;
      bool firstOperandConst, secondOperandConst;

      switch (_nodeType)
      {
      case NT_CONST:
      case NT_VARIABLE:
      case NT_PARAMETER:
         return _dimensionInfo;
         break;
      case NT_FUNCTION:

         //---- special case: If-Function
         if (_branchCondition != NULL)
         {
            if (_branchCondition->GetNodeType() != NT_CONST)
            {
               //check if both operands have the same dimension. Error if not
               assert(_firstOperand != NULL);
               assert(_secondOperand != NULL);

               DimensionInfo dim1, dim2;
               dim1 = _firstOperand->CalcNodeDimensionInfo();
               dim2 = _secondOperand->CalcNodeDimensionInfo();

               if (dim1 == dim2)
                  return dim1;

               //---- dimensions differ. Then, check if one of the operands is constant
               //in that case, return dimension of another operand
               //
               //for example, "Time>0 ? P1 : 0 " will return the dimension of P1

               if (_firstOperand->GetNodeType() == NT_CONST)
                  return dim2;
               if (_secondOperand->GetNodeType() == NT_CONST)
                  return dim1;

               //none of above apply=>Error, dimensions are inconsistent
               throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, ERROR_SOURCE,
                  "Cannot determine units: IF-function has non-const condition and operands dimensions differ");
            }

            double BranchValue = _branchCondition->GetNodeValue();
            if (BranchValue == 1)
            {
               assert(_firstOperand != NULL);
               return _firstOperand->CalcNodeDimensionInfo();
            }
            else
            {
               assert(_secondOperand != NULL);
               return _secondOperand->CalcNodeDimensionInfo();
            }
         }

         //---- special case: random functions
         if (_nodeFunction->IsRandomFunction())
            return dimensionInfo;

         //---- all other functions
         assert(_firstOperand != NULL);
         dimensionInfo = _firstOperand->CalcNodeDimensionInfo();

         //the VALUE of 2nd operand only required by the power function
         secondOperand = Math::GetNaN();

         firstOperandConst = (_firstOperand->GetNodeType() == NT_CONST);
         secondOperandConst = true;

         if (_secondOperand)
         {
            secondOperandConst = (_secondOperand->GetNodeType() == NT_CONST);

            if ((_nodeFunction->GetType() == ElemFunction::EF_POWER) || (_nodeFunction->GetType() == ElemFunction::EF_POWER_F))
            {
               if (!secondOperandConst &&
                  !_secondOperand->CalcNodeDimensionInfo().IsDimensionless())
                  throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, ERROR_SOURCE,
                     "Cannot determine units: POWER-function has non-const exponent");

               if (_firstOperand->CalcNodeDimensionInfo().IsDimensionless())
                  secondOperand = 0;
               else
               {
                  if (secondOperandConst)
                     secondOperand = _secondOperand->GetNodeValue();
                  else
                  {
                     throw FuncParserErrorData(FuncParserErrorData::err_CANNOTCALC_DIMENSION, ERROR_SOURCE,
                        "Cannot determine units: cannot calculate exponent of a POWER-function");

                     //TODO if the formula is X^P1, where P1 is a dimensionless parameter and X non-dimensionless,
                     // we need the value of P1 for getting the dimension.
                     //The exception above must be replaced in this case by the code line below:
                           //secondOperand = _secondOperand->CalcNodeValue(NULL, paramValues, 0.0)
                  }
               }

            }

            //the dimension info of the second operand only required for binary functions
            secondOperandDimensionInfo = _secondOperand->CalcNodeDimensionInfo();
         }
         else
         {
            //unary function - <secondOperandDimensionInfo> passed to 
            //dimensionInfo.TransformBy will not be used
         }

         dimensionInfo.TransformBy(secondOperandDimensionInfo,
            _nodeFunction, secondOperand,
            firstOperandConst, secondOperandConst);

         return dimensionInfo;

      default:
         throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE,
            "Node has unknown node type");
      }
   }

}//.. end "namespace FuncParserNative"
