using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace OSPSuite.FuncParser
{
   internal static class DimensionParserImports
   {
      [DllImport(FuncParserImportDefinitions.NATIVE_DLL, CallingConvention = FuncParserImportDefinitions.CALLING_CONVENTION)]
      public static extern DimensionInformationStructure GetDimensionInfoFor(
         string formula, [In, Out] QuantityDimensionInformationStructure[] quantityDimensions, 
         int size, out bool parseSuccess, out bool calculateDimensionSuccess, out string errorMessage);
   }

   public class DimensionParser
   {

      /// <summary>
      /// Dimension information (=dimension exponents) for the given formula
      /// </summary>
      /// <param name="formula">formula of interest</param>
      /// <param name="quantityDimensions">dimension information for all quantities used in the formula</param>
      /// <returns> if (parseSuccess = true AND calculateDimensionSuccess = true): dimension information for the given formula
      /// if (parseSuccess = true AND calculateDimensionSuccess = false): dimension information could not be calculated (e.g. formula is "x+y" where x and y have not the same dimension
      /// if (parseSuccess = false): formula is invalid
      /// message contains error description if (parseSuccess = false OR calculateDimensionSuccess = false)
      /// </returns>
      public (DimensionInformation, bool parseSuccess, bool calculateDimensionSuccess, string errorMessage) GetDimensionInformationFor(
         string formula, IEnumerable<QuantityDimensionInformation> quantityDimensions)
      {
         var dimensionInfoStructures=new List<QuantityDimensionInformationStructure>();
         foreach (var dimensionInfo in quantityDimensions)
         {
            dimensionInfoStructures.Add(new QuantityDimensionInformationStructure(dimensionInfo));
         }

         var dimensionInformationStructure = DimensionParserImports.GetDimensionInfoFor(
            formula, dimensionInfoStructures.ToArray(), dimensionInfoStructures.Count,
            out var parseSuccess, out var calculateDimensionSuccess, out var errorMessage);

         return (new DimensionInformation(dimensionInformationStructure), parseSuccess, calculateDimensionSuccess,
            errorMessage);
      }
   }
}
