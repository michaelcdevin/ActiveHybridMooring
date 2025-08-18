% make sure the directory where the MoorDyn_SFunc.mex* file is located
% is on the MATLAB path
clear;
close all;
clc;

% these variables are defined in the MoorDyn_SFunc block:
MD_InputFileName = '../MoorDyn/ahm_3dof.txt';
dt               = 0.01;  % [sec]
TMax             = 6.0;   % [sec]

tic;
results = sim('MD_sfunc_test_3dof_new.slx');
fprintf(1,'\n');
toc;
