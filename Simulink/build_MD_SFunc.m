%% INSTRUCTIONS
% This script is used to manually build a Simulink mex file which uses the libmoordyn shared library (.dll, .so, .dylib).
%
% On Windows, be sure [MATLABroot]/extern/include is included on your PATH
% before compiling
% Run `mex -setup C++` in Matlab to configure a C++ compiler if you have not already done so.
% !!BE SURE THE C++ COMPILER MATCHES THE ONE USED TO COMPILE MOORDYN!!

% Andreas: I am using VS2022

ndof = 3;  % 3 or 6

if ndof == 3
    mexname = 'MD_SFunc_3DOF';  % base name of the resulting mex file
    srcname = 'md_sfunc_3dof.cpp';
elseif ndof == 6
    mexname = 'MD_SFunc_6DOF';
    srcname = 'md_sfunc_6dof.cpp';
end

libName = 'moordyn';
libDir = '../../MoorDyn/BLD/source';  % adjust this to where you built MoorDyn
sourceDir = '../../MoorDyn/SRC/source';  % adjust this to where you have the MoorDyn source code

%% BUILD COMMAND
fprintf( '\n----------------------------\n' );
fprintf( 'Creating %s\n\n', [mexname '.' mexext] );

mex(srcname, ...
    ['-L', libDir], ...
    ['-l', libName], ...
    ['-I', sourceDir], ...
    'COMPFLAGS="$COMPFLAGS /std:c++17"', ...
    ['-DS_FUNCTION_NAME=' mexname], ...
    '-D_HAS_CXX17=1', ...
    '-output', mexname, ...
    '-v');
