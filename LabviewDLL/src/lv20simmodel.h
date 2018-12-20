/**********************************************************
 * This file is generated by the 20-sim C++ Code Generator
 *
 *  file:  %FILE_NAME%
 *  subm:  %SUBMODEL_NAME%
 *  model: %MODEL_NAME%
 *  expmt: %EXPERIMENT_NAME%
 *  date:  %GENERATION_DATE%
 *  time:  %GENERATION_TIME%
 *  user:  %USER_NAME%
 *  from:  %COMPANY_NAME%
 *  build: %GENERATION_BUILD%
 **********************************************************/

#pragma once

#define DLLEXPORT __declspec( dllexport )

extern "C"
{
/**
 * Retrieve Settings from the generated 20-sim model
 * [0] = starttime  model start time
 * [1] = finishtime model finish time as entered in the simulator
 * [2] = stepsize   the time betweeen two model calculation steps
 * [3] = inputs     the amount of input signals for this submodel
 * [4] = outputs    the amount of output signals for this submodel
 */
DLLEXPORT int GetSettings(double* starttime, double* finishtime, double* stepsize, int* inputs, int* outputs);

/**
 * Initialize the 20-sim model and its inputs and outputs
 */
DLLEXPORT int Initialize(void);

/**
 * Perform the final model equations (after a normal run)
 * These are the "finalequations" in the 20-sim model
 */
DLLEXPORT int Terminate(double* inarr, int inputs, double* outarr, int outputs, double* time);

/**
 * Calculcate 1 model step. This function performs the actual model calculation.
 * Make sure that you initialize in Labview an empty outarr of size "inputs" and pass an correctly sized input array
 * "inarr" of size "inputs". See the "GetSettings" functions on how to retrieve this information.
 * The "time" parameter is increased with "stepsize" for every call.
 */
DLLEXPORT int CalculateStep(double* inarr, int inputs, double* outarr, int outputs, double* time);

/* To test the generated DLL inside 20-sim */
DLLEXPORT int TestLabviewDLL(double *inarr, int inputs, double *outarr, int outputs, int major);

/**
 * LastErrorMessage()
 *
 * This function is called by 20-sim on a return(FUNCTION_FAILED) of your DLL function.
 * It can be used to send an error message to the simulator warning pane.
 */
DLLEXPORT char* LastErrorMessage();
} /* extern "C" */

#ifdef __GNUC__ //mingw_gcc or cygwin gcc
#define _snprintf_s(A, B, C, ...) snprintf(A, C, __VA_ARGS__)
#define vsnprintf_s(A, B, C, ...) snprintf(A, C, __VA_ARGS__)
#endif

/**
 * Meaning of the dll return values
 */
#define INITIALIZE_OK         0
#define INITIALIZE_FAILED     1
#define FUNCTION_OK           0
#define FUNCTION_FAILED       1
#define MODEL_FINISHED        2
#define TERMINATE_OK          0
#define TERMINATE_FAILED      1
#define INPUT_COUNT_MISMATCH  3
#define OUTPUT_COUNT_MISMATCH 4

void SetErrorMessage(const char *format, ...);