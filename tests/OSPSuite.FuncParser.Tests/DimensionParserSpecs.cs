using System.Collections.Generic;
using NUnit.Framework;
using OSPSuite.BDDHelper;
using OSPSuite.BDDHelper.Extensions;

namespace OSPSuite.FuncParser.DimensionParserTests
{
   public abstract class concern_for_DimensionParser : ContextSpecification<DimensionParser>
   {
      protected IList<QuantityDimensionInformation> _quantityDimensionInfos=new List<QuantityDimensionInformation>();

      public override void GlobalContext()
      {
         base.GlobalContext();

         var p1 = new QuantityDimensionInformation("p1", 1, 1, 1, 1, 1, 1, 1);
         var p2 = new QuantityDimensionInformation("p2", 2, 2, 2, 2, 2, 2, 2);
         var p3 = new QuantityDimensionInformation("p3", 0, 0, 0, 0, 0, 0, 0);
         var p4 = new QuantityDimensionInformation("p4", -3, -2, -1, 0, 1, 2, 3);
         var p5 = new QuantityDimensionInformation("p5", 1, 2, 3, 0, 3, 4, 5);

         _quantityDimensionInfos.Add(p1);
         _quantityDimensionInfos.Add(p2);
         _quantityDimensionInfos.Add(p3);
         _quantityDimensionInfos.Add(p4);
         _quantityDimensionInfos.Add(p5);
      }

      protected override void Context()
      {
         sut = new DimensionParser();
      }

   }

   public class when_getting_formula_dimension : concern_for_DimensionParser
   {
      [Observation]
      [TestCaseSource(nameof(TestData))]
      public void should_return_correct_parsed_dimension(string formula, 
         double lengthExponent, double massExponent, double timeExponent,
         double electricCurrentExponent, double temperatureExponent, double amountExponent,
         double luminousIntensityExponent)
      {
         var (dimension, parseSuccess, calculateDimensionSuccess, _) =
            sut.GetDimensionInformationFor(formula, _quantityDimensionInfos);

         parseSuccess.ShouldBeTrue();
         calculateDimensionSuccess.ShouldBeTrue();

         var returnedDimension=new DimensionInformation(lengthExponent, massExponent, timeExponent,
         electricCurrentExponent, temperatureExponent, amountExponent, luminousIntensityExponent);

         dimension.Equals(returnedDimension).ShouldBeTrue();
      }

      protected static IEnumerable<object[]> TestData()
      {
         //p1={1,   1,   1,  1,  1,  1,  1}
         //p2={2,   2,   2,  2,  2,  2,  2}
         //p3={0,   0,   0,  0,  0,  0,  0}
         //p4={-3, -2,  -1,  0,  1,  2,  3}
         //p5={1,   2,   3,  0,  3,  4,  5}

         yield return new object[]
         {
            "p1*p2*p3",
            3,3,3,3,3,3,3
         };

         yield return new object[]
         {
            "p1*p4/p5",
            -3,-3,-3,1,-1,-1,-1
         };

         yield return new object[]
         {
            "(p1+p2/p1)^17",
            17,17,17,17,17,17,17
         };

         yield return new object[]
         {
            "2>1 ? min(p1-2*p1; p2/p1) : 99",
            1,1,1,1,1,1,1
         };

         yield return new object[]
         {
            "p3*p1>p2/p1 and p3*p1>=p2/p1 or p3*p1<p2/p1 and not p3*p1<=p2/p1 or p3*p1<>p2/p1",
            0,0,0,0,0,0,0
         };

         yield return new object[]
         {
            "2<1 ? 33 : max(p1-2*p1; p2/p1)",
            1,1,1,1,1,1,1
         };

         yield return new object[]
         {
            "sqrt(p4)*exp(p3)*ln(p3)*log(p3)*log10(p3)",
            -3/2.0,-2/2.0,-1/2.0, 0, 1/2.0,2/2.0,3/2.0
         };

         yield return new object[]
         {
            "acos(p3)+asin(p3)+atan(p3)+cos(p3)+cosh(p3)+sin(p3)+sinh(p3)+tan(p3)+tanh(p3)",
            0,0,0,0,0,0,0
         };

         yield return new object[]
         {
            "(Rnd()*p1*p1-Srnd()*p2)/(p1*p1)+Rnd()+Srnd()+Rnd()*Srnd()",
            0,0,0,0,0,0,0
         };

         yield return new object[]
         {
            "p1>p2 ? p1-2*p1 : p2/p1", //non-const if condition but same dimensions in THAN and ELSE parts
            1,1,1,1,1,1,1
         };
      }
   }

   public class when_getting_dimension_for_invalid_formula : concern_for_DimensionParser
   {
      [Observation]
      [TestCaseSource(nameof(TestData))]
      public void should_return_parse_failure(string formula)
      {
         var (_, parseSuccess, calculateDimensionSuccess, errorMessage) =
            sut.GetDimensionInformationFor(formula, _quantityDimensionInfos);

         parseSuccess.ShouldBeFalse();
         calculateDimensionSuccess.ShouldBeFalse();
         string.IsNullOrEmpty(errorMessage).ShouldBeFalse();
      }

      protected static IEnumerable<string> TestData()
      {
         yield return "p1+p2+"; //invalid formula equation
         yield return "p1+p2+p6"; //"p6" is not in the list of passed quantity dimension infos
      }
   }

   public class when_cannot_calculate_dimension_for_valid_formula : concern_for_DimensionParser
   {
      [Observation]
      [TestCaseSource(nameof(TestData))]
      public void should_return_dimension_calculation_failure(string formula)
      {
         var (_, parseSuccess, calculateDimensionSuccess, errorMessage) =
            sut.GetDimensionInformationFor(formula, _quantityDimensionInfos);

         parseSuccess.ShouldBeTrue(); //formula is valid
         calculateDimensionSuccess.ShouldBeFalse();
         string.IsNullOrEmpty(errorMessage).ShouldBeFalse();
      }

      protected static IEnumerable<string> TestData()
      {
         yield return "acos(p5)"; 
         yield return "asin(p5)";
         yield return "atan(p5)";
         yield return "cos(p5)";
         yield return "cosh(p5)";
         yield return "p5 and 2";
         yield return "exp(p5)";
         yield return "ln(p5)";
         yield return "log(p5)";
         yield return "log10(p5)";
         yield return "2^p5";
         yield return "sin(p5)";
         yield return "sinh(p5)";
         yield return "tan(p5)";
         yield return "tanh(p5)";
         yield return "p4>p5";
         yield return "p4>=p5";
         yield return "p4<p5";
         yield return "p4<=p5";
         yield return "p4<>p5";
         yield return "max(p4;p5)";
         yield return "min(p4;p5)";
         yield return "p4-p5";
         yield return "p4=p5";
         yield return "NOT p4";
         yield return "p4 OR p5";
         yield return "p4+p5";
         yield return "p1>p2 ? p1-2*p1 : p2";
      }
   }
}