#include "HelpFunctions.h"

#define CHECK_TIME_PARSE
#define CHECK_TIME_PARSE_LOOP 1000

#ifdef linux
#include <fstream>
#endif

using namespace std;

int main(void)
{
	StringVector VariableNames, ParameterNames;
	DoubleVector VariableValues, ParameterValues;
	FuncParserErrorData ED;
	ParsedFunction pFunc;
	double dValue;
	std::string sCommand;

//	CoInitialize(NULL); //Required for XML String generation ONLY !!!

	VariableNames.push_back("x");   VariableValues.push_back(1);
	VariableNames.push_back("y");   VariableValues.push_back(2);
	VariableNames.push_back("z");   VariableValues.push_back(3);
	ParameterNames.push_back("p1"); ParameterValues.push_back(4);
	ParameterNames.push_back("p2"); ParameterValues.push_back(5);

	pFunc.SetCaseSensitive(true, ED); CheckError(ED);
	pFunc.SetLogicOperatorsAllowed(true, ED); CheckError(ED);
	pFunc.SetSimplifyParametersAllowed(true, ED); CheckError(ED);
	pFunc.SetVariableNames(VariableNames,ED);CheckError(ED);
	pFunc.SetParameterNames(ParameterNames, ED); CheckError(ED);
	pFunc.SetParameterValues(ParameterValues, ED); CheckError(ED);
	pFunc.SetLogicOperatorsAllowed(true,ED); CheckError(ED);
	pFunc.SetComparisonTolerance(0.0,ED); CheckError(ED);

	while(1)
	{
		std::cout<<"f(x=1,y=2,z=3;p1=4,p2=5) = ";
		sCommand=GetInputLine();
		std::cout<<"Processing: '"<<sCommand.c_str()<<"'"<<std::endl;

		if (sCommand=="exit")
			break;
		pFunc.SetStringToParse(sCommand, ED);
		if(!CheckError(ED)) continue;
		pFunc.Parse(ED);
		if(!CheckError(ED)) continue;

		std::string xmlstring = pFunc.GetXMLString(ED);
		if(!CheckError(ED)) continue;
		ofstream outfile(".\\xmlout.xml");
		outfile<<xmlstring.c_str();
		outfile.close();

		dValue=pFunc.CalcExpression(VariableValues, ED);
		if(!CheckError(ED)) continue;

		if(Math::IsFinite(dValue))
			std::cout<<dValue<<std::endl;
		else
		{
			if(Math::IsNaN(dValue))
				std::cout<<"NaN"<<std::endl;
			else if(Math::IsInf(dValue))
				std::cout<<"Inf"<<std::endl;
			else if(Math::IsNegInf(dValue))
				std::cout<<"-Inf"<<std::endl;
		}
	}
}
