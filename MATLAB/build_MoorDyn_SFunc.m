%% INSTRUCTIONS
% This script is used to manually build a Simulink mex file which uses the libmoordyn shared library (.dll, .so, .dylib).
%
% On Windows, be sure [MATLABroot]/extern/include is included on your PATH
% before compiling
% Run `mex -setup C++` in Matlab to configure a C++ compiler if you have not already done so.
% !!BE SURE THE C++ COMPILER MATCHES THE ONE USED TO COMPILE MOORDYN!!

ndof = 6; %6

if ndof == 3
    mexname = 'MoorDyn_SFunc_3DOF'; % base name of the resulting mex file
    srcname = 'moordyn_sfunc_3dof.cpp';
elseif ndof == 6
    mexname = 'MoorDyn_SFunc_6DOF';
    srcname = 'moordyn_sfunc_6dof.cpp';
end

libName = 'moordyn.dll';
libDir = '../../MoorDyn/build/source';
sourceDir = '../../MoorDyn/source';

%% BUILD COMMAND
fprintf( '\n----------------------------\n' );
fprintf( 'Creating %s\n\n', [mexname '.' mexext] );

mex(srcname, ...
    ['-L', libDir], ...
    ['-l', libName], ...
    ['-I', sourceDir], ...
    ['CXXFLAGS=$CXXFLAGS -DS_FUNCTION_NAME=' mexname], ...
    '-output', mexname, ...
    '-v');
