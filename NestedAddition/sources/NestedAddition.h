#ifndef RTW_HEADER_NestedAddition_h_
#define RTW_HEADER_NestedAddition_h_
#include <string.h>
#ifndef NestedAddition_COMMON_INCLUDES_
#define NestedAddition_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "RTWCG_FMU2_target.h"
#endif

#include "NestedAddition_types.h"

#include "multiword_types.h"

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

typedef struct {
  real_T FMU_FmuPrevTime;
  void* FMU_FmuStruct;
  void *FMU_PWORK[3];
  int32_T FMU_FmuIsInitialized;
} DW_NestedAddition_T;

typedef struct {
  real_T X;
  real_T Y;
} ExtU_NestedAddition_T;

typedef struct {
  real_T ERGEBNIS;
} ExtY_NestedAddition_T;

struct tag_RTM_NestedAddition_T {
  const char_T *errorStatus;
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
  } Timing;
};

extern DW_NestedAddition_T NestedAddition_DW;
extern ExtU_NestedAddition_T NestedAddition_U;
extern ExtY_NestedAddition_T NestedAddition_Y;
extern void NestedAddition_initialize(void);
extern void NestedAddition_step(void);
extern void NestedAddition_terminate(void);
extern RT_MODEL_NestedAddition_T *const NestedAddition_M;
extern void fmu_LogOutput();

#endif
