# File: ahm3dof_frc_SimAppServer.tcl (use with Simulink models
# MD_sfunc_test_3dof_opfFrc.slx and HybridControllerP2C2ahm.slx)
# Units: [N,m]
#
# Written: Andreas Schellenberg (andreas.schellenberg@gmail.com)
# Created: 09/2024
# Revision: A
#
# Purpose: this file contains the tcl input to perform
# a local hybrid simulation of an active hybrid mooring
# system. The numerical portion of the system consists
# of a Simulink model running MoorDyn and connecting
# through a generic client element to OpenFresco. The
# laboratory is simulated using a second Simulink model
# running a predictor-corrector, ATS delay compensator,
# and an OpenFAST model simulating the FOWT.


# ------------------------------
# Start of model generation
# ------------------------------
logFile "ahm3dof_frc_SimAppServer.log"
defaultUnits -force N -length m -time sec -temp C

# create ModelBuilder (with three-dimensions and 3 DOF/node)
model BasicBuilder -ndm 3 -ndf 3

# Define geometry for model
# -------------------------
# node $tag $xCrd $yCrd $zCrd $mass
node 1   -489.000     0.000   -474.516
node 2  -1044.000     0.000  -1000.000
node 3    244.400   423.392   -474.516
node 4    522.000   904.130  -1000.000
node 5    244.400  -423.392   -474.516
node 6    522.000  -904.130  -1000.000

# set the boundary conditions
# fix $tag $DX $DY $DZ
fix   2   1  1  1
fix   4   1  1  1
fix   6   1  1  1

# Define control points
# ---------------------
# expControlPoint $tag <-node $nodeTag> $dof $rspType <-fact $f> <-lim $l $u> <-relTrial> <-relCtrl> <-relDaq> ...
expControlPoint 1  1 force -relTrial 2 force -relTrial 3 force -relTrial 4 force -relTrial 5 force -relTrial 6 force -relTrial 7 force -relTrial 8 force -relTrial 9 force -relTrial 1 time
expControlPoint 2  1 force 2 force 3 force 4 force 5 force 6 force 7 force 8 force 9 force 1 disp  2 disp 3 disp 4 disp 5 disp 6 disp 7 disp 8 disp 9 disp 1 vel 2 vel 3 vel 4 vel 5 vel 6 vel 7 vel 8 vel 9 vel 1 time

# Define experimental control
# ---------------------------
# expControl SimSimulink $tag ipAddr $ipPort -trialCP $cpTags -outCP $cpTags
expControl SimSimulink 1 "127.0.0.1" 22222 -trialCP 1 -outCP 2

# Define experimental setup
# -------------------------
# expSetup NoTransformation $tag <–control $ctrlTag> –dof $DOFs -sizeTrialOut $t $o <–trialDispFact $f> ...
expSetup NoTransformation 1 -control 1 -dof 1 2 3 4 5 6 7 8 9 -sizeTrialOut 9 9

# Define experimental site
# ------------------------
# expSite LocalSite $tag $setupTag
expSite LocalSite 1 1
# setSizeExpSite $siteTag -ctrlSizes (5 $size) -daqSizes (5 &size)
setSizeExpSite 1 -ctrlSizes 0 0 0 9 1 -daqSizes 9 9 9 9 1

# ------------------------------
# End of model generation
# ------------------------------


# ------------------------------
# Start of recorder generation
# ------------------------------
expRecorder Site -file "Output/Site_trialFrc.out" -time -site 1 trialForce
expRecorder Site -file "Output/Site_outDsp.out" -time -site 1 outDisp
expRecorder Site -file "Output/Site_outVel.out" -time -site 1 outVel
expRecorder Site -file "Output/Site_outFrc.out" -time -site 1 outForce

expRecorder Setup -file "Output/Setup_trialFrc.out" -time -setup 1 trialForce
expRecorder Setup -file "Output/Setup_outDsp.out" -time -setup 1 outDisp
expRecorder Setup -file "Output/Setup_outVel.out" -time -setup 1 outVel
expRecorder Setup -file "Output/Setup_outFrc.out" -time -setup 1 outForce

expRecorder Control -file "Output/Control_ctrlSig.out" -time -control 1 ctrlSignal
expRecorder Control -file "Output/Control_daqSig.out" -time -control 1 daqSignal
# --------------------------------
# End of recorder generation
# --------------------------------


# ------------------------------
# Start the server process
# ------------------------------
# startSimAppSiteServer $siteTag $port <-ssl> <-udp>
startSimAppSiteServer 1 8090;

wipeExp
exit
# --------------------------------
# End of analysis
# --------------------------------
