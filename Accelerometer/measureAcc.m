%% initialization
clear all;
com = 'COM3';


%% setup serial port
s = setupSerial(com);


%% calibrate the accelerometer
calib = calibrateAcc(s);


%% read few values from serial port
acc_val = readAcc(s, calib);
display(acc_val);

%% Start vizualization
vizAcc(s, calib);


%% close serial and removed it from workspace
fclose(s);
delete(s);
clear s;


