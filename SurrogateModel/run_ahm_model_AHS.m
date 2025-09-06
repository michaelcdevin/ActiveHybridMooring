% make sure the directory where the MoorDyn_SFunc.mex* file is located is on the MATLAB path
clear all;
close all;
clc;

% set Python enviornment so we can use pylibkriging
%pe = pyenv('Version', 'C:\Users\mcdevin\AppData\Local\miniforge3\envs\ahm\python.exe');
pe = pyenv('Version', 'C:\ProgramData\anaconda3\envs\AHM\python.exe');
if strcmp(pe.Version,"")
    disp "Python not installed"
    return
end

% load the predict_motions module (dependencies: pandas, numpy, pylibkriging)
ret = py.importlib.reload(py.importlib.import_module('predict_motions'));

% these variables are defined in the MoorDyn_SFunc block:
MD_InputFileName = '../MoorDyn/ahm_3dof.txt';
dt               = 0.01;  % [sec]
TMax             = 2.0;   % [sec]

% run the Simulink model
tic;
results = sim('ahm_model_no_actuators_AHS.slx');
fprintf(1,'\n');
toc;

% plot results
figure;
plot(results.trialForce.Time,results.trialForce.Data);
hold on;
grid on;
xlabel('Time [sec]');
ylabel('Trial Forces [N]');

figure;
plot(results.daqDisp.Time,results.daqDisp.Data);
hold on;
grid on;
xlabel('Time [sec]');
ylabel('Daq Displacements [m]');

figure;
plot(results.daqVel.Time,results.daqVel.Data);
hold on;
grid on;
xlabel('Time [sec]');
ylabel('Daq Velocities [m/sec]');

