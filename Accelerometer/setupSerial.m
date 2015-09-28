function s = setupSerial(comPort)
    % function to connect Arduino Nano to Matlab

    % initialize the com port
    s = serial(comPort);
    set(s, 'DataBits', 8);
    set(s, 'StopBits', 1);
    set(s, 'BaudRate', 9600);
    set(s, 'Parity', 'none');

    % open serial port
    fopen(s);
end