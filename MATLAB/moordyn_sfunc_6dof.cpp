/*
 * sfuntmpl_basic.c: Basic 'C' template for a level 2 S-function.
 *
 * Copyright 1990-2018 The MathWorks, Inc.
 */


/*
 * You must specify the S_FUNCTION_NAME as the name of your S-function
 * (i.e. replace sfuntmpl_basic with the name of your S-function).
 */

#define S_FUNCTION_LEVEL 2

#ifndef S_FUNCTION_NAME
#define S_FUNCTION_NAME  MoorDyn_SFunc
#endif
/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"
// #include "mex.hpp"
// #include "matrix.hpp"  // for mxCreateDoubleScalar
// #include "mexAdapter.hpp"
#include "MoorDyn2.hpp"


#define PARAM_FILENAME 0
#define PARAM_DT 1
#define PARAM_TMAX 2
#define NUM_PARAM 3
#define CHANNEL_LENGTH 20
#define INTERFACE_STRING_LENGTH 1025
#define MAXIMUM_OUTPUTS 4000

// two DWork arrays:
#define WORKARY_OUTPUT 0
#define WORKARY_INPUT 1

static int ndof = 0;
static double dt = 0;
static double TMax = 0;
static int NumInputs = 36;
static int NumOutputs = 18;
static char InputFileName[INTERFACE_STRING_LENGTH];
static int err = 0;
static char err_msg[INTERFACE_STRING_LENGTH];
double t = -1;  // start at -1 for initialization

// function definitions
static int checkError(SimStruct *S);
static void mdlTerminate(SimStruct *S); // defined here so I can call it from checkError
static void getInputs(SimStruct *S, double *InputAry);
static void setOutputs(SimStruct *S, double *OutputAry);

moordyn::MoorDyn* mdSystem;

/* Error handling
 * --------------
 *
 * You should use the following technique to report errors encountered within
 * an S-function:
 *
 *       ssSetErrorStatus(S,"Error encountered due to ...");
 *       return;
 *
 * Note that the 2nd argument to ssSetErrorStatus must be persistent memory.
 * It cannot be a local variable. For example the following will cause
 * unpredictable errors:
 *
 *      mdlOutputs()
 *      {
 *         char msg[256];         {ILLEGAL: to fix use "static char msg[256];"}
 *         sprintf(msg,"Error due to %s", string);
 *         ssSetErrorStatus(S,msg);
 *         return;
 *      }
 *
 */

static int checkError(SimStruct *S) {
    if (err > MOORDYN_SUCCESS) {
        ssPrintf("\n");
        strcpy(err_msg, "MoorDyn exited with error.");
        ssSetErrorStatus(S, err_msg);
        mdlTerminate(S);  // terminate on error (in case Simulink doesn't do so itself)
        return 1;
    }
    return 0;
}

static void getInputs(SimStruct *S, double *InputAry) {
   int k;
   InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S, 0);

   for (k = 0; k < ssGetDWorkWidth(S, WORKARY_INPUT); k++) {
      InputAry[k] = (double)(*uPtrs[k]);
   }
}

static void setOutputs(SimStruct *S, double *OutputAry) {
   int k;
   double *y = ssGetOutputPortRealSignal(S, 0);

   for (k = 0; k < ssGetOutputPortWidth(S, WORKARY_OUTPUT); k++) {
      y[k] = OutputAry[k];
   }
}

/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    static char ChannelNames[CHANNEL_LENGTH *  + 1];
    if (t == -1) {
        ssSetNumSFcnParams(S, NUM_PARAM);  /* Number of expected parameters */

        // Get parameters from user-inputted block parameters (should not change during simulation)
        ssSetSFcnParamTunable(S, PARAM_FILENAME, SS_PRM_NOT_TUNABLE); 
        mxGetString(ssGetSFcnParam(S, PARAM_FILENAME), InputFileName, INTERFACE_STRING_LENGTH);

        ssSetSFcnParamTunable(S, PARAM_DT, SS_PRM_NOT_TUNABLE); 
        dt = mxGetScalar(ssGetSFcnParam(S, PARAM_DT));

        ssSetSFcnParamTunable(S, PARAM_TMAX, SS_PRM_NOT_TUNABLE); 
        TMax = mxGetScalar(ssGetSFcnParam(S, PARAM_TMAX));

        // Initialize mooring system
        mdSystem = new moordyn::MoorDyn(InputFileName);
        if (checkError(S)) return;

        // MCD: I don't think I need this, but still not entirely sure TODO
        // set DT in the Matlab workspace (necessary for Simulink block solver options)
        // pm = mxCreateDoubleScalar(dt);
        // err = mexPutVariable("base", "DT", pm);
        // mxDestroyArray(pm);
        // if (checkError(S)) return;

        ssSetNumContStates(S, 0);
        ssSetNumDiscStates(S, 0);
        if (!ssSetNumInputPorts(S, 1)) return;
        ssSetInputPortWidth(S, 0, NumInputs);
        // ssSetInputPortRequiredContiguous(S, 0, true); /*direct input signal access*/
        
        /*
        * Set direct feedthrough flag (1=yes, 0=no).
        * A port has direct feedthrough if the input is used in either
        * the mdlOutputs or mdlGetTimeOfNextVarHit functions.
        */
        ssSetInputPortDirectFeedThrough(S, 0, 0); // no direct feedthrough because we're just putting everything in one update routine (acting like a discrete system)

        if (!ssSetNumOutputPorts(S, 1)) return;
        ssSetOutputPortWidth(S, 0, NumOutputs);

        ssSetNumSampleTimes(S, 1);
        ssSetNumRWork(S, 0);
        ssSetNumIWork(S, 0);
        ssSetNumPWork(S, 0);
        ssSetNumModes(S, 0);
        ssSetNumNonsampledZCs(S, 0);

        if(!ssSetNumDWork(   S, 2)) return;

        ssSetDWorkWidth(   S, WORKARY_OUTPUT, ssGetOutputPortWidth(S, 0));
        ssSetDWorkDataType(S, WORKARY_OUTPUT, SS_DOUBLE); /* use SS_DOUBLE if needed */

        ssSetDWorkWidth(   S, WORKARY_INPUT, ssGetInputPortWidth(S, 0));
        ssSetDWorkDataType(S, WORKARY_INPUT, SS_DOUBLE);

        /* Specify the operating point save/restore compliance to be same as a 
        * built-in block */
        ssSetOperatingPointCompliance(S, USE_DEFAULT_OPERATING_POINT);

        ssSetRuntimeThreadSafetyCompliance(S, RUNTIME_THREAD_SAFETY_COMPLIANCE_TRUE);
        ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
    }
}



/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, dt);
    ssSetOffsetTime(S, 0, 0.0);

    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}



#undef MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
#if defined(MDL_INITIALIZE_CONDITIONS)
  /* Function: mdlInitializeConditions ========================================
   * Abstract:
   *    In this function, you should initialize the continuous and discrete
   *    states for your S-function block.  The initial states are placed
   *    in the state vector, ssGetContStates(S) or ssGetRealDiscStates(S).
   *    You can also perform any other initialization activities that your
   *    S-function may require. Note, this routine will be called at the
   *    start of simulation and if it is present in an enabled subsystem
   *    configured to reset states, it will be call when the enabled subsystem
   *    restarts execution to reset the states.
   */
  static void mdlInitializeConditions(SimStruct *S)
  {
  }
#endif /* MDL_INITIALIZE_CONDITIONS */



#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
  /* Function: mdlStart =======================================================
   * Abstract:
   *    This function is called once at start of model execution. If you
   *    have states that should be initialized once, this is the place
   *    to do it.
   */
  static void mdlStart(SimStruct *S)
  {
    double *InputAry = (double *)ssGetDWork(S, WORKARY_INPUT); //malloc(NumInputs*sizeof(double));   
    double *OutputAry = (double *)ssGetDWork(S, WORKARY_OUTPUT);
    if (t == -1){
        
        // get general system definitions
        ndof = mdSystem->NCoupledDOF();
        double x[ndof], xd[ndof];

        // initial positions -- this ideally would be read from the input file TODO
        x[0] =  -406.060;
        x[1] =     0.000;
        x[2] =  -446.000;
        x[3] =     0.000;
        x[4] =     0.000;
        x[5] =   180.000;
        x[6] =   147.490;
        x[7] =   387.691;
        x[8] =  -446.000;
        x[9] =     0.000;
        x[10] =    0.000;
        x[11] =   60.000;
        x[12] =  147.490;
        x[13] = -387.691;
        x[14] = -446.000;
        x[15] =    0.000;
        x[16] =    0.000;
        x[17] =  -60.000;

         // initial velocity is zero
        memset(xd, 0.0, sizeof(double));
        
        // Setup for time series simulation
        int err = mdSystem->Init(x, xd);
        if (checkError(S)) return;
        t = 0;
    }
  }
#endif /*  MDL_START */



/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    double *OutputAry = (double *)ssGetDWork(S, WORKARY_OUTPUT);
    setOutputs(S, OutputAry);
}



#define MDL_UPDATE  /* Change to #undef to remove function */
#if defined(MDL_UPDATE)
  /* Function: mdlUpdate ======================================================
   * Abstract:
   *    This function is called once for every major integration time step.
   *    Discrete states are typically updated here, but this function is useful
   *    for performing any tasks that should only take place once per
   *    integration step.
   */
  static void mdlUpdate(SimStruct *S, int_T tid)
  {
    double *InputAry  = (double *)ssGetDWork(S, WORKARY_INPUT);
    double *OutputAry = (double *)ssGetDWork(S, WORKARY_OUTPUT);

    getInputs(S, InputAry);

    double x[ndof], xd[ndof];
    std::copy(InputAry, InputAry + ndof, x);
    std::copy(InputAry + ndof + 1, InputAry + 2*ndof, xd);

    err = mdSystem->Step(x, xd, OutputAry, t, dt);
    t += dt;

    if (checkError(S)) return;
    setOutputs(S, OutputAry);
  }
#endif /* MDL_UPDATE */



#undef MDL_DERIVATIVES  /* Change to #undef to remove function */
#if defined(MDL_DERIVATIVES)
  /* Function: mdlDerivatives =================================================
   * Abstract:
   *    In this function, you compute the S-function block's derivatives.
   *    The derivatives are placed in the derivative vector, ssGetdX(S).
   */
  static void mdlDerivatives(SimStruct *S)
  {
  }
#endif /* MDL_DERIVATIVES */



/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
  if (t > -1) {
    MoorDynClose();
    t = -1;
  }
  return;
}


/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
