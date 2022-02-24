#include "NestedAddition_macros.h"
#include "NestedAddition.h"
#include "NestedAddition_private.h"

DW_NestedAddition_T NestedAddition_DW;
ExtU_NestedAddition_T NestedAddition_U;
ExtY_NestedAddition_T NestedAddition_Y;
static RT_MODEL_NestedAddition_T NestedAddition_M_;
RT_MODEL_NestedAddition_T *const NestedAddition_M = &NestedAddition_M_;
void NestedAddition_step(void)
{
  real_T realValue;
  real_T realValue_0;
  real_T realValue_1;
  real_T realValue_2;
  real_T realValue_3;
  real_T realValue_4;
  real_T stepError;
  if (NestedAddition_DW.FMU_FmuIsInitialized == 0) {
    NestedAddition_DW.FMU_FmuPrevTime = (((NestedAddition_M->Timing.clockTick0+
      NestedAddition_M->Timing.clockTickH0* 4294967296.0)) * 0.2);
    FMU2_enterInitializationMode(&NestedAddition_DW.FMU_FmuStruct);
    realValue = NestedAddition_U.X;
    FMU2_setReal(&NestedAddition_DW.FMU_FmuStruct, 0, 1, &realValue);
    realValue_0 = NestedAddition_U.Y;
    FMU2_setReal(&NestedAddition_DW.FMU_FmuStruct, 1, 1, &realValue_0);
    FMU2_exitInitializationMode(&NestedAddition_DW.FMU_FmuStruct);
    NestedAddition_DW.FMU_FmuIsInitialized = 1;
    FMU2_getReal(&NestedAddition_DW.FMU_FmuStruct, 2, 1, &realValue_3);
    stepError = realValue_3;
  } else {
    real_T stepsize;
    stepsize = (((NestedAddition_M->Timing.clockTick0+
                  NestedAddition_M->Timing.clockTickH0* 4294967296.0)) * 0.2) -
      NestedAddition_DW.FMU_FmuPrevTime;
    if (stepsize > 0.0) {
      stepError = stepsize - 0.2;
      if (stepsize - 0.2 < 0.0) {
        stepError = 0.2 - stepsize;
      }

      if (stepError < 1.1920928955078125E-7) {
        stepsize = 0.2;
      }

      FMU2_doStep(&NestedAddition_DW.FMU_FmuStruct,
                  NestedAddition_DW.FMU_FmuPrevTime, stepsize, 1);
      NestedAddition_DW.FMU_FmuPrevTime += stepsize;
      FMU2_getReal(&NestedAddition_DW.FMU_FmuStruct, 2, 1, &realValue_4);
      stepError = realValue_4;
    }
  }

  NestedAddition_Y.ERGEBNIS = stepError;
  realValue_1 = NestedAddition_U.X;
  FMU2_setReal(&NestedAddition_DW.FMU_FmuStruct, 0, 1, &realValue_1);
  realValue_2 = NestedAddition_U.Y;
  FMU2_setReal(&NestedAddition_DW.FMU_FmuStruct, 1, 1, &realValue_2);
  fmu_LogOutput();
  NestedAddition_M->Timing.clockTick0++;
  if (!NestedAddition_M->Timing.clockTick0) {
    NestedAddition_M->Timing.clockTickH0++;
  }
}

void NestedAddition_initialize(void)
{
  (void) memset((void *)NestedAddition_M, 0,
                sizeof(RT_MODEL_NestedAddition_T));
  (void) memset((void *)&NestedAddition_DW, 0,
                sizeof(DW_NestedAddition_T));
  (void)memset(&NestedAddition_U, 0, sizeof(ExtU_NestedAddition_T));
  NestedAddition_Y.ERGEBNIS = 0.0;

  {
    const char_T *dllLoc;
    const char_T *fmuInstanceName;
    const char_T *fmuLoc;
    boolean_T fmuLogging;
    boolean_T fmuVisible;
    const void *fmu2callbacks;
    dllLoc = fmu_nestedDLLLocation("94184ebd83e9074cf44fedc599ccb8c5", NULL,
      "Addition.dll");
    fmuInstanceName = fmu_instanceName();
    fmuLoc = fmu_nestedResourceLocation("94184ebd83e9074cf44fedc599ccb8c5", NULL,
      0);
    fmu2callbacks = fmu_callback();
    fmuVisible = fmu_fmuVisible();
    fmuLogging = fmu_fmuLogging();
    NestedAddition_DW.FMU_FmuStruct = FMU2_fmuInitializeCS(dllLoc,
      fmuInstanceName, "{8f600e4d-a6f4-c2ed-3558-a9cbedce34b9}", fmuLoc, NULL, 0,
      0, 0, fmu2callbacks, fmuVisible, fmuLogging);
    createParamIdxToOffset(&NestedAddition_DW.FMU_FmuStruct, 0);
    createEnumValueList(&NestedAddition_DW.FMU_FmuStruct, 0);
  }

  NestedAddition_DW.FMU_FmuIsInitialized = 0;
  FMU2_setupExperiment(&NestedAddition_DW.FMU_FmuStruct, false, 0.001,
                       (((NestedAddition_M->Timing.clockTick0+
    NestedAddition_M->Timing.clockTickH0* 4294967296.0)) * 0.2), false, 1000.0);
}

void NestedAddition_terminate(void)
{
  if (NestedAddition_DW.FMU_FmuStruct != NULL) {
    FMU2_terminate(&NestedAddition_DW.FMU_FmuStruct);
  }
}
