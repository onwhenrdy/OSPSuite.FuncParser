#ifndef _ParsedFunction_H_
#define _ParsedFunction_H_

#include "FuncParser/FuncNode.h"
#include "FuncParser/FuncParserTypeDefs.h"
#include "FuncParser/QuantityDimensionInfo.h"

#include <memory>

namespace FuncParserNative
{
   class FuncParserErrorData;

   class ParsedFunction
   {
   private:
       std::unique_ptr<FuncNode> _funcNode;
       std::unique_ptr<FuncNode> _simplifiedNode;
       bool _caseSensitive;
       StringVector _variableNames;
       StringVector _parameterNames;
       StringVector _fixedParameters;
       std::unique_ptr<double[]> _parameterValues;
       bool _simplifyParametersAllowed;
       bool _logicOperatorsAllowed;
       bool _logicalNumericMixAllowed;
       bool _parsed;
       double _comparisonTolerance;
       bool _parametersSimplified;
       std::string _stringToParse;
       bool _parametersAvailable;

       void ResetParsedState(bool simplifiedStateOnly = false);
       IndexVector GetFixedParametersIndexVector();
       void SimplifyParameters();
       std::vector<std::string> ExtractQuantityNames(
           const std::vector<QuantityDimensionInfo> &quantityDimensions);

   public:
      FUNCPARSER_EXPORT ParsedFunction();
      FUNCPARSER_EXPORT bool GetCaseSensitive() const;
      FUNCPARSER_EXPORT void SetCaseSensitive(bool caseSensitive);
      FUNCPARSER_EXPORT const StringVector& GetVariableNames() const;
      FUNCPARSER_EXPORT void SetVariableNames(StringVector variableNames);
      FUNCPARSER_EXPORT const StringVector& GetParameterNames() const;
      FUNCPARSER_EXPORT void SetParameterNames(StringVector parameterNames);
      FUNCPARSER_EXPORT DoubleVector GetParameterValues() const;
      FUNCPARSER_EXPORT void SetParameterValues(const DoubleVector& parameterValues);
      FUNCPARSER_EXPORT bool GetSimplifyParametersAllowed();
      FUNCPARSER_EXPORT void SetSimplifyParametersAllowed(bool simplifyParametersAllowed);
      FUNCPARSER_EXPORT bool GetLogicOperatorsAllowed();
      FUNCPARSER_EXPORT void SetLogicOperatorsAllowed(bool logicOperatorsAllowed);
      FUNCPARSER_EXPORT bool GetLogicalNumericMixAllowed();
      FUNCPARSER_EXPORT void SetLogicalNumericMixAllowed(bool logicalNumericMixAllowed);
      FUNCPARSER_EXPORT double GetComparisonTolerance();
      FUNCPARSER_EXPORT void SetComparisonTolerance(double comparisonTolerance);
      FUNCPARSER_EXPORT const std::string GetStringToParse() const;
      FUNCPARSER_EXPORT void SetStringToParse(std::string stringToParse);
      FUNCPARSER_EXPORT void Parse();
      FUNCPARSER_EXPORT double CalcExpression(const DoubleVector& Arg);
      FUNCPARSER_EXPORT std::string GetXMLString(bool InSimplifiedState = true, const std::string& pContainerNodeName = "Rate");
      FUNCPARSER_EXPORT double CalcExpression(double* dArgs, FuncParserErrorData& ED);
      FUNCPARSER_EXPORT void SetParametersNotToSimplify(StringVector parameterNames);
      FUNCPARSER_EXPORT const StringVector& GetParametersNotToSimplify() const;

      DimensionInfo GetDimensionInfoFor(const std::string& formula,
         const std::vector<QuantityDimensionInfo>& quantityDimensions);
   };

   //-------------- C interface for PInvoke -----------------------------------------
   extern "C"
   {
      FUNCPARSER_EXPORT ParsedFunction* CreateParsedFunction();
      FUNCPARSER_EXPORT void DisposeParsedFunction(ParsedFunction* parsedFunction);
      FUNCPARSER_EXPORT bool GetCaseSensitive(ParsedFunction* parsedFunction);
      FUNCPARSER_EXPORT void SetCaseSensitive(ParsedFunction* parsedFunction, bool caseSensitive);
      //   FUNCPARSER_EXPORT const StringVector& GetVariableNames() const;
      FUNCPARSER_EXPORT void SetVariableNames(ParsedFunction* parsedFunction, const char** variableNames, int size);
      //   FUNCPARSER_EXPORT const StringVector& GetParameterNames(FuncParserErrorData& ED) const;
      FUNCPARSER_EXPORT void SetParameterNames(ParsedFunction* parsedFunction, const char** parameterNames, int size);
      //   FUNCPARSER_EXPORT const DoubleVector GetParameterValues(FuncParserErrorData& ED) const;
      FUNCPARSER_EXPORT void SetParametersNotToSimplify(ParsedFunction* parsedFunction, const char** parameterNames, int size, bool& success, char** errorMessage);
      //   FUNCPARSER_EXPORT const StringVector& GetParametersNotToSimplify(FuncParserErrorData& ED) const;
      FUNCPARSER_EXPORT void SetParameterValues(ParsedFunction* parsedFunction, const double* parameterValues, int size, bool& success, char** errorMessage);
      FUNCPARSER_EXPORT bool GetSimplifyParametersAllowed(ParsedFunction* parsedFunction);
      FUNCPARSER_EXPORT void SetSimplifyParametersAllowed(ParsedFunction* parsedFunction, bool simplifyParametersAllowed);
      FUNCPARSER_EXPORT bool GetLogicOperatorsAllowed(ParsedFunction* parsedFunction);
      FUNCPARSER_EXPORT void SetLogicOperatorsAllowed(ParsedFunction* parsedFunction, bool logicOperatorsAllowed);
      FUNCPARSER_EXPORT bool GetLogicalNumericMixAllowed(ParsedFunction* parsedFunction);
      FUNCPARSER_EXPORT void SetLogicalNumericMixAllowed(ParsedFunction* parsedFunction, bool logicalNumericMixAllowed);
      FUNCPARSER_EXPORT double GetComparisonTolerance(ParsedFunction* parsedFunction);
      FUNCPARSER_EXPORT void SetComparisonTolerance(ParsedFunction* parsedFunction, double comparisonTolerance, bool& success, char** errorMessage);
      FUNCPARSER_EXPORT const char* GetStringToParse(ParsedFunction* parsedFunction);
      FUNCPARSER_EXPORT void SetStringToParse(ParsedFunction* parsedFunction, const char* stringToParse);
      FUNCPARSER_EXPORT void Parse(ParsedFunction* parsedFunction, bool& success, char** errorMessage);
      FUNCPARSER_EXPORT double CalcExpression(ParsedFunction* parsedFunction, const double* arguments, int size, bool& success, char** errorMessage);
      FUNCPARSER_EXPORT char* GetXMLString(ParsedFunction* parsedFunction, bool& success, char** errorMessage, bool InSimplifiedState = true, const char* containerNodeName = "Rate");
      FUNCPARSER_EXPORT void UpdateFrom(ParsedFunction* srcParsedFunction, ParsedFunction* targetParsedFunction);
   }

}//.. end "namespace FuncParserNative"


#endif //_ParsedFunction_H_

