/* Copyright 2021 The MathWorks, Inc. */
/*
 * @file: RTWCG_FMU_util.h
 *  
 * @brief FMU path handling util function. This file is included during FMU code generation.
 *      
 */

#ifndef RTWCG_FMU_util_h
#define RTWCG_FMU_util_h



//current FMU unzip directory
#if FMU_CG_TARGET == 1
const char* fmu_resourceLocation();
#else
extern const char* fmu_resourceLocation();
#endif

#if FMU_CG_TARGET == 20
#include "fmi2Functions.h"
extern const void* fmu_callback();
extern fmi2Boolean fmu_fmuVisible();
extern fmi2Boolean fmu_fmuLogging();
extern fmi2String  fmu_instanceName();
#endif

//nested FMU dll and resources directory
char* fmu_nestedDLLLocation(const char* uid, const char* model, const char* dllName);
char* fmu_nestedResourceLocation(const char* uid, const char* model, int root);

//convert uri to local path
char* uriToLocal(const char *uri);

#endif
