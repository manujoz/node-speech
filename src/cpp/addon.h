#pragma once

#include "includes.h"
#include "globals.h"
#include "utils.h"
#include "managedSpeech.h"

using namespace CsNodeSpeech;

struct _func_result
{
    napi_env env;
    napi_value global;
    napi_ref func;
    napi_threadsafe_function funcTh;
} FuncResult;

System::String^ dispatchEventFromCs(System::String^ data, System::String^ evName);
