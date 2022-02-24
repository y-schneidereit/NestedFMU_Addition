#include "Addition_macros.h"
#include "Addition.h"
#include "Addition_private.h"

ExtU_Addition_T Addition_U;
ExtY_Addition_T Addition_Y;
static RT_MODEL_Addition_T Addition_M_;
RT_MODEL_Addition_T *const Addition_M = &Addition_M_;
void Addition_step(void)
{
  Addition_Y.Ergebnis = Addition_U.x + Addition_U.y;
  fmu_LogOutput();
}

void Addition_initialize(void)
{
  (void) memset((void *)Addition_M, 0,
                sizeof(RT_MODEL_Addition_T));
  (void)memset(&Addition_U, 0, sizeof(ExtU_Addition_T));
  Addition_Y.Ergebnis = 0.0;
}

void Addition_terminate(void)
{
}
