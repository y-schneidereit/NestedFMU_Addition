#ifndef RTW_HEADER_Addition_h_
#define RTW_HEADER_Addition_h_
#include <string.h>
#ifndef Addition_COMMON_INCLUDES_
#define Addition_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif

#include "Addition_types.h"

#include "multiword_types.h"

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

typedef struct {
  real_T x;
  real_T y;
} ExtU_Addition_T;

typedef struct {
  real_T Ergebnis;
} ExtY_Addition_T;

struct tag_RTM_Addition_T {
  const char_T *errorStatus;
};

extern ExtU_Addition_T Addition_U;
extern ExtY_Addition_T Addition_Y;
extern void Addition_initialize(void);
extern void Addition_step(void);
extern void Addition_terminate(void);
extern RT_MODEL_Addition_T *const Addition_M;
extern void fmu_LogOutput();

#endif
