using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace OSPSuite.FuncParser
{
   internal static class FuncParserImports
   {
      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern bool IsValidVariableOrParameterName(string name, out string errorMessage);
   }

   public class FuncParser
   {
      public (bool isValid, string errorMessage) IsValidVariableOrParameterName(string variableOrParameterName)
      {
         var isValid = FuncParserImports.IsValidVariableOrParameterName(variableOrParameterName, out var errorMessage);

         return (isValid, errorMessage);
      }
   }
}
