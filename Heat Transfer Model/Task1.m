% title - task1.m
% author - Abdallah Muhammed
% date - 01/04/2023
% This script is designed to plot the two temperatures (theta1 & theta2)
% and the step input heat flow (q1) for a themal system.

clear
clc
close all

%Define model constants
C_1 = 50;   %First thermal capacitance in J/K
C_2 = 60;   %Second thermal capacitance in J/K
R_1 = 10;   %First thermal resistance in K/W
R_2 = 10;   %Second thermal resistance in K/W
R_3 = 10;   %Third thermal resistance in K/W
theta_a = 293.15; %Enviroment temperature in K (20 degrees Celsius)

%Calculate sum in paralell of thermal resistances
R_12 = ((R_1*R_2)/(R_1 + R_2)); %Sum of the first and second thermal
%resistances in paralell
R_23 = ((R_2*R_3)/(R_2 + R_3)); %Sum of the second and third thermal
%resistances in paralell

%Define equations for the constants of theta1
A_1 = ( 1 / (C_1*C_2*R_2) );
B_1 = 1;
D_1 = ( (1 / (C_1*R_12)) + (1 / (C_2*R_23)));
E_1 = ( ( ((R_2)^2) - (R_12*R_23) ) / ( C_1*C_2*R_12*R_23*((R_2)^2) ) );


%Define equations for the constants of theta2
A_2 = (1/C_1);
B_2 = ( (1 / ( ((C_1)^2) * R_12 )) + (1 / (C_1*C_2*R_23)) );
D_2 = (1 / ( ((C_1)^2)*C_2*R_12*R_23 ));
E_2 = 1;
F_2 = ( (2 / (C_1*R_12)) + (1 / (C_2*R_23)));
G_2 = ( (1 / ( ((C_1)^2) * ((R_12)^2) )) + (1 / (C_1*C_2*R_12*R_23)) + ( (((R_2)^2) - (R_12*R_23)) / (C_1*C_2*R_12*R_23*((R_2)^2)) ) );
H_2 = ( (((R_2)^2) - (R_12*R_23)) / ( ((C_1)^2)*C_2*((R_12)^2)*R_23*((R_2)^2)) );

%Run the simulink model
sim("task1sim.slx")

%Plot the graphs
tiledlayout(3,1)

%Theta 1
theta_1 = nexttile;
plot(theta_1, ans.theta_1.Time/60, ans.theta_1.data)
title(theta_1, "\theta_{1}")
xlabel(theta_1, "Time(minutes)")
ylabel(theta_1, "Temperature(\circC)")
grid on
%Use stepinfo to find settling time
theta_1_st = stepinfo(ans.theta_1.data, ans.theta_1.Time/60, ans.theta_1.data(end), (theta_a - 273.15)).SettlingTime;
%Calculate steady state value
theta_1_ss = ans.theta_1.data(end);

%Theta 2
theta_2 = nexttile;
plot(theta_2, ans.theta_2.Time/60, ans.theta_2.data)
title(theta_2, "\theta_{2}")
xlabel(theta_2, "Time(minutes)")
ylabel(theta_2, "Temperature(\circC)")
grid on
%Use stepinfo to find settling time
theta_2_st = stepinfo(ans.theta_2.data, ans.theta_2.Time/60, ans.theta_2.data(end), (theta_a - 273.15)).SettlingTime;
%Calculate steady state value
theta_2_ss = ans.theta_2.data(end);


%Input heat flow q
q = nexttile;
plot(q, ans.input_heat_flow.Time/60, ans.input_heat_flow.data)
title(q, " Input Heat Flow q_{1}(t)")
xlabel(q, "Time(minutes)")
ylabel(q, "q_{1}(J)")
grid on

