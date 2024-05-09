system = MoorDynM_Create('../MoorDyn/oc3.txt');

%% 3 coupled points x 3 components per point = 9 DoF
x = zeros(9,1);
xd = zeros(9,1);
%% Get the initial positions from the system itself
for i=1:3
    %% 4 = first fairlead id
    point = MoorDynM_GetPoint(system, i + 3);
    x(1 + 3 * (i - 1):3 * i) = MoorDynM_GetPointPos(point);
end

%% Setup the initial condition
MoorDynM_Init(system, x, xd);

%% Make the points move at 0.5 m/s to the positive x direction
for i=1:3
    xd(1 + 3 * (i - 1)) = 0.5;
end
t = 0.0;
dt = 0.5;
[t, f] = MoorDynM_Step(system, x, xd, t, dt);

%% Print the position and tension of the line nodes
n_lines = MoorDynM_GetNumberLines(system);
for line_id=1:n_lines
    line_id
    line = MoorDynM_GetLine(system, line_id);
    n_nodes = MoorDynM_GetLineNumberNodes(line);
    for node_id=1:n_nodes
        node_id
        pos = MoorDynM_GetLineNodePos(line, node_id - 1);
        pos
        ten = MoorDynM_GetLineNodeTen(line, node_id - 1);
        ten
    end
end

%% Alright, time to finish!
MoorDynM_Close(system);