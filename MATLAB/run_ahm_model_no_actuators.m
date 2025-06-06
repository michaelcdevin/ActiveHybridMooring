% make sure the directory where the MoorDyn_SFunc.mex* file is located
% is in the MATLAB path

% these variables are defined in the MoorDyn_SFunc block:
MD_InputFileName = '../MoorDyn/ahm_3dof.txt';
FAST_InputFileName = '../OpenFASTModel/Input files/VolturnUS15MW/VolturnUS-15MW_Taut_1000m_hybrid.fst';
TMax = 60;

tic
sim('../Simulink/ahm_model_no_actuators.slx');
exec_time = toc;

scaled_exec_time = exec_time^2;
disp(exec_time)
disp(scaled_exec_time)
disp(scaled_exec_time / TMax)