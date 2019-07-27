using System;
using System.Collections.Generic;
using OSPSuite.FuncParser;

namespace ConsoleApp1
{
   class Program
   {
      static void Main(string[] args)
      {
         try
         {
            Test1();
            //Test2();
            //Test3();
         }
         catch (Exception e)
         {
            Console.WriteLine(e);
         }

         Console.WriteLine("Press Enter");
         Console.ReadLine();
      }

      static void Test3()
      {
         var pf = new ParsedFunction();
         var stringsToParse = new[] {"1e-3", "1.25e-4", "-2.2e-2", "1e+3", "1.25e+2", "-2.2e+2"};
         foreach (var stringToParse in stringsToParse)
         {
            pf.StringToParse = stringToParse;
            pf.Parse();
            var d = pf.CalcExpression(null);
            Console.WriteLine($"f({stringToParse}) = {d}");
         }
      }

      static void Test2()
      {
         double p1 = 1, p2 = 2, p3 = 3, x = 3, y = 4;
         var pf = new ParsedFunction();

         var caseSensitive = pf.CaseSensitive;
         pf.CaseSensitive = !caseSensitive;
         caseSensitive = pf.CaseSensitive;
         Console.WriteLine($"CaseSensitive={caseSensitive}");

         pf.SetVariableNames(new[] { "x", "y" });
         pf.SetParameterNames(new[] { "p1", "p2", "p3" });
         pf.SetParameterValues(new[] { p1, p2, p3 });
         pf.SetParametersNotToSimplify(new[] { "p3" });
         pf.SimplifyParametersAllowed = true;
         pf.LogicOperatorsAllowed = false;
         pf.LogicalNumericMixAllowed = false;
         pf.ComparisonTolerance = 0.1;
         pf.StringToParse = "2*p1+3*p2+p3+x*y";
         var stringToParse = pf.StringToParse;

         pf.Parse();
         Console.WriteLine("Parse: OK");

         var value = pf.CalcExpression(new[] { x, y });
         Console.WriteLine($"p1={p1} p2={p2} p3={p3} x={x} y={y}\n");
         Console.WriteLine($"{stringToParse} = {value}\n");

         var xmlString = pf.GetXMLString();
         Console.WriteLine($"{xmlString}\n");
         pf = null;

         pf = new ParsedFunction();
         pf.SetVariableNames(new[] { "x", "y" });
         pf.StringToParse = "x+t";
         pf.Parse();
      }

      static void Test1()
      {
         var startTime = DateTime.UtcNow;

         const int NUMBER_ITERATIONS = 1000;
         const int NUMBER_VARIABLES = 100;

         var variableNames = new List<string>();
         var variableValues = new List<double>();

         var stringToParse = "";
         for (var variableIdx = 1; variableIdx <= NUMBER_VARIABLES; variableIdx++)
         {
            var variable = $"V{variableIdx}";
            variableNames.Add(variable);
            stringToParse += variable + (variableIdx < NUMBER_VARIABLES ? "+" : "");

            variableValues.Add(variableIdx);
         }

         for (var iterationIdx = 0; iterationIdx < NUMBER_ITERATIONS; iterationIdx++)
         {
            var pf = new ParsedFunction();
            pf.SetVariableNames(variableNames);
            pf.StringToParse = stringToParse;

            //pf.Parse();
            var xmlString = pf.GetXMLString();
            double value = pf.CalcExpression(variableValues);

            pf = null;
         }

         var endTime = DateTime.UtcNow;
         TimeSpan timeDiff = endTime - startTime;
         Console.WriteLine($"Duration = {timeDiff.TotalSeconds} seconds");

         //         Console.WriteLine($"{value}\n{xmlString}\n");
      }
   }
}
