% make sure the directory where the MoorDyn_SFunc.mex* file is located
% is in the MATLAB path
clear;
close all;
clc;

% these variables are defined in the MoorDyn_SFunc block:
MD_InputFileName   = '../MoorDyn/ahm_3dof.txt';
FAST_InputFileName = '../OpenFASTModel/Input files/VolturnUS15MW/VolturnUS-15MW_Taut_1000m_hybrid_100Hz.fst';
DT                 = 0.01;  % [sec]
TMax               = 60;  % [sec]
pdfFileName        = fullfile(pwd,'ahm_model_no_actuators_results_100Hz.pdf');

% run the Simulink model
tic;
results = sim('ahm_model_no_actuators.slx');
fprintf(1,'\n');
exec_time = toc;

scaled_exec_time = exec_time^2;
disp(exec_time)
disp(scaled_exec_time)
disp(scaled_exec_time / TMax)

% plot results
hFig = figure;
plot(results.trialForce.Time,results.trialForce.Data);
hold on;
grid on;
set(gca,'XLim',[0,TMax],'YLim',[-4E7,4E7]);
xlabel('Time [sec]');
ylabel('Trial Forces [N]');
exportgraphics(hFig,pdfFileName,'ContentType','vector');

hFig = figure;
plot(results.daqDisp.Time,results.daqDisp.Data);
hold on;
grid on;
set(gca,'XLim',[0,TMax],'YLim',[-500,500]);
xlabel('Time [sec]');
ylabel('Daq Displacements [m]');
exportgraphics(hFig,pdfFileName,'ContentType','vector','Append',true);

hFig = figure;
plot(results.daqVel.Time,results.daqVel.Data);
hold on;
grid on;
set(gca,'XLim',[0,TMax],'YLim',[-2.5,2.5]);
xlabel('Time [sec]');
ylabel('Daq Velocities [m/sec]');
exportgraphics(hFig,pdfFileName,'ContentType','vector','Append',true);

hFig = figure;
plot(results.daqForce.Time,results.daqForce.Data);
hold on;
grid on;
set(gca,'XLim',[0,TMax],'YLim',[-2.5E4,2.5E4]);
xlabel('Time [sec]');
ylabel('Daq Forces [N]');
exportgraphics(hFig,pdfFileName,'ContentType','vector','Append',true);
