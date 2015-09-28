function calib = calibrateAcc(s)

% initialization
calib.offset = zeros(1, 3);
calib.gain = ones(1, 3);

% lay acceleremoter on flat surfface
% z=1, x=y=0
mbox = msgbox('Lay accelerometer on a flat surface');
uiwait(mbox);
acc_z = readAcc(s, calib);
display(acc_z);

% X points up
% x=1, z=y=0
mbox = msgbox('Stand accelerometer that way so that X arrow points up');
uiwait(mbox);
acc_x = readAcc(s, calib);
display(acc_x);

% Y points up
% y=1, x=y=0
mbox = msgbox('Stand accelerometer that way so that Y arrow points up');
uiwait(mbox);
acc_y = readAcc(s, calib);
display(acc_y);

% compute offset
calib.offset(1) = (acc_z(1) + acc_y(1))/2;
calib.offset(2) = (acc_z(2) + acc_x(2))/2;
calib.offset(3) = (acc_x(3) + acc_y(3))/2;

% compute gain
calib.gain(1) = acc_x(1) - calib.offset(1);
calib.gain(2) = acc_y(2) - calib.offset(2);
calib.gain(3) = acc_z(3) - calib.offset(3);

mbox = msgbox('Calibration complete.');
uiwait(mbox);
end


