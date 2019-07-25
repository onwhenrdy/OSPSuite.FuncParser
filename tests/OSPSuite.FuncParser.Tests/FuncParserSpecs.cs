using System.Collections.Generic;
using NUnit.Framework;
using OSPSuite.BDDHelper;
using OSPSuite.BDDHelper.Extensions;

namespace OSPSuite.FuncParser.FuncParserTests
{
   public abstract class concern_for_func_parser : ContextSpecification<FuncParser>
   {
      protected override void Context()
      {
         sut = new FuncParser();
      }

   }

   public class when_testing_variable_or_parameter_names : concern_for_func_parser
   {
      [Observation]
      [TestCaseSource(nameof(TestData))]
      public void should_return_true_for_valid_names(string variableOrParameterName)
      {
         var (isValid, _) = sut.IsValidVariableOrParameterName(variableOrParameterName);
         isValid.ShouldBeTrue();
      }

      [Observation]
      [TestCaseSource(nameof(InvalidTestData))]
      public void should_return_false_for_invalid_names(string variableOrParameterName)
      {
         var (isValid, _) = sut.IsValidVariableOrParameterName(variableOrParameterName);
         isValid.ShouldBeFalse();
      }

      protected static IEnumerable<string> TestData()
      {
         yield return "x";
         yield return "_x";
         yield return "x";
         yield return "x2";
         yield return "2x";
         yield return "2e";
         yield return "you_will_never_never_know_if_you_never_never_go";
         yield return "3°";
         yield return "2%";
         yield return "x$";
         yield return "y§";
         yield return "Tralala";
         yield return "x%y";
      }

      protected static IEnumerable<string> InvalidTestData()
      {
         yield return null;
         yield return " ";
         yield return "x y";
         yield return "x\\";
         yield return "x/";
         yield return "x\"";
         yield return "x'";
         yield return "x+";
         yield return "x-";
         yield return "x*";
         yield return "x^";
         yield return "x.";
         yield return "x,";
         yield return "x<";
         yield return "x>";
         yield return "x=";
         yield return "x!";
         yield return "x(";
         yield return "x)";
         yield return "x[";
         yield return "x]";
         yield return "x{";
         yield return "x}";
         yield return "x¬";
         yield return "X|";
         yield return "X&";
         yield return "X;";
         yield return "sin";
         yield return "SIN";
         yield return "sIN";
         yield return "SiN";
         yield return "or";
         yield return "OR";
         yield return "or";
         yield return "AND";
         yield return "NOT";
         yield return "ACOS";
         yield return "ASIN";
         yield return "ATAN";
         yield return "COS";
         yield return "COSH";
         yield return "EXP";
         yield return "IF";
         yield return "LN";
         yield return "LOG";
         yield return "LOG10";
         yield return "MAX";
         yield return "MIN";
         yield return "RND";
         yield return "SINH";
         yield return "SQRT";
         yield return "TAN";
         yield return "TANH";
         yield return "SRND";
         yield return "x?";
         yield return "X:";
         yield return "222";
         yield return "-3";
         yield return "1e3";
         yield return "pi";
         yield return "Pi";
         yield return "pI";
         yield return "PI";
         yield return "e";
         yield return "E";
      }
   }
}