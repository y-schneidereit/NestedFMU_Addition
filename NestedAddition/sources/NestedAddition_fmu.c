#include "NestedAddition_macros.h"
/*
 * Copyright 2014-2015 The MathWorks, Inc.
 *
 * File    : NestedAddition_fmu.c
 *
 * Abstract:
 *      An FMI API wrapper for grt code generated by Simulink.
 *
 */

/*==================*
 * Required defines *
 *==================*/

#ifndef MODEL
# error Must specify a model name.  Define MODEL=name.
#else
/* create generic macros that work with any model */
# define EXPAND_CONCAT(name1,name2) name1 ## name2
# define CONCAT(name1,name2) EXPAND_CONCAT(name1,name2)
# define MODEL_INITIALIZE CONCAT(MODEL,_initialize)
# define MODEL_STEP       CONCAT(MODEL,_step)
# define MODEL_TERMINATE  CONCAT(MODEL,_terminate)
# define RT_MDL_TYPE      RT_MODEL_NestedAddition_T
#endif
#define QUOTE1(name) #name
#define QUOTE(name) QUOTE1(name)    /* need to expand name    */
#ifndef FMI2_FUNCTION_PREFIX
#define FMI2_FUNCTION_PREFIX NestedAddition_
#endif
/*==========*
 * Includes *
 *==========*/
#include <math.h>
#include <time.h>
#include <stdio.h>   /* optional for printf */
#include <string.h>  /* optional for strcmp */
#include "rtmodel.h" /* optional for automated builds */
#include "NestedAddition.h"
#include "NestedAddition_private.h"
#include "fmi2Functions.h"
#include "RTWCG_FMU_util.h"
/*====================*
 * External functions *
 *====================*/
extern void MODEL_INITIALIZE();
extern void MODEL_TERMINATE();
extern void MODEL_STEP();
/*==================================*
 * Global data local to this module *
 *==================================*/
const char *RT_MEMORY_ALLOCATION_ERROR = "memory allocation error"; 
typedef struct {
    /* Model data */
    RT_MDL_TYPE *S;
    fmi2EventInfo eventInfo;
    /* FMU data*/
    fmi2String instanceName;
    fmi2Type type;
    fmi2String GUID;
    fmi2String resourceLocation;
    const fmi2CallbackFunctions *functions;
    fmi2Boolean visible;
    fmi2Boolean loggingOn;
    fmi2Real time;
    /* Variable Indices */
    real_T **rList;
    int32_T **iList;
    boolean_T **bList;
    char **sList;
    int32_T *sSize;
    /* Output Buffers */
    real_T *rOutput;
    int32_T *iOutput;
    boolean_T *bOutput;
    char **sOutput;
} FMUModelData;
FMUModelData *_instance = NULL;
void fmu_LogOutput() {
    FMUModelData *modelData = (FMUModelData *)_instance;
modelData->rOutput[0] = *(modelData->rList[2]);
}
void fmu_RestoreOutput() {
    FMUModelData *modelData = (FMUModelData *)_instance;
*(modelData->rList[2]) = modelData->rOutput[0];
}
const char* fmu_resourceLocation() {
    FMUModelData *modelData = (FMUModelData *)_instance;
    return (const char *)modelData->resourceLocation;
}

const void* fmu_callback() {
    FMUModelData *modelData = (FMUModelData *)_instance;
    return (const void *)modelData->functions;
}

fmi2Boolean fmu_fmuVisible() {
    FMUModelData *modelData = (FMUModelData *)_instance;
    return modelData->visible;
}

fmi2Boolean fmu_fmuLogging() {
    FMUModelData *modelData = (FMUModelData *)_instance;
    return modelData->loggingOn;
}

fmi2String fmu_instanceName() {
    FMUModelData *modelData = (FMUModelData *)_instance;
    return modelData->instanceName;
}

/*====================*
 *  Static functions  *
 *====================*/
static void rtOneStep(RT_MDL_TYPE *S) {
    (void)S;
    MODEL_STEP();
    fmu_RestoreOutput();
}
/*===============*
 * FMI functions *
 *===============*/

const char* fmi2GetTypesPlatform() {
    static char str[] = {'d','e','f','a','u','l','t','\0'};
    return str;
}

const char* fmi2GetVersion() {
    static char str[] = {'2','.','0','\0'};
    return str;
}

fmi2Status fmi2SetDebugLogging(fmi2Component c, fmi2Boolean loggingOn, size_t nCategories, const fmi2String categories[]) {  
    FMUModelData *modelData = (FMUModelData *)c;
    (void)nCategories;
    (void)categories;

    modelData->loggingOn = loggingOn;
    return fmi2OK;
}

fmi2Component fmi2Instantiate(fmi2String instanceName, fmi2Type fmuType, fmi2String fmuGUID,
                            fmi2String fmuResourceLocation, const fmi2CallbackFunctions *functions,
                            fmi2Boolean visible, fmi2Boolean loggingOn) {

    FMUModelData *modelData = NULL;
    const char_T *errmsg = NULL;

    if (!functions->logger) {
        return NULL;
    } else if (!instanceName || strlen(instanceName) == 0) {
        functions->logger(functions->componentEnvironment, "?", fmi2Error, "error",
                "fmi2Instantiate: Missing instance name.");
        return NULL;
    } else if (!fmuGUID || strlen(fmuGUID) == 0) {
        functions->logger(functions->componentEnvironment, instanceName, fmi2Error, "error",
                "fmi2Instantiate: Missing GUID.");
        return NULL;
    } else if (strcmp(fmuGUID, "{80a437cd-34fd-95f1-ca10-1ac311c31b97}")) {
        functions->logger(functions->componentEnvironment, instanceName, fmi2Error, "error",
                "fmi2Instantiate: Wrong GUID %s. Expected %s.", fmuGUID, "{80a437cd-34fd-95f1-ca10-1ac311c31b97}");
        return NULL;
    }
    _instance = modelData = (FMUModelData*)calloc(1, sizeof(FMUModelData));
    if (!modelData) {
        functions->logger(functions->componentEnvironment, instanceName, fmi2Error, "error",
                "fmi2Instantiate: Memory allocation error.");
        return NULL;
    }
    modelData->S = NestedAddition_M;
    if (modelData->S == NULL) {
        functions->logger(functions->componentEnvironment, instanceName, fmi2Error, "error",
                "Memory allocation error during model registration");
        return NULL;
    }
    errmsg = (const char_T *) (rtmGetErrorStatus(modelData->S));
    if (errmsg != NULL) {
        functions->logger(functions->componentEnvironment, instanceName, fmi2Error, "error",
                "Error during model registration: %s\n", errmsg);
        MODEL_TERMINATE();
        return NULL;
    }
    modelData->eventInfo.newDiscreteStatesNeeded = fmi2False;
    modelData->eventInfo.terminateSimulation = fmi2False;
    modelData->eventInfo.nominalsOfContinuousStatesChanged = fmi2False;
    modelData->eventInfo.valuesOfContinuousStatesChanged = fmi2False;
    modelData->eventInfo.nextEventTimeDefined = fmi2False;
    modelData->eventInfo.nextEventTime = 0;
    
    modelData->instanceName = (fmi2String)calloc(strlen(instanceName)+1, sizeof(fmi2Char*));
    if (!modelData->instanceName) {
        functions->logger(functions->componentEnvironment, instanceName, fmi2Error, "error",
                "fmi2Instantiate: Memory allocation error.");
        return NULL;
    }
    strcpy((char *)modelData->instanceName, (char *)instanceName);
    modelData->type = fmuType;
    modelData->GUID = (fmi2String)calloc(strlen(fmuGUID)+1, sizeof(fmi2Char*));
    if (!modelData->GUID) {
        functions->logger(functions->componentEnvironment, instanceName, fmi2Error, "error",
                "fmi2Instantiate: Memory allocation error.");
        return NULL;
    }
    strcpy((char *)modelData->GUID, (char *)fmuGUID);
    if (fmuResourceLocation) {
        modelData->resourceLocation = uriToLocal(fmuResourceLocation);
        if (!modelData->resourceLocation) {
            functions->logger(functions->componentEnvironment, instanceName, fmi2Error, "error",
                    "fmi2Instantiate: Memory allocation error.");
            return NULL;
        }
        else if (strlen(modelData->resourceLocation) == 0) {
            functions->logger(functions->componentEnvironment, instanceName, fmi2Error, "error",
                    "fmi2Instantiate: uriToLocal cannot handle fmuResourceLocation.");
            return NULL;
        }
    } else {
        modelData->resourceLocation = fmuResourceLocation;
    }
    modelData->functions = functions;
    modelData->visible = visible;
    modelData->loggingOn = loggingOn;
    modelData->time = 0.0;
    /* initialize variables */
    if (rtmGetStopRequested(modelData->S)) {
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                fmi2Error, "error", "stop requested in fmi2Instantiate");
        return NULL;
    }
    modelData->rList = (real_T**)calloc(4, sizeof(real_T*));
    if (!modelData->rList) {
        functions->logger(functions->componentEnvironment, instanceName, fmi2Error, "error",
                "fmi2Instantiate: Memory allocation error.");
        return NULL;
    }
    modelData->rOutput = (real_T*)calloc(1, sizeof(real_T));
    if (!modelData->rOutput) {
        functions->logger(functions->componentEnvironment, instanceName, fmi2Error, "error",
                "fmi2Instantiate: Memory allocation error.");
        return NULL;
    }
    modelData->rList[0] = &(NestedAddition_U.X);
    modelData->rList[1] = &(NestedAddition_U.Y);
    modelData->rList[2] = &(NestedAddition_Y.ERGEBNIS);
    modelData->rList[3] = &(modelData->time);

    return modelData;
}

void fmi2FreeInstance(fmi2Component c) {
    FMUModelData *modelData = (FMUModelData *)c;
    const char_T *errStatus = NULL;
    
    if (!modelData) return;
    errStatus = (const char_T *) (rtmGetErrorStatus(modelData->S));
    
    if (errStatus != NULL && strcmp(errStatus, "Simulation finished")) {
        modelData->functions->logger(modelData->functions->componentEnvironment, 
                modelData->instanceName, fmi2Error, "error", "%s", errStatus);

        MODEL_TERMINATE();
        return;
    }    
    MODEL_TERMINATE();
    
    free((void*)modelData->instanceName);
    free((void*)modelData->GUID);
    free((void*)modelData->resourceLocation);
    free((void*)modelData->rList);
    free((void*)modelData->rOutput);
    free((void*)modelData);
}

fmi2Status fmi2SetupExperiment(fmi2Component c, fmi2Boolean toleranceDefined, fmi2Real tolerance,
                            fmi2Real startTime, fmi2Boolean stopTimeDefined, fmi2Real stopTime) {

    FMUModelData *modelData = (FMUModelData *)c;
    (void)toleranceDefined;
    (void)tolerance;
    (void)stopTimeDefined;
    (void)stopTime;

    if (fabs(startTime) > 1.0E-7) {
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                fmi2Error, "error", "start time is not zero.");
        return fmi2Error;
    }

    modelData->time = startTime;
    return fmi2OK;
}

fmi2Status fmi2EnterInitializationMode(fmi2Component c) {
   FMUModelData *modelData = (FMUModelData *)c;
   {MODEL_INITIALIZE();}
    (void)c;
    return fmi2OK;
}

fmi2Status fmi2ExitInitializationMode(fmi2Component c) {
    FMUModelData *modelData = (FMUModelData *)c;

    /* call step after setting input variables*/
    rtOneStep(modelData->S);
    if (rtmGetStopRequested(modelData->S)) {
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                fmi2Error, "error", "stop requested in fmi2ExitInitializationMode");
        return fmi2Error;
    }
    return fmi2OK;
}

fmi2Status fmi2Terminate(fmi2Component c) {
    (void)c;
    return fmi2OK;
}

fmi2Status fmi2Reset(fmi2Component c) {
    FMUModelData *modelData = (FMUModelData *)c;
    const char_T *errmsg = (const char_T *) (rtmGetErrorStatus(modelData->S));
    /* Reset model data */
    if (errmsg != NULL && strcmp(errmsg, "Simulation finished")) {
        modelData->functions->logger(modelData->functions->componentEnvironment, 
                modelData->instanceName, fmi2Error, "error", "%s", errmsg);

        MODEL_TERMINATE();
        return fmi2Error;
    }    
    MODEL_TERMINATE();
    
    modelData->S = NestedAddition_M;
    if (modelData->S == NULL) {
        modelData->functions->logger(modelData->functions->componentEnvironment, 
               modelData->instanceName, fmi2Error, "error",
               "Memory allocation error during model registration");
        return fmi2Error;
    }
    errmsg = (const char_T *) (rtmGetErrorStatus(modelData->S));
    if (errmsg != NULL) {
        modelData->functions->logger(modelData->functions->componentEnvironment, 
                modelData->instanceName, fmi2Error, "error",
                "Error during model registration: %s\n", errmsg);
        MODEL_TERMINATE();
        return fmi2Error;
    }
    modelData->eventInfo.newDiscreteStatesNeeded = fmi2False;
    modelData->eventInfo.terminateSimulation = fmi2False;
    modelData->eventInfo.nominalsOfContinuousStatesChanged = fmi2False;
    modelData->eventInfo.valuesOfContinuousStatesChanged = fmi2False;
    modelData->eventInfo.nextEventTimeDefined = fmi2False;
    modelData->eventInfo.nextEventTime = 0;
    modelData->rList[0] = &(NestedAddition_U.X);
    modelData->rList[1] = &(NestedAddition_U.Y);
    modelData->rList[2] = &(NestedAddition_Y.ERGEBNIS);
    modelData->rList[3] = &(modelData->time);
    if (rtmGetStopRequested(modelData->S)) {
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                fmi2Error, "error", "stop requested in fmi2Reset");
        return fmi2Error;
    }
    return fmi2OK;
}

fmi2Status fmi2GetReal (fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Real value[]) {
    FMUModelData *modelData = (FMUModelData *)c;
    size_t i;
    if (nvr != 0 && (!value || !vr)) {
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "invalid memory access");
        return fmi2Error;
    }
    for (i = 0; i < nvr; i++) {
        if (vr[i] >= 4.0) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "valueReference out of range");
            return fmi2Error;
        }
    }
    for (i = 0; i < nvr; i++) {
        value[i] = (fmi2Real)(*modelData->rList[vr[i]]);
        if (modelData->loggingOn) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
               fmi2OK, "info", "getReal vr:%d, value:%lf", vr[i], value[i]);
        }
    }
    return fmi2OK;
}

fmi2Status fmi2SetReal (fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Real value[]) {
    FMUModelData *modelData = (FMUModelData *)c;
    size_t i;
    bool is_diff_value = false;
    if (nvr != 0 && (!value || !vr)) {
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "invalid memory access");
        return fmi2Error;
    }
    for (i = 0; i < nvr; i++) {
        if (vr[i] >= 4.0) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "valueReference out of range");
            return fmi2Error;
        }
        if (*modelData->rList[vr[i]] != (real_T)(value[i])) {
            is_diff_value = true;
            *modelData->rList[vr[i]] = (real_T)(value[i]);
        }
        if (modelData->loggingOn) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
               fmi2OK, "info", "setReal vr:%d, value:%lf", vr[i], value[i]);
        }
    }
    return fmi2OK;
}

fmi2Status fmi2GetInteger(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Integer value[]) {
    FMUModelData *modelData = (FMUModelData *)c;
    size_t i;    
    if (nvr != 0 && (!value || !vr)) {
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "invalid memory access");
        return fmi2Error;
    }
    for (i = 0; i < nvr; i++) {
        if (vr[i] >= 0.0) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "valueReference out of range");
            return fmi2Error;
        }
    }
    for (i = 0; i < nvr; i++) {
        value[i] = (fmi2Integer)(*modelData->iList[vr[i]]);
        if (modelData->loggingOn) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
               fmi2OK, "info", "getInteger vr:%d, value:%d", vr[i], value[i]);
        }
    }
    return fmi2OK;
}

fmi2Status fmi2SetInteger(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Integer value[]) {
    FMUModelData *modelData = (FMUModelData *)c;
    size_t i;    
    bool is_diff_value = false;
    if (nvr != 0 && (!value || !vr)) {
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "invalid memory access");
        return fmi2Error;
    }
    for (i = 0; i < nvr; i++) {
        if (vr[i] >= 0.0) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "valueReference out of range");
            return fmi2Error;
        }
        if (*modelData->iList[vr[i]] != (int32_T)(value[i])) {
            is_diff_value = true;
            *modelData->iList[vr[i]] = (int32_T)(value[i]);
        }
        if (modelData->loggingOn) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
               fmi2OK, "info", "setInteger vr:%d, value:%d", vr[i], value[i]);
        }
    }
    return fmi2OK;
}

fmi2Status fmi2GetBoolean(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Boolean value[]) {
    FMUModelData *modelData = (FMUModelData *)c;
    size_t i;    
    if (nvr != 0 && (!value || !vr)) {
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "invalid memory access");
        return fmi2Error;
    }
    for (i = 0; i < nvr; i++) {
        if (vr[i] >= 0.0) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "valueReference out of range");
            return fmi2Error;
        }
    }
    for (i = 0; i < nvr; i++) {
        value[i] = (*modelData->bList[vr[i]] == false)?fmi2False:fmi2True;
        if (modelData->loggingOn) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
               fmi2OK, "info", "getBoolean vr:%d, value:%u", vr[i], value[i]);
        }
    }
    return fmi2OK;
}

fmi2Status fmi2SetBoolean(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Boolean value[]) {
    FMUModelData *modelData = (FMUModelData *)c;
    size_t i;    
    bool is_diff_value = false;
    if (nvr != 0 && (!value || !vr)) {
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "invalid memory access");
        return fmi2Error;
    }
    for (i = 0; i < nvr; i++) {
        if (vr[i] >= 0.0) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "valueReference out of range");
            return fmi2Error;
        }
        if (*modelData->bList[vr[i]] != ((value[i]==fmi2False)?false:true)) {
            is_diff_value = true;
            *modelData->bList[vr[i]] = (value[i]==fmi2False)?false:true;
        }
        if (modelData->loggingOn) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
               fmi2OK, "info", "setBoolean vr:%d, value:%u", vr[i], value[i]);
        }
    }
    return fmi2OK;
}

fmi2Status fmi2GetString (fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2String value[]) {
    FMUModelData *modelData = (FMUModelData *)c;
    size_t i;    
    if (nvr != 0 && (!value || !vr)) {
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "invalid memory access");
        return fmi2Error;
    }
    for (i = 0; i < nvr; i++) {
        if (vr[i] >= 0.0) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "valueReference out of range");
            return fmi2Error;
        }
    }
    for (i = 0; i < nvr; i++) {
        value[i] = modelData->sList[vr[i]];
        if (modelData->loggingOn) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
               fmi2OK, "info", "getString vr:%d, value:%s", vr[i], value[i]);
        }
    }
    return fmi2OK;
}

fmi2Status fmi2SetString (fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2String value[]) {
    FMUModelData *modelData = (FMUModelData *)c;
    size_t i;    
    bool is_diff_value = false;
    size_t size;
    if (nvr != 0 && (!value || !vr)) {
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "invalid memory access");
        return fmi2Error;
    }
    for (i = 0; i < nvr; i++) {
       if (vr[i] >= 0.0) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "valueReference out of range");
            return fmi2Error;
        }
        if (strcmp(modelData->sList[vr[i]], value[i]) != 0) {
            is_diff_value = true;
            size = strlen(value[i]) < (modelData->sSize[vr[i]] - 1) ? strlen(value[i]) : (modelData->sSize[vr[i]] - 1);
            strncpy(modelData->sList[vr[i]], value[i], size);
            *((char*)modelData->sList[vr[i]]+size) = '\x00';
        }
        if (modelData->loggingOn) {
            modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
               fmi2OK, "info", "setString vr:%d, value:%s", vr[i], value[i]);
        }
    }
    return fmi2OK;
}

fmi2Status fmi2GetFMUstate (fmi2Component c, fmi2FMUstate* FMUstate) {
    (void)c;
    (void)FMUstate;
    return fmi2OK;
}

fmi2Status fmi2SetFMUstate (fmi2Component c, fmi2FMUstate FMUstate) {
    (void)c;
    (void)FMUstate;
    return fmi2OK;
}

fmi2Status fmi2FreeFMUstate(fmi2Component c, fmi2FMUstate* FMUstate) {
    (void)c;
    (void)FMUstate;
    return fmi2OK;
}

fmi2Status fmi2SerializedFMUstateSize(fmi2Component c, fmi2FMUstate FMUstate, size_t *size) {
    (void)c;
    (void)FMUstate;
    (void)size;
    return fmi2OK;
}

fmi2Status fmi2SerializeFMUstate (fmi2Component c, fmi2FMUstate FMUstate, fmi2Byte serializedState[], size_t size) {
    (void)c;
    (void)FMUstate;
    (void)serializedState;
    (void)size;
    return fmi2OK;
}

fmi2Status fmi2DeSerializeFMUstate (fmi2Component c, const fmi2Byte serializedState[], size_t size,
                                    fmi2FMUstate* FMUstate) {
    (void)c;
    (void)serializedState;
    (void)size;
    (void)FMUstate;
    return fmi2OK;
}

fmi2Status fmi2GetDirectionalDerivative(fmi2Component c, const fmi2ValueReference vUnknown_ref[], size_t nUnknown,
                                        const fmi2ValueReference vKnown_ref[] , size_t nKnown,
                                        const fmi2Real dvKnown[], fmi2Real dvUnknown[]) {
    (void)c;
    (void)vUnknown_ref;
    (void)nUnknown;
    (void)vKnown_ref;
    (void)nKnown;
    (void)dvKnown;
    (void)dvUnknown;
    return fmi2OK;
}

/* ---------------------------------------------------------------------------
 * Functions for FMI for Co-Simulation
 * ---------------------------------------------------------------------------
 */

/* Simulating the slave */
fmi2Status fmi2SetRealInputDerivatives(fmi2Component c, const fmi2ValueReference vr[], size_t nvr,
                                     const fmi2Integer order[], const fmi2Real value[]) {
    (void)c;
    (void)vr;
    (void)nvr;
    (void)order;
    (void)value;
    return fmi2OK;
}

fmi2Status fmi2GetRealOutputDerivatives(fmi2Component c, const fmi2ValueReference vr[], size_t nvr,
                                      const fmi2Integer order[], fmi2Real value[]) {
    size_t i;
    (void)c;
    (void)vr;
    (void)order;
    for (i = 0; i < nvr; i++) value[i] = 0;
    return fmi2OK;
}

fmi2Status fmi2CancelStep(fmi2Component c) {
    (void)c;
    return fmi2OK;
}

fmi2Status fmi2DoStep(fmi2Component c, fmi2Real currentCommunicationPoint,
                    fmi2Real communicationStepSize, fmi2Boolean noSetFMUStatePriorToCurrentPoint) {
    FMUModelData *modelData = (FMUModelData *)c;
    int steps = 0;
    int i;
    fmi2Status returnStatus = fmi2OK;
    
    (void)noSetFMUStatePriorToCurrentPoint;
    steps = (int)(communicationStepSize / (double)0.2 + 0.5);    
    modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
            fmi2OK, "info", "CommunicationStepSize=%.16g, LocalSolverStepSize=%.16g", communicationStepSize, (double)0.2);
    if (communicationStepSize < 0 || fabs(communicationStepSize - steps * (double)0.2) > 1.0E-6 * (double)0.2) {
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "Stepsize must be non-negative and divisible by %.16g", (double)0.2);
        returnStatus = fmi2Error;
    } else if (fabs(modelData->time - currentCommunicationPoint) > 1.0E-6 * (double)0.2) {
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                    fmi2Error, "error", "CurrentCommunicationPoint is not equal to FMU time");
        returnStatus = fmi2Error;
    } else {
        modelData->time = currentCommunicationPoint;
        modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                fmi2OK, "info", "Local solver will do %d steps from t = %.16g.", steps, currentCommunicationPoint);
        for (i = 0; i < steps; i++) {
            if (rtmGetStopRequested(modelData->S)) {
                modelData->functions->logger(modelData->functions->componentEnvironment, modelData->instanceName, 
                        fmi2Error, "error", "stop requested in fmi2DoStep");
                returnStatus = fmi2Error;
                break;
            }
            rtOneStep(modelData->S);
            modelData->time += (double)0.2;
        }
    }
    
    if (modelData->functions->stepFinished) {
        modelData->functions->stepFinished(modelData->functions->componentEnvironment, returnStatus);
    }
    return returnStatus;
}

fmi2Status fmi2GetStatus(fmi2Component c, const fmi2StatusKind s, fmi2Status *value) {
    (void)c;
    (void)s;
    (void)value;
    return fmi2OK;
}

fmi2Status fmi2GetRealStatus(fmi2Component c, const fmi2StatusKind s, fmi2Real *value) {
    (void)c;
    (void)s;
    (void)value;
    return fmi2OK;
}

fmi2Status fmi2GetIntegerStatus(fmi2Component c, const fmi2StatusKind s, fmi2Integer *value) {
    (void)c;
    (void)s;
    (void)value;
    return fmi2OK;
}

fmi2Status fmi2GetBooleanStatus(fmi2Component c, const fmi2StatusKind s, fmi2Boolean *value) {
    (void)c;
    (void)s;
    (void)value;
    return fmi2OK;
}

fmi2Status fmi2GetStringStatus(fmi2Component c, const fmi2StatusKind s, fmi2String *value) {
    (void)c;
    (void)s;
    (void)value;
    return fmi2OK;
}

/* ---------------------------------------------------------------------------
 * Functions for FMI for Model Exchange
 * ---------------------------------------------------------------------------
 */

fmi2Status fmi2CompletedIntegratorStep(fmi2Component c, fmi2Boolean noSetFMUStatePriorToCurrentPoint, fmi2Boolean* enterEventMode, fmi2Boolean* terminateSimulation) {
    (void)c;
    (void)noSetFMUStatePriorToCurrentPoint;
    (void)enterEventMode;
    (void)terminateSimulation;
    return fmi2OK;
}

fmi2Status fmi2EnterContinuousTimeMode(fmi2Component c) {
    (void)c;
    return fmi2OK;
}

fmi2Status fmi2EnterEventMode(fmi2Component c) {
    (void)c;
    return fmi2OK;
}

fmi2Status fmi2GetContinuousStates(fmi2Component c, fmi2Real x[], size_t nx) {
    (void)c;
    (void)x;
    (void)nx;
    return fmi2OK;
}

fmi2Status fmi2GetDerivatives(fmi2Component c, fmi2Real derivatives[], size_t nx) {
    (void)c;
    (void)derivatives;
    (void)nx;
    return fmi2OK;
}

fmi2Status fmi2GetEventIndicators(fmi2Component c, fmi2Real eventIndicators[], size_t ni) {
    (void)c;
    (void)eventIndicators;
    (void)ni;
    return fmi2OK;
}

fmi2Status fmi2GetNominalsOfContinuousStates(fmi2Component c, fmi2Real x_nominal[], size_t nx) {
    (void)c;
    (void)x_nominal;
    (void)nx;
    return fmi2OK;
}

fmi2Status fmi2NewDiscreteStates(fmi2Component c, fmi2EventInfo* fmi2eventInfo) {
    (void)c;
    (void)fmi2eventInfo;
    return fmi2OK;
}

fmi2Status fmi2SetContinuousStates(fmi2Component c, const fmi2Real x[], size_t nx) {
    (void)c;
    (void)x;
    (void)nx;
    return fmi2OK;
}

fmi2Status fmi2SetTime(fmi2Component c, fmi2Real time) {
    (void)c;
    (void)time;
    return fmi2OK;
}
