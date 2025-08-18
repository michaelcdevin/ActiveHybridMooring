%% make sure the directory where the MoorDyn_SFunc.mex* file is located is on the MATLAB path
% start OpenFresco and source "ahm3dof_frc_SimAppServer.tcl" before running this file
clear all;
close all;
clc;

% these variables are defined in the MoorDyn_SFunc block:
MD_InputFileName = 'MoorDyn models/ahm_3dof.txt';
dt               = 0.01;  % [sec]
TMax             = 30.0;  % [sec]
pdfFileName      = fullfile(pwd,'Output/ahm_model_with_openfresco_results_P2C2.pdf');

% run the Simulink model
tic;
results = sim('MD_sfunc_test_3dof_opfFrc_2025a.slx');  % use with "OpenFresco middleware/ahm3dof_frc_SimAppServer.tcl"
fprintf(1,'\n');
toc;

% plot results
hFig = figure;
plot(results.trialForce.Time,results.trialForce.Data,'LineWidth',1.0);
hold on;
grid on;
set(gca,'XLim',[0,TMax],'YLim',[-4E7,4E7]);
xlabel('Time [sec]');
ylabel('Trial Forces [N]');
legend('Line 1: DOF 1','Line 1: DOF 2','Line 1: DOF 3','Line 2: DOF 1','Line 2: DOF 2','Line 2: DOF 3','Line 3: DOF 1','Line 3: DOF 2','Line 3: DOF 3');
exportgraphics(hFig,pdfFileName,'ContentType','vector');

hFig = figure;
plot(results.daqDisp.Time,results.daqDisp.Data,'LineWidth',1.0);
hold on;
grid on;
set(gca,'XLim',[0,TMax],'YLim',[-500,500]);
xlabel('Time [sec]');
ylabel('Daq Displacements [m]');
legend('Line 1: DOF 1','Line 1: DOF 2','Line 1: DOF 3','Line 2: DOF 1','Line 2: DOF 2','Line 2: DOF 3','Line 3: DOF 1','Line 3: DOF 2','Line 3: DOF 3');
exportgraphics(hFig,pdfFileName,'ContentType','vector','Append',true);

hFig = figure;
plot(results.daqVel.Time,results.daqVel.Data,'LineWidth',1.0);
hold on;
grid on;
set(gca,'XLim',[0,TMax],'YLim',[-5,5]);
xlabel('Time [sec]');
ylabel('Daq Velocities [m/sec]');
legend('Line 1: DOF 1','Line 1: DOF 2','Line 1: DOF 3','Line 2: DOF 1','Line 2: DOF 2','Line 2: DOF 3','Line 3: DOF 1','Line 3: DOF 2','Line 3: DOF 3');
exportgraphics(hFig,pdfFileName,'ContentType','vector','Append',true);

hFig = figure;
plot(results.daqForce.Time,results.daqForce.Data,'LineWidth',1.0);
hold on;
grid on;
set(gca,'XLim',[0,TMax],'YLim',[-2E5,2E5]);
xlabel('Time [sec]');
ylabel('Daq Forces [N]');
legend('Line 1: DOF 1','Line 1: DOF 2','Line 1: DOF 3','Line 2: DOF 1','Line 2: DOF 2','Line 2: DOF 3','Line 3: DOF 1','Line 3: DOF 2','Line 3: DOF 3');
exportgraphics(hFig,pdfFileName,'ContentType','vector','Append',true);
