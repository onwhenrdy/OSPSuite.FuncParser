#include "FuncParser/FuncParserErrorData.h"
#include "FuncParser/ParsedFunction.h"
#include "FuncParser/Math.h"
#include "FuncParser/FuncParser.h"
#include "FuncParser/StringHelper.h"
#include "FuncParser/PInvokeHelper.h"

namespace FuncParserNative
{
   using namespace std;

   void ParsedFunction::ResetParsedState(bool SimplifiedStateOnly /*= false*/)
   {
      _parametersSimplified = false;
      if (_simplifiedNode)
      {
          _simplifiedNode = nullptr;
      }
      if (SimplifiedStateOnly)
         return;

      _parsed = false;
      if (_funcNode)
      {
          _funcNode = nullptr;
      }
   }

   IndexVector ParsedFunction::GetFixedParametersIndexVector(void)
   {
      IndexVector indexVector;
      // estimate size to prevent too many allocations
      indexVector.reserve(_fixedParameters.size() / 2);

      for (auto i = 0; i < _fixedParameters.size(); i++)
      {
          for (auto Index = 0; Index < _parameterNames.size(); Index++)
          {
              if ((_caseSensitive && (_fixedParameters[i] == _parameterNames[Index]))
                  || (!_caseSensitive
                      && (FuncParser::ToUpper(_fixedParameters[i])
                          == FuncParser::ToUpper(_parameterNames[Index]))))
              {
                  //check if fixed parameter name was already set (caller has used some parameter name twice or more
                  //In this case: don't raise an error, just ignore it
                  auto bAlreadyUsed = false;
                  for (auto k = 0; k < indexVector.size(); k++)
                  {
                      if (indexVector[k] == Index)
                      {
                          bAlreadyUsed = true;
                          break;
                      }
                  }
                  if (bAlreadyUsed)
                      break;

                  //Append an index of the fixed variable to the index list
                  indexVector.push_back(Index);

                  break;
              }
          }
      }

      return indexVector;
   }

   void ParsedFunction::SimplifyParameters()
   {
      IndexVector indexVector;
      double Dummy;

      if (!_parametersSimplified)
      {
          _simplifiedNode.reset(_funcNode->Clone());
          if (_simplifyParametersAllowed)
          {
              //get the indices of fixed parameters first
              indexVector = GetFixedParametersIndexVector();

              //simplify WITH parameter values
              //if parameter values are not set by now, so NULL is passed as the last argument
              //Node simplifying routine checks for this and ignores parameter values in this case
              _simplifiedNode->SimplifyNode(Dummy,
                                            indexVector,
                                            _comparisonTolerance,
                                            _parameterValues.get());
          } else
          {
              //simplify WITHOUT parameter values
              _simplifiedNode->SimplifyNode(Dummy, indexVector, _comparisonTolerance);
          }
          _parametersSimplified = true;
      }
   }

   ParsedFunction::ParsedFunction()
       : _caseSensitive(true)
       , _logicOperatorsAllowed(true)
       , _logicalNumericMixAllowed(false)
       , _parametersSimplified(false)
       , _parsed(false)
       , _simplifyParametersAllowed(true)
       , _comparisonTolerance(0.0)
       , _parametersAvailable(false)
   {
   }

   bool ParsedFunction::GetCaseSensitive() const
   {
      return _caseSensitive;
   }

   void ParsedFunction::SetCaseSensitive(bool caseSensitive)
   {
      if (caseSensitive == _caseSensitive)
         return;

      _caseSensitive = caseSensitive;
      if (_caseSensitive)
         ResetParsedState();
   }

   const StringVector& ParsedFunction::GetVariableNames() const
   {
      return _variableNames;
   }

   void ParsedFunction::SetVariableNames(StringVector variableNames)
   {
       _variableNames = std::move(variableNames);
       ResetParsedState();
   }

   const StringVector& ParsedFunction::GetParameterNames() const
   {
      return _parameterNames;
   }

   void ParsedFunction::SetParameterNames(StringVector parameterNames)
   {
       _parameterNames = std::move(parameterNames);
       _parametersAvailable = (_parameterNames.size() > 0);
       ResetParsedState();
       _fixedParameters.clear();
   }

   DoubleVector ParsedFunction::GetParameterValues() const
   {
      DoubleVector parameterValues;

      if (_parameterValues == nullptr)
          return parameterValues;

      // only one allocation
      parameterValues.reserve(_parameterNames.size());
      for (auto i = 0; i < _parameterNames.size(); i++)
          parameterValues.push_back(_parameterValues[i]);

      return parameterValues;
   }

   void ParsedFunction::SetParameterValues(const DoubleVector& parameterValues)
   {
      const char* ERROR_SOURCE = "ParsedFunction::SetParameterValues";

      if (parameterValues.size() != _parameterNames.size())
         throw FuncParserErrorData(FuncParserErrorData::err_BADARG, ERROR_SOURCE,
            "Number of parameter values differs from the number of parameter names");

      ResetParsedState(true);
      if (_parameterValues)
      {
          _parameterValues = nullptr;
      }
      _parameterValues.reset(new double[parameterValues.size()]);
      for (auto i = 0; i < parameterValues.size(); i++)
          _parameterValues[i] = parameterValues[i];
   }

   bool ParsedFunction::GetSimplifyParametersAllowed()
   {
      return _simplifyParametersAllowed;
   }

   void ParsedFunction::SetSimplifyParametersAllowed(bool simplifyParametersAllowed)
   {
      _simplifyParametersAllowed = simplifyParametersAllowed;
   }

   bool ParsedFunction::GetLogicOperatorsAllowed()
   {
      return _logicOperatorsAllowed;
   }

   void ParsedFunction::SetLogicOperatorsAllowed(bool logicOperatorsAllowed)
   {

      if (logicOperatorsAllowed == _logicOperatorsAllowed)
         return;

      _logicOperatorsAllowed = logicOperatorsAllowed;

      if (!_logicOperatorsAllowed)
         ResetParsedState();
   }

   bool ParsedFunction::GetLogicalNumericMixAllowed()
   {
      return _logicalNumericMixAllowed;
   }

   void ParsedFunction::SetLogicalNumericMixAllowed(bool logicalNumericMixAllowed)
   {
      if (logicalNumericMixAllowed == _logicalNumericMixAllowed)
         return;

      _logicalNumericMixAllowed = logicalNumericMixAllowed;

      if (_logicOperatorsAllowed && !_logicalNumericMixAllowed)
         ResetParsedState();
   }

   double ParsedFunction::GetComparisonTolerance()
   {
      return _comparisonTolerance;
   }

   void ParsedFunction::SetComparisonTolerance(double comparisonTolerance)
   {
      const char* ERROR_SOURCE = "ParsedFunction::SetComparisonTolerance";

      if (_comparisonTolerance == comparisonTolerance)
         return;

      if (comparisonTolerance < 0.0)
         throw FuncParserErrorData(FuncParserErrorData::err_BADARG, ERROR_SOURCE,
            "Tolerance may not be negative");

      _comparisonTolerance = comparisonTolerance;

      //reset Simplified state (due to some comparison results might have been changed with the new tolerance)
      ResetParsedState(true);
   }

   const std::string ParsedFunction::GetStringToParse() const
   {
      return _stringToParse;
   }

   void ParsedFunction::SetStringToParse(std::string stringToParse)
   {
      if (stringToParse == _stringToParse)
         return;
      _stringToParse = std::move(stringToParse);
      ResetParsedState();
   }

   void ParsedFunction::Parse()
   {
      if (_parsed) return;

      FuncParser funcParser;
      _funcNode.reset(funcParser.Parse(_stringToParse,
                                       _variableNames,
                                       _parameterNames,
                                       _caseSensitive,
                                       _logicOperatorsAllowed,
                                       _comparisonTolerance,
                                       _logicalNumericMixAllowed));

      _parsed = true;
   }

   double ParsedFunction::CalcExpression(const DoubleVector& Arg)
   {
       const std::string ERROR_SOURCE = "ParsedFunction::CalcExpression";

       //check arguments size
       if (Arg.size() != _variableNames.size())
           throw FuncParserErrorData(
               FuncParserErrorData::err_BADARG,
               ERROR_SOURCE,
               "Number of variable values differs from the number of variable names");

       //check if Parameter values are available
       if (_parameterNames.size() > 0 && !_parameterValues)
           throw FuncParserErrorData(FuncParserErrorData::err_BADARG,
                                     ERROR_SOURCE,
                                     "Parameter values are not set");

       //that is also checked in <Parse> itself. The check is done here
       //once more just to prevent an additional function call (time!!)
       if (!_parsed)
       {
           Parse();
       }

       //that is also checked in <SimplifyParameters> itself. The check is done here
       //once more just to prevent an additional function call (time!!)
       if (!_parametersSimplified)
       {
           SimplifyParameters();
       }

       // Arg ist guarateed to be contiguous in memory
       auto dValue = _simplifiedNode->CalcNodeValue(Arg.data(),
                                                    _parameterValues.get(),
                                                    _comparisonTolerance);

       return dValue;
   }

   std::string ParsedFunction::GetXMLString(bool InSimplifiedState /*= true*/, const std::string& pContainerNodeName /*= "Rate"*/)
   {
      if (!_parsed)
      {
         Parse();
      }

      std::string RateString;
      FuncNode *Node2Expand = nullptr;

      if (InSimplifiedState)
      {
          SimplifyParameters();
          Node2Expand = _simplifiedNode.get();
      } else
      {
          Node2Expand = _funcNode.get();
      }

      assert(Node2Expand != nullptr);
      RateString = Node2Expand->XMLString(_variableNames, _parameterNames);

      std::string StringToParseAttr;
      if ((_stringToParse.find_first_of("<") == std::string::npos)
          && (_stringToParse.find_first_of(">") == std::string::npos)
          && (_stringToParse.find_first_of("&") == std::string::npos)
          && (_stringToParse.find_first_of("'") == std::string::npos)
          && (_stringToParse.find_first_of("\"") == std::string::npos))
      {
          StringToParseAttr = " StringToParse=\"" + _stringToParse + "\" ";
      } else
      {
          StringToParseAttr = "";
      }
      auto XMLString = "<" + pContainerNodeName + StringToParseAttr + ">" + RateString + "</"
                       + pContainerNodeName + ">";

      return XMLString;
   }

   double ParsedFunction::CalcExpression(double* dArgs, FuncParserErrorData& ED)
   {
      ED.Clear();
      const std::string ERROR_SOURCE = "ParsedFunction::CalcExpression";

      double dValue = Math::GetNaN();

      try
      {

         //check if Parameter values are available
         if (_parametersAvailable && !_parameterValues)
             throw FuncParserErrorData(FuncParserErrorData::err_BADARG,
                                       ERROR_SOURCE,
                                       "Parameter values are not set");

         //that is also checked in <Parse> itself. The check is done here
         //once more just to prevent an additional function call (time!!)
         if (!_parsed)
         {
             Parse();
         }

         //that is also checked in <SimplifyParameters> itself. The check is done here
         //once more just to prevent a function call (time!!)
         if (!_parametersSimplified)
         {
             SimplifyParameters();
         }

         dValue = _simplifiedNode->CalcNodeValue(dArgs,
                                                 _parameterValues.get(),
                                                 _comparisonTolerance);
      }
      catch (FuncParserErrorData& ED_)
      {
         ED = ED_;
      }
      catch (...)
      {
         ED.SetError(FuncParserErrorData::err_RUNTIME, ERROR_SOURCE, "Unknown error");
      }

      return dValue;
   }

   void ParsedFunction::SetParametersNotToSimplify(StringVector parameterNames)
   {
       _fixedParameters = std::move(parameterNames);
       ResetParsedState(true);
   }

   const StringVector& ParsedFunction::GetParametersNotToSimplify() const
   {
      return _fixedParameters;
   }

   DimensionInfo ParsedFunction::GetDimensionInfoFor(const std::string& formula,
      const std::vector<QuantityDimensionInfo>& quantityDimensions)
   {

      DimensionInfo dimensionInfo;

      try
      {
          const auto parameterNames = ExtractQuantityNames(quantityDimensions);

          SetCaseSensitive(true);
          SetLogicalNumericMixAllowed(true);
          SetLogicOperatorsAllowed(true);

          //pass all quantities as parameters
          SetParameterNames(parameterNames);

          //mark all parameters not for simplifying but allow simplifying itself,
          //so only arithmetic expression like 2*sin(pi/3)+2
          //will be simplified
          SetParametersNotToSimplify(parameterNames);

          SetSimplifyParametersAllowed(false);
          SetStringToParse(formula);
          Parse();
          SimplifyParameters();

          StringVector variableNames;
          //pass dimension infos to the expression tree nodes
          for (const auto qdim_info : quantityDimensions)
          {
              _simplifiedNode->SetDimensionInfo(qdim_info, variableNames, parameterNames);
          }

          //finally get dimension for the formula
          dimensionInfo = _simplifiedNode->CalcNodeDimensionInfo();
      } catch (FuncParserErrorData &ED)
      {
         ED.SetDescription(ED.GetDescription() + " (Formula: " + formula + ")");
         throw ED;
      }
      catch (...)
      {
          const char *ERROR_SOURCE = "ParsedFunction::GetDimensionInfoFor";
          throw FuncParserErrorData(FuncParserErrorData::err_RUNTIME,
                                    ERROR_SOURCE,
                                    "Unknown error (Formula: " + formula + ")");
      }

      return dimensionInfo;
   }

   std::vector<std::string> ParsedFunction::ExtractQuantityNames(
       const std::vector<QuantityDimensionInfo> &quantityDimensions)
   {
       std::vector<std::string> quantityNames;
       quantityNames.reserve(quantityDimensions.size());

       for (const auto &qdim_info : quantityDimensions)
       {
           quantityNames.push_back(qdim_info.GetQuantityName());
       }

       return quantityNames;
   }

   //-------------- C interface for PInvoke -----------------------------------------
   ParsedFunction* CreateParsedFunction()
   {
      return new ParsedFunction();
   }

   void DisposeParsedFunction(ParsedFunction* parsedFunction)
   {
       if (parsedFunction)
           return;

       delete parsedFunction;
   }

   bool GetCaseSensitive(ParsedFunction* parsedFunction)
   {
      return parsedFunction->GetCaseSensitive();
   }

   void SetCaseSensitive(ParsedFunction* parsedFunction, bool caseSensitive)
   {
      parsedFunction->SetCaseSensitive(caseSensitive);
   }

   void SetVariableNames(ParsedFunction* parsedFunction, const char* variableNames[], int size)
   {
      StringVector vec;
      vec.reserve(size);

      for (auto i = 0; i < size; i++)
          vec.push_back(variableNames[i]);

      parsedFunction->SetVariableNames(std::move(vec));
   }

   void SetParameterNames(ParsedFunction* parsedFunction, const char* parameterNames[], int size)
   {
      StringVector vec;
      vec.reserve(size);

      for (auto i = 0; i < size; i++)
          vec.push_back(parameterNames[i]);

      parsedFunction->SetParameterNames(std::move(vec));
   }

   void SetParameterValues(ParsedFunction *parsedFunction,
                           const double *parameterValues,
                           int size,
                           bool &success,
                           char **errorMessage)
   {
      success = true;

      try
      {
         DoubleVector vec;
         vec.reserve(size);

         for (auto i = 0; i < size; i++)
             vec.push_back(parameterValues[i]);

         parsedFunction->SetParameterValues(vec);
      }
      catch (FuncParserErrorData& ED)
      {
         *errorMessage = ErrorMessageFrom(ED);
         success = false;
      }
      catch (...)
      {
         *errorMessage = ErrorMessageFromUnknown("SetParameterValues");
         success = false;
      }
   }

   void SetParametersNotToSimplify(ParsedFunction *parsedFunction,
                                   const char *parameterNames[],
                                   int size,
                                   bool &success,
                                   char **errorMessage)
   {
      success = true;

      try
      {
         StringVector vec;
         vec.reserve(size);

         for (auto i = 0; i < size; i++)
             vec.push_back(parameterNames[i]);

         parsedFunction->SetParametersNotToSimplify(std::move(vec));
      }
      catch (FuncParserErrorData& ED)
      {
         *errorMessage = ErrorMessageFrom(ED);
         success = false;
      }
      catch (...)
      {
         *errorMessage = ErrorMessageFromUnknown("SetParametersNotToSimplify");
         success = false;
      }
   }

   bool GetSimplifyParametersAllowed(ParsedFunction* parsedFunction)
   {
       return parsedFunction->GetSimplifyParametersAllowed();
   }

   void SetSimplifyParametersAllowed(ParsedFunction* parsedFunction, bool simplifyParametersAllowed)
   {
       parsedFunction->SetSimplifyParametersAllowed(simplifyParametersAllowed);
   }

   bool GetLogicOperatorsAllowed(ParsedFunction* parsedFunction)
   {
       return parsedFunction->GetLogicOperatorsAllowed();
   }

   void SetLogicOperatorsAllowed(ParsedFunction* parsedFunction, bool logicOperatorsAllowed)
   {
      parsedFunction->SetLogicOperatorsAllowed(logicOperatorsAllowed);
   }

   bool GetLogicalNumericMixAllowed(ParsedFunction* parsedFunction)
   {
      return parsedFunction->GetLogicalNumericMixAllowed();
   }

   void SetLogicalNumericMixAllowed(ParsedFunction* parsedFunction, bool logicalNumericMixAllowed)
   {
      parsedFunction->SetLogicalNumericMixAllowed(logicalNumericMixAllowed);
   }

   double GetComparisonTolerance(ParsedFunction* parsedFunction)
   {
      return parsedFunction->GetComparisonTolerance();
   }

   void SetComparisonTolerance(ParsedFunction *parsedFunction,
                               double comparisonTolerance,
                               bool &success,
                               char **errorMessage)
   {
      try
      {
         parsedFunction->SetComparisonTolerance(comparisonTolerance);
         success = true;
      }
      catch (FuncParserErrorData& ED)
      {
         *errorMessage = ErrorMessageFrom(ED);
         success = false;
      }
      catch (...)
      {
         *errorMessage = ErrorMessageFromUnknown("SetComparisonTolerance");
         success = false;
      }
   }

   const char* GetStringToParse(ParsedFunction* parsedFunction)
   {
      return MarshalString(parsedFunction->GetStringToParse());
   }

   void SetStringToParse(ParsedFunction* parsedFunction, const char* stringToParse)
   {
      parsedFunction->SetStringToParse(stringToParse);
   }

   void Parse(ParsedFunction* parsedFunction, bool& success, char** errorMessage)
   {
      success = true;

      try
      {
         parsedFunction->Parse();
      }
      catch (FuncParserErrorData& ED)
      {
         *errorMessage = ErrorMessageFrom(ED);
         success = false;
      }
      catch (...)
      {
         *errorMessage = ErrorMessageFromUnknown("Parse");
         success = false;
      }
   }

   double CalcExpression(ParsedFunction* parsedFunction, const double* arguments, int size, bool& success, char** errorMessage)
   {
      success = true;

      try
      {
         DoubleVector argumentsVec;
         argumentsVec.reserve(size);

         for (int i = 0; i < size; i++)
             argumentsVec.push_back(arguments[i]);

         return parsedFunction->CalcExpression(argumentsVec);
      }
      catch (FuncParserErrorData& ED)
      {
         *errorMessage = ErrorMessageFrom(ED);
         success = false;
      }
      catch (...)
      {
         *errorMessage = ErrorMessageFromUnknown("CalcExpression");
         success = false;
      }

      return Math::GetNaN();
   }

   char *GetXMLString(ParsedFunction *parsedFunction,
                      bool &success,
                      char **errorMessage,
                      bool InSimplifiedState,
                      const char *containerNodeName)
   {
      success = true;

      try
      {
         return MarshalString(parsedFunction->GetXMLString());
      }
      catch (FuncParserErrorData& ED)
      {
         *errorMessage = ErrorMessageFrom(ED);
         success = false;
      }
      catch (...)
      {
         *errorMessage = ErrorMessageFromUnknown("GetStringToParse");
         success = false;
      }

      return MarshalString("");
   }

   void UpdateFrom(ParsedFunction* srcParsedFunction, ParsedFunction* targetParsedFunction)
   {
      targetParsedFunction->SetCaseSensitive(srcParsedFunction->GetCaseSensitive());
      targetParsedFunction->SetVariableNames(srcParsedFunction->GetVariableNames());
      targetParsedFunction->SetParameterNames(srcParsedFunction->GetParameterNames());
      targetParsedFunction->SetParameterValues(srcParsedFunction->GetParameterValues());
      targetParsedFunction->SetSimplifyParametersAllowed(srcParsedFunction->GetSimplifyParametersAllowed());
      targetParsedFunction->SetLogicOperatorsAllowed(srcParsedFunction->GetLogicOperatorsAllowed());
      targetParsedFunction->SetStringToParse(srcParsedFunction->GetStringToParse());
      targetParsedFunction->SetComparisonTolerance(srcParsedFunction->GetComparisonTolerance());
      targetParsedFunction->SetLogicalNumericMixAllowed(srcParsedFunction->GetLogicalNumericMixAllowed());
      targetParsedFunction->SetParametersNotToSimplify(srcParsedFunction->GetParametersNotToSimplify());
   }

}//.. end "namespace FuncParserNative"
