using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

namespace OSPSuite.FuncParser
{
   internal static class FuncParserImportDefinitions
   {

#if _WINDOWS
      public const String NATIVE_DLL = "OSPSuite.FuncParserNative.dll";
#else
      public const String NATIVE_DLL = "libOSPSuite.FuncParserNative.so";
#endif

      public const CallingConvention CALLING_CONVENTION = CallingConvention.Cdecl;
   }

   internal static class ParsedFunctionImports
   {
      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern IntPtr CreateParsedFunction();

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern void DisposeParsedFunction(IntPtr parsedFunction);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern bool GetCaseSensitive(IntPtr parsedFunction);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern void SetCaseSensitive(IntPtr parsedFunction, bool caseSensitive);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern void SetVariableNames(IntPtr parsedFunction, [In, Out] string[] variableNames, int size);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern void SetParameterNames(IntPtr parsedFunction, [In, Out] string[] parameterNames, int size);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern void SetParameterValues(IntPtr parsedFunction, [In, Out] double[] parameterValues, int size, out bool success, out string errorMessage);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern void SetParametersNotToSimplify(IntPtr parsedFunction, [In, Out] string[] parameterNames, int size, out bool success, out string errorMessage);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern bool GetSimplifyParametersAllowed(IntPtr parsedFunction);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern void SetSimplifyParametersAllowed(IntPtr parsedFunction, bool simplifyParametersAllowed);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern bool GetLogicOperatorsAllowed(IntPtr parsedFunction);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern void SetLogicOperatorsAllowed(IntPtr parsedFunction, bool logicOperatorsAllowed);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern bool GetLogicalNumericMixAllowed(IntPtr parsedFunction);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern void SetLogicalNumericMixAllowed(IntPtr parsedFunction, bool logicalNumericMixAllowed);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern double GetComparisonTolerance(IntPtr parsedFunction);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern void SetComparisonTolerance(IntPtr parsedFunction, double comparisonTolerance, out bool success, out string errorMessage);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern string GetStringToParse(IntPtr parsedFunction);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern void SetStringToParse(IntPtr parsedFunction, string stringToParse);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern void Parse(IntPtr parsedFunction, out bool success, out string errorMessage);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern double CalcExpression(IntPtr parsedFunction, [In, Out] double[] arguments, int size, out bool success, out string errorMessage);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern string GetXMLString(IntPtr parsedFunction, out bool success, out string errorMessage);

      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern void UpdateFrom(IntPtr srcParsedFunction, IntPtr targetParsedFunction);

   }

   public class ParsedFunction
   {
      private readonly IntPtr _parsedFunction;

      public ParsedFunction()
      {
         _parsedFunction = ParsedFunctionImports.CreateParsedFunction();
      }

      ~ParsedFunction()
      {
         ParsedFunctionImports.DisposeParsedFunction(_parsedFunction);
      }

      public bool CaseSensitive
      {
         get => ParsedFunctionImports.GetCaseSensitive(_parsedFunction);
         set => ParsedFunctionImports.SetCaseSensitive(_parsedFunction, value);
      }

      public void SetVariableNames(IEnumerable<string> variableNames)
      {
         var (variableNamesArray, size) = convertToArray(variableNames);
         ParsedFunctionImports.SetVariableNames(_parsedFunction, variableNamesArray, size);
      }

      public void SetParameterNames(IEnumerable<string> parameterNames)
      {
         var (parameterNamesArray, size) = convertToArray(parameterNames);
         ParsedFunctionImports.SetParameterNames(_parsedFunction, parameterNamesArray, size);
      }

      public void SetParameterValues(IEnumerable<double> parameterValues)
      {
         var (parameterValuesArray, size) = convertToArray(parameterValues);
         ParsedFunctionImports.SetParameterValues(_parsedFunction, parameterValuesArray, size, out var success, out var errorMessage);

         if (success)
            return;

         throw new Exception(errorMessage);
      }

      public void SetParametersNotToSimplify(IEnumerable<string> parameterNames)
      {
         var (parameterNamesArray, size) = convertToArray(parameterNames);
         ParsedFunctionImports.SetParametersNotToSimplify(_parsedFunction, parameterNamesArray, size, out var success, out var errorMessage);

         if (success)
            return;

         throw new Exception(errorMessage);
      }

      public bool SimplifyParametersAllowed
      {
         get => ParsedFunctionImports.GetSimplifyParametersAllowed(_parsedFunction);
         set => ParsedFunctionImports.SetSimplifyParametersAllowed(_parsedFunction, value);
      }

      public bool LogicOperatorsAllowed
      {
         get => ParsedFunctionImports.GetLogicOperatorsAllowed(_parsedFunction);
         set => ParsedFunctionImports.SetLogicOperatorsAllowed(_parsedFunction, value);
      }

      public bool LogicalNumericMixAllowed
      {
         get => ParsedFunctionImports.GetLogicalNumericMixAllowed(_parsedFunction);
         set => ParsedFunctionImports.SetLogicalNumericMixAllowed(_parsedFunction, value);
      }

      public double ComparisonTolerance
      {
         get => ParsedFunctionImports.GetComparisonTolerance(_parsedFunction);
         set
         {
            ParsedFunctionImports.SetComparisonTolerance(_parsedFunction, value, out var success, out var errorMessage);

            if (success)
               return;

            throw new Exception(errorMessage);
         }
      }

      public string StringToParse
      {
         get => ParsedFunctionImports.GetStringToParse(_parsedFunction);
         set => ParsedFunctionImports.SetStringToParse(_parsedFunction, value);
      }

      public void Parse()
      {
         ParsedFunctionImports.Parse(_parsedFunction, out var success, out var errorMessage);

         if (success)
            return;

         throw new Exception(errorMessage);
      }

      public double CalcExpression(IEnumerable<double> arguments)
      {
         var (argumentsArray, size) = convertToArray(arguments);
         var value = ParsedFunctionImports.CalcExpression(_parsedFunction, argumentsArray, size, out var success, out var errorMessage);

         if (success)
            return value;

         throw new Exception(errorMessage);
      }

      public string GetXMLString()
      {
         var xmlString = ParsedFunctionImports.GetXMLString(_parsedFunction, out var success, out var errorMessage);

         if (success)
            return xmlString;

         throw new Exception(errorMessage);
      }

      public void UpdateFrom(ParsedFunction srcParsedFunction)
      {
         ParsedFunctionImports.UpdateFrom(srcParsedFunction.NativeParsedFunction,_parsedFunction);
      }

      internal IntPtr NativeParsedFunction => _parsedFunction;

      private (T[] array, int arraySize) convertToArray<T>(IEnumerable<T> values)
      {
         var array = values?.ToArray();
         int size = array?.Length ?? 0;

         return (array, size);
      }
   }
}
