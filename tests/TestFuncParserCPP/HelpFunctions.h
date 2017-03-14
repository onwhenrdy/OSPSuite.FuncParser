#ifndef __HelpFunctions_h_
#define __HelpFunctions_h_


#include "FuncParser/FuncParserErrorData.h"
#include "FuncParser/Math.h"
#include "FuncParser/ParsedFunction.h"
#include "FuncParser/FuncParserTypeDefs.h"
#include <string>
#include <vector>

#include <stdio.h>
#include <iostream>

#ifdef linux
#include <fstream>
#include <stdlib.h>
#else
#include <fstream>
#endif

using namespace FuncParserNative;

void MsgErrED(FuncParserErrorData & ED);

bool CheckError(FuncParserErrorData & ED);

std::string GetInputLine(void);

#endif
