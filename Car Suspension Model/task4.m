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

%Load the road profile data into the model
file = load("roadProfile.mat");
time = file.sampledTime;
left = file.roadprofileL;
right = file.roadprofileR;

leftWheel = [time left];
rightWheel = [time, right];

%Run the simulink model
sim("task4sim.slx")

%Extract data from simulink model
t = ans.left_data.Time;
amplitudeLeft = ans.left_data.Data(:, 1);
amplitudeRoadL = ans.left_data.Data(:, 2);
amplitudeRight = ans.right_data.Data(:, 1);
amplitudeRoadR = ans.right_data.Data(:, 2);


%Calculate RMSE and MAE 
sumRMSEL = 0;
sumRMSER = 0;
sumMAE = 0;
for i = 1:length(t)
    sumRMSEL = ((amplitudeLeft(i) - amplitudeRoadL(i))^2) + sumRMSEL;
    sumRMSER = ((amplitudeRight(i) - amplitudeRoadR(i))^2) + sumRMSER;
    sumMAE = abs(amplitudeLeft(i) - amplitudeRight(i))+ sumMAE;
end

RMSEL = sqrt(sumRMSEL/i);
RMSER = sqrt(sumRMSER/i);
MAE = sumMAE/i;
fprintf('Left wheel RMSE %f\n', RMSEL);
fprintf('Right wheel RMSE %f\n', RMSER);
fprintf('MAE %f\n', MAE);