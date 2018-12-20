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

/* Standard include files */
#include <stdio.h>
#include <math.h>
/* Include the header for memcpy and memset
 * You may need to change this into <memory.h> for older compilers
 */
#include <string.h>

/* 20-sim include files */
#include "%SUBMODEL_NAME%.h"

/* Debugging */
#include "common/trace.h"

/* SetErrorMessage */
#include "lv20simmodel.h"

/* this PRIVATE function sets the input variables from the input vector */
void %SUBMODEL_NAME%::CopyInputsToVariables (XXDouble *u)
{
	/* copy the input vector to the input variables */
%INPUT_TO_VARIABLE_EQUATIONS%
}

/* this PRIVATE function uses the output variables to fill the output vector */
void %SUBMODEL_NAME%::CopyVariablesToOutputs (XXDouble *y)
{
	/* copy the output variables to the output vector */
%VARIABLE_TO_OUTPUT_EQUATIONS%
}

%SUBMODEL_NAME%::%SUBMODEL_NAME%(void)
{
	%VARPREFIX%number_constants = %NUMBER_CONSTANTS%;
	%VARPREFIX%number_parameters = %NUMBER_PARAMETERS%;
	%VARPREFIX%number_initialvalues = %NUMBER_INITIAL_VALUES%;
	%VARPREFIX%number_variables = %NUMBER_VARIABLES%;
	%VARPREFIX%number_states = %NUMBER_STATES%;
	%VARPREFIX%number_rates = %NUMBER_STATES%;
	%VARPREFIX%number_matrices = %NUMBER_MATRICES%;
	%VARPREFIX%number_unnamed = %NUMBER_UNNAMED%;

	/* the variable arrays */
	%VARPREFIX%%XX_CONSTANT_ARRAY_NAME% = new XXDouble[%NUMBER_CONSTANTS% + 1];		/* constants */
	%VARPREFIX%%XX_PARAMETER_ARRAY_NAME% = new XXDouble[%NUMBER_PARAMETERS% + 1];		/* parameters */
	%VARPREFIX%%XX_INITIAL_VALUE_ARRAY_NAME% = new XXDouble[%NUMBER_INITIAL_VALUES% + 1];		/* initial values */
	%VARPREFIX%%XX_VARIABLE_ARRAY_NAME% = new XXDouble[%NUMBER_VARIABLES% + 1];		/* variables */
	%VARPREFIX%%XX_STATE_ARRAY_NAME% = new XXDouble[%NUMBER_STATES% + 1];		/* states */
	%VARPREFIX%%XX_RATE_ARRAY_NAME% = new XXDouble[%NUMBER_STATES% + 1];		/* rates (or new states) */
	%VARPREFIX%%XX_MATRIX_ARRAY_NAME% = new XXMatrix[%NUMBER_MATRICES% + 1];		/* matrices */
	%VARPREFIX%%XX_UNNAMED_ARRAY_NAME% = new XXDouble[%NUMBER_UNNAMED% + 1];		/* unnamed */
	%VARPREFIX%workarray = new XXDouble[%WORK_ARRAY_SIZE% + 1];

	Reset(%START_TIME%);
	%VARPREFIX%finish_time = %FINISH_TIME%;
}

void %SUBMODEL_NAME%::Reset(XXDouble starttime)
{
	%VARPREFIX%start_time = starttime;
	%VARPREFIX%step_size = %TIME_STEP_SIZE%;
	%VARPREFIX%%XX_TIME% = starttime;
	%VARPREFIX%major = true;
	%VARPREFIX%stop_run = false;

	/* Clear the allocated variable memory */
	memset(%VARPREFIX%%XX_CONSTANT_ARRAY_NAME%, 0, (%NUMBER_CONSTANTS% + 1) * sizeof(XXDouble));
	memset(%VARPREFIX%%XX_PARAMETER_ARRAY_NAME%, 0, (%NUMBER_PARAMETERS% + 1) * sizeof(XXDouble));
	memset(%VARPREFIX%%XX_INITIAL_VALUE_ARRAY_NAME%, 0, (%NUMBER_INITIAL_VALUES% + 1) * sizeof(XXDouble));
	memset(%VARPREFIX%%XX_VARIABLE_ARRAY_NAME%, 0, (%NUMBER_VARIABLES% + 1) * sizeof(XXDouble));
	memset(%VARPREFIX%%XX_STATE_ARRAY_NAME%, 0, (%NUMBER_STATES% + 1) * sizeof(XXDouble));
	memset(%VARPREFIX%%XX_RATE_ARRAY_NAME%, 0, (%NUMBER_STATES% + 1) * sizeof(XXDouble));
	memset(%VARPREFIX%%XX_MATRIX_ARRAY_NAME%, 0, (%NUMBER_MATRICES% + 1) * sizeof(XXDouble));
	memset(%VARPREFIX%%XX_UNNAMED_ARRAY_NAME%, 0, (%NUMBER_UNNAMED% + 1) * sizeof(XXDouble));
	memset(%VARPREFIX%workarray, 0, (%WORK_ARRAY_SIZE% + 1) * sizeof(XXDouble));

%IF%%NUMBEROF_DELAYFUNCTION%
	/* Clear the delay storage memory */
	memset(delay_update_array, 0, %NUMBEROF_DELAYFUNCTION% * sizeof(XXDouble));
	memset(delay_last_values, 0, %NUMBEROF_DELAYFUNCTION% * sizeof(XXDouble));

%ENDIF%
%IF%%NUMBEROF_INITIALFUNCTION%
	/* Clear the initialfunction storage memory */
	memset(initial_value_array, 0, %NUMBEROF_INITIALFUNCTION% * sizeof(XXDouble));

%ENDIF%
	state = initialrun;
}

bool %SUBMODEL_NAME%::IsFinished(void)
{
	return (state == finished);
}

%SUBMODEL_NAME%::~%SUBMODEL_NAME%(void)
{
	/* free memory */
	delete[] %VARPREFIX%%XX_CONSTANT_ARRAY_NAME%;
	delete[] %VARPREFIX%%XX_PARAMETER_ARRAY_NAME%;
	delete[] %VARPREFIX%%XX_INITIAL_VALUE_ARRAY_NAME%;
	delete[] %VARPREFIX%%XX_VARIABLE_ARRAY_NAME%;
	delete[] %VARPREFIX%%XX_STATE_ARRAY_NAME%;
	delete[] %VARPREFIX%%XX_RATE_ARRAY_NAME%;
	delete[] %VARPREFIX%%XX_MATRIX_ARRAY_NAME%;
	delete[] %VARPREFIX%%XX_UNNAMED_ARRAY_NAME%;
	delete[] %VARPREFIX%workarray;
}

/* the initialization function for submodel */
void %SUBMODEL_NAME%::Initialize (XXDouble *u, XXDouble *y, XXDouble t)
{
	/* initialization phase (allocating memory) */
	%VARPREFIX%%XX_INITIALIZE% = true;
	%VARPREFIX%stop_run = false;

	/* set the constants */
%INITIALIZE_CONSTANTS%

	/* set the parameters */
%INITIALIZE_PARAMETERS%

	/* set the initial values */
%INITIALIZE_INITIAL_VALUES%

	/* set the states */
%INITIALIZE_STATES%

	/* set the matrices */
%INITIALIZE_MATRICES%

	/* (re-)initialize the integration method */
	myintegmethod.Initialize(this);
	
	/* copy the inputs */
	%VARPREFIX%%XX_TIME% = t;
	CopyInputsToVariables (u);

	/* calculate initial and static equations */
	CalculateInitial ();
	CalculateStatic ();
	CalculateInput ();
	CalculateDynamic ();
	CalculateOutput ();

	/* Set the outputs */
	CopyVariablesToOutputs (y);

	/* end of initialization phase */
	%VARPREFIX%%XX_INITIALIZE% = false;

	state = mainrun;
}

/* the function that calculates the submodel */
void %SUBMODEL_NAME%::Calculate (XXDouble *u, XXDouble *y /*, XXDouble t*/)
{
	switch (state)
	{
		case initialrun:	/* calculate the model for the first time */
			Initialize(u, y, 0.0);
			break;
		case mainrun:	/* calculate the model */
			if ( ( %VARPREFIX%%XX_TIME% <= (%VARPREFIX%finish_time)) || ( %VARPREFIX%finish_time <= 0.0 ) )
			{
				/* another precessor submodel could determine the parameters of this submodel
				   and therefore the static parameter calculations need to be performed. */
				CalculateStatic ();
				CopyInputsToVariables (u);
				CalculateInput ();
				myintegmethod.Step();
				CalculateOutput ();
				CopyVariablesToOutputs (y);
			}
			if ( ( %VARPREFIX%stop_run == true ) || (( %VARPREFIX%finish_time > 0.0 ) && ( %VARPREFIX%%XX_TIME% >= %VARPREFIX%finish_time)) )
			{
				state = finished;
			}
			break;
		case finished:
			break;
		default:
			break;
	}
}

/* the termination function for submodel */
void %SUBMODEL_NAME%::Terminate (XXDouble *u, XXDouble *y /*, XXDouble t */)
{
	/* copy the inputs */
	CopyInputsToVariables (u);

	/* calculate the final model equations */
	CalculateFinal ();

	/* set the outputs */
	CopyVariablesToOutputs (y);
}


/* This function calculates the initial equations of the model.
 * These equations are calculated before anything else
 */
void %SUBMODEL_NAME%::CalculateInitial (void)
{
%INITIAL_EQUATIONS%
}

/* This function calculates the static equations of the model.
 * These equations are only dependent from parameters and constants
 */
void %SUBMODEL_NAME%::CalculateStatic (void)
{
%STATIC_EQUATIONS%
}

/* This function calculates the input equations of the model.
 * These equations are dynamic equations that must not change
 * in calls from the integration method (like random and delay).
 */
void %SUBMODEL_NAME%::CalculateInput (void)
{
%INPUT_EQUATIONS%
}

/* This function calculates the dynamic equations of the model.
 * These equations are called from the integration method
 * to calculate the new model rates (that are then integrated).
 */
void %SUBMODEL_NAME%::CalculateDynamic (void)
{
%DYNAMIC_EQUATIONS%
}

/* This function calculates the output equations of the model.
 * These equations are not needed for calculation of the rates
 * and are kept separate to make the dynamic set of equations smaller.
 * These dynamic equations are called often more than one time for each
 * integration step that is taken. This makes model computation much faster.
 */
void %SUBMODEL_NAME%::CalculateOutput (void)
{
%OUTPUT_EQUATIONS%
%IF%%NUMBEROF_DELAYFUNCTION%
	/* delay update */
	DelayUpdate();
%ENDIF%
}

/* This function calculates the final equations of the model.
 * These equations are calculated after all the calculations
 * are performed
 */
void %SUBMODEL_NAME%::CalculateFinal (void)
{
%FINAL_EQUATIONS%
}

%IF%%NUMBEROF_DELAYFUNCTION%
void %SUBMODEL_NAME%::DelayUpdate(void)
{
	memcpy(delay_update_array, delay_last_values, %NUMBEROF_DELAYFUNCTION% * sizeof(XXDouble));
}

XXDouble %SUBMODEL_NAME%::XXDelay (XXDouble argument1, XXDouble argument2, XXInteger id)
{
	if (%VARPREFIX%major)
	{
		delay_last_values[id] = argument1;
	}

	if (%VARPREFIX%%XX_INITIALIZE%)
	{
		return argument2;
	}

	return delay_update_array[id];
}
%ENDIF%

%IF%%NUMBEROF_INITIALFUNCTION%
XXDouble %SUBMODEL_NAME%::XXInitialValue (XXDouble argument, XXInteger identifier)
{
	if (%VARPREFIX%%XX_INITIALIZE%)
	{
		initial_value_array[identifier] = argument;
	}

	return initial_value_array[identifier];
}

%ENDIF%
%IF%%NUMBEROF_STOPSTATEMENT%
XXBoolean %SUBMODEL_NAME%::XXStopSimulation (XXString message, XXInteger id)
{
	DPRINTF("StopSimulation: %s\n", message);
	SetErrorMessage("StopSimulation: %s", message);
	%VARPREFIX%stop_run = true;
	return 0;
}

%ENDIF%

bool %SUBMODEL_NAME%::SetFinishTime(XXDouble newtime)
{
	if ((newtime <= 0.0) || ( newtime > %VARPREFIX%%XX_TIME%))
	{
		%VARPREFIX%finish_time = newtime;
		return true;
	}

	return false;
}
