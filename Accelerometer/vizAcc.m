function vizAcc(s, calib)
% vizualize the accelerometer data

% create plot and show axes
h = figure(1);
ax = axes('box', 'on');
% create stop button
stopButton = uicontrol('Style', 'pushbutton', 'String', 'Stop',...
    'pos', [10 10 50 25], 'parent', h, 'Callback', @stopViz,...
    'UserData', 1);

% Initialization
n_buffer = 100;
index = 1:n_buffer;
data = zeros(3, n_buffer);
% plot initialization
plot(index(1), data(1:3, 1));
xlim([0, n_buffer]); ylim([-3.5, 3.5]);  % static limits
xlabel('Time');
ylabel('Magnitude of all axis accelerometer');
legend('X', 'Y', 'Z');
hold on;
drawnow;

% run
while (get(stopButton, 'UserData'))
    % read acc data and merge it with data matrix
    acc = readAcc(s, calib); data = [data(1:3,2:end) acc'];
    % plot
    cla; plot(index, data); drawnow;
end

function stopViz(source, callbackdata)
    set(source, 'UserData', 0);
end

end
