fp=NET.addAssembly('C:\Program Files (x86)\BTS Products\PK-Sim 6.0\FuncParser5_0.dll');

variables=NET.createArray('System.String', 2);
variables(1)='x';
variables(2)='y';

parameters=NET.createArray('System.String', 2);
parameters(1)='p1';
parameters(2)='p2';

parametervalues=NET.createArray('System.Double', 2);
parametervalues(1)=1;
parametervalues(2)=2;

%optional: this parameters will not be replaced with their value
paramsNotToSimplify=NET.createArray('System.String', 1);
paramsNotToSimplify(1)='p1';

f=FuncParserNET.ParsedFunction;

f.CaseSensitive=true;
f.LogicOperatorsAllowed=true;
f.ComparisonTolerance=1e-10;
f.SimplifyParametersAllowed=true;

f.VariableNames=variables;
f.ParameterNames=parameters;
f.ParameterValues=parametervalues;
f.ParametersNotToSimplify=paramsNotToSimplify;

f.StringToParse='p1*x+p2*y';
xml=f.XMLString;

