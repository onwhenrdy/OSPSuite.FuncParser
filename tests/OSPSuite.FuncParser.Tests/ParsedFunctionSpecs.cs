using System;
using System.Collections.Generic;
using System.Xml;
using NUnit.Framework;
using OSPSuite.BDDHelper;
using OSPSuite.BDDHelper.Extensions;

namespace OSPSuite.FuncParser.ParsedFunctionTests
{
   public abstract class concern_for_ParsedFunction : ContextSpecification<ParsedFunction>
   {
      // ---------------------------------------------------------------------------
      // values of variables and parameters used in all test expressions
      protected static double x => 1.0;
      protected static double y => 2.0;
      protected static double z => 3.0;
      protected static double p1 => 4.0;
      protected static double p2 => 5.0;

      protected override void Context()
      {
         sut = new ParsedFunction
         {
            CaseSensitive = true,
            LogicOperatorsAllowed = true,
            LogicalNumericMixAllowed = false,
            SimplifyParametersAllowed = true
         };

         sut.SetVariableNames(new[] {"x", "y", "z"});
         sut.SetParameterNames(new[] {"p1", "p2"});
         sut.SetParameterValues(new[] {p1, p2});
      }

      protected static IEnumerable<object[]> TestData()
      {
         yield return new object[]
         {
            "(x+z)^(x+z)*z+p1-x+y+z+p1*p2-(x+y-z)^2",
            Math.Pow((x + z), (x + z)) * z + p1 - x + y + z + p1 * p2 - Math.Pow((x + y - z), 2)
         };

         yield return new object[]
         {
            "(x+z)^(x+z)*z+p1-x-y+z+p1*p2-(x+y-z)^2",
            Math.Pow((x + z), (x + z)) * z + p1 - x - y + z + p1 * p2 - Math.Pow((x + y - z), 2)
         };

         yield return new object[]
         {
            "(x+z)^(x+z)*z+p1-x-y+z+p1*p2-(x-y-z)^2",
            Math.Pow((x + z), (x + z)) * z + p1 - x - y + z + p1 * p2 - Math.Pow((x - y - z), 2)
         };

         yield return new object[]
         {
            "(x+z)^(x+z)*z+p1-x-y-z+p1*p2-(x-y-z)^2",
            Math.Pow((x + z), (x + z)) * z + p1 - x - y - z + p1 * p2 - Math.Pow((x - y - z), 2)
         };

         yield return new object[]
         {
            "(x+z)^(x+z)*z+p1-x-y-z-p1*p2-(x-y-z)^2",
            Math.Pow((x + z), (x + z)) * z + p1 - x - y - z - p1 * p2 - Math.Pow((x - y - z), 2)
         };

         yield return new object[]
         {
            "(x+z)^(x+z)*z+p1-x+y-z-p1*p2-(x-y-z)^2",
            Math.Pow((x + z), (x + z)) * z + p1 - x + y - z - p1 * p2 - Math.Pow((x - y - z), 2)
         };

         yield return new object[]
         {
            "(x+z)^(x+z)*z+p1-x+y-z+p1*p2-(x-y-z)^2",
            Math.Pow((x + z), (x + z)) * z + p1 - x + y - z + p1 * p2 - Math.Pow((x - y - z), 2)
         };

         yield return new object[]
         {
            "p1*p2+p1-p2-x-y-z",
            p1 * p2 + p1 - p2 - x - y - z
         };

         yield return new object[]
         {
            "sin(x)*cos(y+z-p1/(2.12+3.16-log(z+p1^p2)))",
            Math.Sin(x) * Math.Cos(y + z - p1 / (2.12 + 3.16 - Math.Log(z + Math.Pow(p1, p2))))
         };

         yield return new object[]
         {
            "sin(x)*cos(y+z-p1/(2.12+3.16-tan(z+p1^p2)))",
            Math.Sin(x) * Math.Cos(y + z - p1 / (2.12 + 3.16 - Math.Tan(z + Math.Pow(p1, p2))))
         };

         yield return new object[]
         {
            "sin(x*p2/(p1-x*y+z-p1-p2)/cos(z)+tan(x^exp(y-asin(z/10)*acos(-0.345)+atan(exp(log(2)+log(1))))))*(sinh(cosh(tanh(sqrt(p1+p2-z)))))",
            Math.Sin(x * p2 / (p1 - x * y + z - p1 - p2) / Math.Cos(z) + Math.Tan(Math.Pow(x,
                        Math.Exp(y - Math.Asin(z / 10) * Math.Acos(-0.345) +
                                 Math.Atan(Math.Exp(Math.Log(2.0) + Math.Log(1.0))))))) *
            (Math.Sinh(Math.Cosh(Math.Tanh(Math.Sqrt(p1 + p2 - z)))))
         };

         yield return new object[]
         {
            "(x-(1.2-(2.3/(4.5/(12.13*2/7))/2)-3)*sin(pI*log(E^2)))*(x+13-12-3-4-5-6-7/(1+1/(1+1/(1+1/atan(Pi/6)))))",
            (x - (1.2 - (2.3 / (4.5 / (12.13 * 2 / 7)) / 2) - 3) * Math.Sin(Math.PI * Math.Log(Math.E * Math.E))) *
            (x + 13 - 12 - 3 - 4 - 5 - 6 - 7 / (1 + 1 / (1 + 1 / (1 + 1 / Math.Atan(Math.PI / 6)))))
         };

         yield return new object[]
         {
            "(y>z or z>y) and not (p1=p2)",
            // ReSharper disable once CompareOfFloatsByEqualityOperator
            (y > z || z > y) && p1 != p2 ? 1.0 : 0.0
         };

         yield return new object[]
         {
            "1e-3",
            1e-3
         };

         yield return new object[]
         {
            "1e-3+2e-2-3e-1",
            1e-3 + 2e-2 - 3e-1
         };
      }

      protected static IEnumerable<string> InvalidTestData()
      {
         yield return "";
         yield return "x+y+z+";
         yield return "max(x)";
         yield return "sin(x;y)";
         yield return "(x>y)+1";
         yield return "((x+y)";
         yield return "()";
         yield return "(x+y))";
         yield return "x**y";
         yield return "z>2 and >3";
         yield return "x<2 and y<";
         yield return "x!y";
         yield return "2+^3";
         yield return "Crazy(x)";
         yield return "sin";
         yield return "rnd(x+y)";
         yield return "max(x;y;z)"; //currently only 2 arguments are allowed
         yield return "x>y ? 2 (: 3)";
         yield return "x+-z";
         yield return "x*y/";
         yield return "x^y^z^";
         yield return ")x+y(";
      }

      protected double[] _arguments => new[] { x, y, z };
   }

   //Every test defines math. expression to parse and calculates
   //its value directly (via C++ code)
   // After that, 3 ParsedFunction are defined: 
   //  1: SimplifyParametersAllowed = true (which is actually sut)
   //  2: SimplifyParametersAllowed = false
   //  3: SimplifyParametersAllowed = true but some parameters set to "not to be simplified"
   //(besides that, Parsed Functions are equal)
   // For every function, value is calculated via FuncParser and compared to directly calculated value
   public class when_calculating_expressions : concern_for_ParsedFunction
   {
      [Observation]
      [TestCaseSource(nameof(TestData))]
      public void should_return_correct_value_when_simplified(string stringToParse, double expectedValue)
      {
         sut.StringToParse = stringToParse;
         sut.CalcExpression(_arguments).ShouldBeEqualTo(expectedValue);
      }

      [Observation]
      [TestCaseSource(nameof(TestData))]
      public void should_return_correct_value_when_not_simplified(string stringToParse, double expectedValue)
      {
         var notSimplifiedParsedFunction = new ParsedFunction();
         notSimplifiedParsedFunction.UpdateFrom(sut);

         notSimplifiedParsedFunction.SimplifyParametersAllowed = false;
         notSimplifiedParsedFunction.StringToParse = stringToParse;

         notSimplifiedParsedFunction.CalcExpression(_arguments).ShouldBeEqualTo(expectedValue);
      }

      [Observation]
      [TestCaseSource(nameof(TestData))]
      public void should_return_correct_value_when_partly_simplified(string stringToParse, double expectedValue)
      {
         var partlySimplifiedParsedFunction = new ParsedFunction();
         partlySimplifiedParsedFunction.UpdateFrom(sut);

         partlySimplifiedParsedFunction.StringToParse = stringToParse;
         partlySimplifiedParsedFunction.SetParametersNotToSimplify(new[] {"p1"});

         partlySimplifiedParsedFunction.CalcExpression(_arguments).ShouldBeEqualTo(expectedValue);
      }

      [TestCase]
      [Ignore("Required just for Resharper - otherwise no tests are found in the solution")]
      public void Dummy(){}
   }

   public class when_getting_xml_string : concern_for_ParsedFunction
   {
      [Observation]
      [TestCaseSource(nameof(TestData))]
      public void should_return_valid_xml_string(string stringToParse, double expectedValue)
      {
         sut.StringToParse = stringToParse;
         var (validXml, message)= isValidXml(sut.GetXMLString());
         validXml.ShouldBeTrue(message);
      }

      (bool, string) isValidXml(string xml)
      {
         try
         {
            var doc = new XmlDocument();
            doc.LoadXml(xml);
            return (true, "");
         }
         catch (Exception ex)
         {
            return (false, ex.Message);
         }
      }
   }

   public class when_parsing_invalid_expression : concern_for_ParsedFunction
   {
      [Observation]
      [TestCaseSource(nameof(InvalidTestData))]
      public void should_throw_an_exception_when_parsing_invalid_expression(string stringToParse)
      {
         sut.StringToParse = stringToParse;
         The.Action(()=>sut.Parse()).ShouldThrowAn<Exception>();
      }
   }

   public class when_calculating_invalid_expression : concern_for_ParsedFunction
   {
      [Observation]
      [TestCaseSource(nameof(InvalidTestData))]
      public void should_throw_an_exception_when_calculating_invalid_expression(string stringToParse)
      {
         sut.StringToParse = stringToParse;
         The.Action(() => sut.CalcExpression(_arguments)).ShouldThrowAn<Exception>();
      }
   }

   public class when_setting_invalid_number_of_parameters_or_variables : concern_for_ParsedFunction
   {
      protected override void Context()
      {
         base.Context();
         sut.StringToParse = "x+y+z";
      }

      [Observation]
      public void should_throw_an_exception_when_setting_too_few_parameter_values()
      {
         The.Action(() => sut.SetParameterValues(new []{13.0})).ShouldThrowAn<Exception>();
      }

      [Observation]
      public void should_throw_an_exception_when_setting_too_many_parameter_values()
      {
         The.Action(() => sut.SetParameterValues(new[] { 13.0, 14, 15 })).ShouldThrowAn<Exception>();
      }

      [Observation]
      public void should_throw_an_exception_when_setting_too_few_variable_values()
      {
         The.Action(() => sut.CalcExpression(new[] { 13.0, 14})).ShouldThrowAn<Exception>();
      }

      [Observation]
      public void should_throw_an_exception_when_setting_too_many_variable_values()
      {
         The.Action(() => sut.CalcExpression(new[] { 13.0, 14, 15, 16 })).ShouldThrowAn<Exception>();
      }
   }

   public class when_setting_variable_or_parameter_names_case_sensitive : concern_for_ParsedFunction
   {
      protected override void Context()
      {
         base.Context();
         sut.StringToParse = "x-X+p-P";
         sut.SetParameterNames(new[] { "p", "P" });
         sut.SetVariableNames(new []{"x","X"});
         sut.SetParameterValues(new []{10.0,20});
         sut.CaseSensitive = true;
      }

      [Observation]
      public void should_set_parameter_and_variable_names_which_differ_in_case_only()
      {
         sut.Parse();
      }

      [Observation]
      public void should_calculate_correct_value()
      {
         sut.CalcExpression(new [] {1.0, 2.0}).ShouldBeEqualTo(-11);
         sut.CalcExpression(new [] {2.0, 1.0}).ShouldBeEqualTo(-9);
      }
   }

   public class when_setting_variable_or_parameter_names_case_insensitive : concern_for_ParsedFunction
   {
      protected override void Context()
      {
         base.Context();
         sut.StringToParse = "2*x-X+2*p-P";
         sut.CaseSensitive = false;
      }

      [Observation]
      public void should_throw_exception_when_parameter_names_which_differ_in_case_only()
      {
         sut.SetParameterNames(new[] { "p", "P" });
         sut.SetVariableNames(new[] { "x"});
         sut.SetParameterValues(new[] { 10.0, 20 });
         The.Action(() => sut.Parse()).ShouldThrowAn<Exception>();
      }

      [Observation]
      public void should_throw_exception_when_variable_names_which_differ_in_case_only()
      {
         sut.SetParameterNames(new[] { "p"});
         sut.SetVariableNames(new[] { "x", "X" });
         sut.SetParameterValues(new[] { 10.0});
         The.Action(() => sut.Parse()).ShouldThrowAn<Exception>();
      }

      [Observation]
      public void should_calculate_correct_value()
      {
         sut.SetParameterNames(new[] { "p" });
         sut.SetVariableNames(new[] { "X" });
         sut.SetParameterValues(new[] { 10.0 });
         sut.CalcExpression(new []{1.0}).ShouldBeEqualTo(11);
      }

      [Observation]
      public void should_throw_exception_when_parameter_name_is_the_same_as_variable_name()
      {
         sut.SetParameterNames(new[] { "p", "x" });
         sut.SetVariableNames(new[] { "P", "X" });
         sut.SetParameterValues(new[] { 10.0, 20 });
         The.Action(() => sut.Parse()).ShouldThrowAn<Exception>();
      }
   }

   public class when_expression_contains_numeric_logical_mix : concern_for_ParsedFunction
   {
      protected override void Context()
      {
         base.Context();
         sut.StringToParse = "(2>3)*10 + (2<3)*100";
      }

      [Observation]
      public void should_throw_exception_when_numeric_logical_mix_is_not_allowed()
      {
         sut.LogicalNumericMixAllowed = false;
         The.Action(() => sut.Parse()).ShouldThrowAn<Exception>();
      }

      [Observation]
      public void should_calculate_correct_value_when_numeric_logical_mix_is_allowed()
      {
         sut.LogicalNumericMixAllowed = true;
         sut.CalcExpression(_arguments).ShouldBeEqualTo(100);
      }
   }

   public class when_calculating_random_function : concern_for_ParsedFunction
   {
      protected override void Context()
      {
         base.Context();
         sut.StringToParse = "x*RND()+y*Rnd()+SRND()";
      }

      [Observation]
      public void should_return_value_between_0_and_1()
      {
         //---- 0<Rnd, Srnd<1 ==>  0 <= x*rnd+y*rnd+srnd <= x+y+1
         var value = sut.CalcExpression(_arguments);
         value.ShouldBeGreaterThanOrEqualTo(0.0);
         value.ShouldBeSmallerThanOrEqualTo(x+y+1);
      }
   }
}
