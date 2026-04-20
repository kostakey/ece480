clear; clc; close all;

%% Load data
T = readtable('drop_transient_data.csv'); % change this depending on what file you are running

t = T.time(:);
ax = T.accel_x(:);
ay = T.accel_y(:);
az = T.accel_z(:);
au = T.accel_uni(:);
strain = T.strain(:);

%% Remove NaNs if needed
valid = ~(isnan(t) | isnan(ax) | isnan(ay) | isnan(az));
t = t(valid);
ax = ax(valid);
ay = ay(valid);
az = az(valid);

% For uniaxial and strain, fill missing values with nearest samples
au = fillmissing(au, 'nearest');
strain = fillmissing(strain, 'nearest');

%% Remove DC offsets
ax = ax - mean(ax);
ay = ay - mean(ay);
az = az - mean(az);
au = au - mean(au);
strain = strain - mean(strain);

%% Sampling info
N = length(t);
dt = mean(diff(t));
Fs = 1/dt;

fprintf('Sampling frequency: %.2f Hz\n', Fs);

%% Frequency vector
f = Fs*(0:floor(N/2))/N;

%% Helper function for FFT
calcFFT = @(x) deal( ...
    abs(fft(x)/N), ...
    abs(fft(x)/N) );

%% Compute FFTs
X_ax = fft(ax);
X_ay = fft(ay);
X_az = fft(az);
X_au = fft(au);
X_strain = fft(strain);

P2_ax = abs(X_ax/N);
P2_ay = abs(X_ay/N);
P2_az = abs(X_az/N);
P2_au = abs(X_au/N);
P2_strain = abs(X_strain/N);

P1_ax = P2_ax(1:floor(N/2)+1);
P1_ay = P2_ay(1:floor(N/2)+1);
P1_az = P2_az(1:floor(N/2)+1);
P1_au = P2_au(1:floor(N/2)+1);
P1_strain = P2_strain(1:floor(N/2)+1);

P1_ax(2:end-1) = 2*P1_ax(2:end-1);
P1_ay(2:end-1) = 2*P1_ay(2:end-1);
P1_az(2:end-1) = 2*P1_az(2:end-1);
P1_au(2:end-1) = 2*P1_au(2:end-1);
P1_strain(2:end-1) = 2*P1_strain(2:end-1);

%% Ignore DC peak when finding dominant frequency
[~, idx_ax] = max(P1_ax(2:end));
[~, idx_ay] = max(P1_ay(2:end));
[~, idx_az] = max(P1_az(2:end));
[~, idx_au] = max(P1_au(2:end));
[~, idx_strain] = max(P1_strain(2:end));

idx_ax = idx_ax + 1;
idx_ay = idx_ay + 1;
idx_az = idx_az + 1;
idx_au = idx_au + 1;
idx_strain = idx_strain + 1;

fprintf('Dominant frequency in accel_x: %.2f Hz\n', f(idx_ax));
fprintf('Dominant frequency in accel_y: %.2f Hz\n', f(idx_ay));
fprintf('Dominant frequency in accel_z: %.2f Hz\n', f(idx_az));
fprintf('Dominant frequency in accel_uni: %.2f Hz\n', f(idx_au));
fprintf('Dominant frequency in strain: %.2f Hz\n', f(idx_strain));

%% Plot FFTs
figure;
plot(f, P1_ax, 'LineWidth', 1.5);
grid on;
xlabel('Frequency (Hz)');
ylabel('|P1(f)|');
title('FFT of X Acceleration');

figure;
plot(f, P1_ay, 'LineWidth', 1.5);
grid on;
xlabel('Frequency (Hz)');
ylabel('|P1(f)|');
title('FFT of Y Acceleration');

figure;
plot(f, P1_az, 'LineWidth', 1.5);
grid on;
xlabel('Frequency (Hz)');
ylabel('|P1(f)|');
title('FFT of Z Acceleration');

figure;
plot(f, P1_strain, 'LineWidth', 1.5);
grid on;
xlabel('Frequency (Hz)');
ylabel('|P1(f)|');
title('FFT of Strain');

%% Optional: save plots
outFolder = 'fft_figures';
if ~exist(outFolder, 'dir')
    mkdir(outFolder);
end

exportgraphics(findobj('Type','figure', 'Number', 1), fullfile(outFolder,'wrench_fft_accel_x.png'), 'Resolution', 300);
exportgraphics(findobj('Type','figure', 'Number', 2), fullfile(outFolder,'wrench_fft_accel_y.png'), 'Resolution', 300);
exportgraphics(findobj('Type','figure', 'Number', 3), fullfile(outFolder,'wrench_fft_accel_z.png'), 'Resolution', 300);
exportgraphics(findobj('Type','figure', 'Number', 4), fullfile(outFolder,'wrench_fft_strain.png'), 'Resolution', 300);