#include <stdio.h>

// model specific constants
# define GUID "{59727d70-9c23-5e16-8918-9a66ef1f3ee9}"

#ifndef FMI2_FUNCTION_PREFIX
#define FMI2_FUNCTION_PREFIX NestedAddition_
#endif

// no runtime resources
#define RESOURCE_LOCATION "file:///C:/Users/schyan01/git/fmu_nestedaddition/NestedAddition/resources" // absolut path to the unziped fmu
#include "fmi2Functions.h"

// callback functions
static void cb_logMessage(fmi2ComponentEnvironment componentEnvironment, fmi2String instanceName, fmi2Status status, fmi2String category, fmi2String message, ...) {
	printf("%s\n", message);
}

static void* cb_allocateMemory(size_t nobj, size_t size) {
	return calloc(nobj, size);
}

static void cb_freeMemory(void* obj) {
	free(obj);
}

#define CHECK_STATUS(S) { status = S; if (status != fmi2OK) goto TERMINATE; }

int main(int argc, char *argv[]) {

	fmi2Status status = fmi2OK;

	fmi2CallbackFunctions callbacks = {cb_logMessage, cb_allocateMemory, cb_freeMemory, NULL, NULL};
	
	fmi2Component c = NestedAddition_fmi2Instantiate("instance1", fmi2CoSimulation, GUID, RESOURCE_LOCATION, &callbacks, fmi2False, fmi2False);
	
	if (!c) return 1;
	
	fmi2Real Time = 0;
	fmi2Real stepSize = 1;
	
	// Informs the FMU to setup the experiment. Must be called after fmi2Instantiate and befor fmi2EnterInitializationMode
	CHECK_STATUS(NestedAddition_fmi2SetupExperiment(c, fmi2False, 0, Time, fmi2False, 0));
	
	// Informs the FMU to enter Initialization Mode.
	CHECK_STATUS(NestedAddition_fmi2EnterInitializationMode(c));
	
	fmi2ValueReference X_ref = 0;
	fmi2Real X = 0;

	fmi2ValueReference Y_ref = 1;
	fmi2Real Y = 0;

	fmi2ValueReference ERGEBNIS_ref = 2;
	fmi2Real ERGEBNIS;

	fmi2ValueReference fmuTime_ref = 3;
	fmi2Real fmuTime;
	
	CHECK_STATUS(NestedAddition_fmi2SetReal(c, &X_ref, 1, &X));
	CHECK_STATUS(NestedAddition_fmi2SetReal(c, &Y_ref, 1, &Y));

	CHECK_STATUS(NestedAddition_fmi2ExitInitializationMode(c));

	printf("time, X, Y, ERGEBNIS\n");
	
	for (int nSteps = 0; nSteps <= 10; nSteps++) {

		Time = nSteps * stepSize;
		
		// set an input
		CHECK_STATUS(NestedAddition_fmi2SetReal(c, &X_ref, 1, &X));
		CHECK_STATUS(NestedAddition_fmi2SetReal(c, &Y_ref, 1, &Y));

		// get an output
		CHECK_STATUS(NestedAddition_fmi2GetReal(c, &fmuTime_ref, 1, &fmuTime));
				
		// perform a simulation step
 		CHECK_STATUS(NestedAddition_fmi2DoStep(c, Time, stepSize, fmi2True));	// The computation of a time step is started.
		
		CHECK_STATUS(NestedAddition_fmi2GetReal(c, &ERGEBNIS_ref, 1, &ERGEBNIS));

		printf("%.2f, %.2f, %.0f, %.0f, %.0f\n", Time, fmuTime, X, Y, ERGEBNIS);

		X++;
		Y += 2;
	}
	
TERMINATE:

	// clean up
	if (status < fmi2Fatal) {
		NestedAddition_fmi2FreeInstance(c);
	}
	
	return status;
}
