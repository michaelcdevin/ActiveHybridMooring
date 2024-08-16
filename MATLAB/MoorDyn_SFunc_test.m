% make sure the directory where the MoorDyn_SFunc.mex* file is located
% is in the MATLAB path

% these variables are defined in the MoorDyn_SFunc block:
MD_InputFileName = '../MoorDyn/ahm_3dof.txt';
dt               = 0.01; % seconds
TMax             = 60; % seconds

sim('../Simulink/AHM_loop.slx');
