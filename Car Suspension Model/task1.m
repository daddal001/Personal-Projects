% title - task1.m
% author - Abdallah Muhammed
% date - 1/11/2022
% This script is designed to prompt the user for an input, pass the input 
% to the simulink model and then plot and analyse the data returned from 
% the model

clear
clc
close all

%Define model constants

M_s = 100;   %Mass of Driver's seat and fixings in kg
M_c = 250;   %Mass of chassis in kg
M_t = 50;    %Mass of Wheels, axles etc. in kg

K_t = 120000;   %Tyre stiffness in N/m 
K_s = 2200;     %Seat spring stiffness in N/m
Ks_c = 13000;   %Suspension spring stiffness during sports mode
Kc_c = 8000;    %Suspension spring stiffness during cruise mode

B_s = 700;      %Seat damping coefficient in Ns/m
B_sb = 300;     %Seat back friction in Ns/m
Bs_c = 1500;    %Chassis damping coefficient during sports mode
Bc_c = 900;     %Chassis damping coefficient during cruise mode

r_t = 0.1;      %step amplitude in metres

%Prompt user for input
choice = input("Would you like to model sports(s) or cruise(c) mode? ", "s");
if choice == 's'
    K_c = Ks_c;
    B_c = Bs_c;
elseif choice == 'c'
    K_c = Kc_c;
    B_c = Bc_c;
else
    disp("Invalid input");
end

%Run the simulink model
sim("task1sim.slx")

%Extract data from simulink model
t = ans.seat_data.Time;
amplitude = ans.seat_data.Data;

%Plot and analyse data the data
plot(t, amplitude);
xlabel('t (seconds)')
ylabel('Amplitude (m)')
title("Driver's seat step response" )
grid on
fprintf("Rise time: %.10f\n", stepinfo(amplitude, t).RiseTime);
fprintf("Overshoot: %f\n", stepinfo(amplitude, t).SettlingMax);
fprintf("Settling time 2%% : %f\n", stepinfo(amplitude, t).SettlingTime);
fprintf("Settling time 5%% : %f\n", stepinfo(amplitude, t, 'SettlingTimeThreshold', 0.05).SettlingTime);
