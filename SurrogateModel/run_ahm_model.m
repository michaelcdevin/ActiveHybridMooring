clear
clc

pyenv('Version', 'C:\Users\mcdevin\AppData\Local\miniforge3\envs\ahm\python.exe');
MD_InputFileName = '../MoorDyn/ahm_3dof.txt';
dt               = 0.01; % seconds
TMax             = 0.1; % seconds

sim('ahm_model_no_actuators.slx');
