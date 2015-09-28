function acc = readAcc(s, calib)
% send read command to the controller
fprintf(s, 'R');
% read accelerometer
x = fscanf(s, '%d %d %d\n');
acc = (x'-calib.offset)./calib.gain;
end