#include "HelpFunctions.h"

void MsgErrED(FuncParserErrorData & ED)
{
	std::string ErrorMsg;
	switch(ED.GetNumber())
	{
		case FuncParserErrorData::err_RUNTIME:
			ErrorMsg="err_RUNTIME";
			break;
		case FuncParserErrorData::err_PARSE:
			ErrorMsg="err_PARSE";
			break;
		case FuncParserErrorData::err_BADARG:
			ErrorMsg="err_BADARG";
			break;
		case FuncParserErrorData::err_BADCALL:
			ErrorMsg="err_BADCALL";
			break;
		case FuncParserErrorData::err_NUMERIC:
			ErrorMsg="err_NUMERIC";
			break;
		case FuncParserErrorData::err_OK:
			break;
	}
	ErrorMsg=ErrorMsg+"\nin "+ED.GetSource()+"\n"+ED.GetDescription();
	std::cout<<ErrorMsg.c_str()<<std::endl;
}

bool CheckError(FuncParserErrorData & ED)
{
	if(ED.GetNumber() != FuncParserErrorData::err_OK)
	{
		MsgErrED(ED);
		return false;
	}
	return true;
}

std::string GetInputLine(void)
{
	std::string sCommand="";

	while(1)
	{
		std::getline(std::cin, sCommand,'\n');
		if(sCommand=="") continue;
		break;
	}

	return sCommand;
}


