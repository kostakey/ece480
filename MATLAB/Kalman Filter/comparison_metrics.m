clear; clc; close all;

%% Load data
T = readtable('drop_transient_data.csv'); % change this depending on what file you are running

t  = T.time(:);
ax = T.accel_x(:);
ay = T.accel_y(:);
az = T.accel_z(:);
au = T.accel_uni(:);

N  = length(t);
dt = mean(diff(t), 'omitnan');

%% Remove initial offsets
n_bias = min(200, floor(N/10));

ax = ax - mean(ax(1:n_bias), 'omitnan');
ay = ay - mean(ay(1:n_bias), 'omitnan');
az = az - mean(az(1:n_bias), 'omitnan');
au = au - mean(au(1:n_bias), 'omitnan');

%% 1D constant acceleration block
A1 = [1 dt 0.5*dt^2;
      0 1 dt;
      0 0 1];

%% Full 3D state transition
% State ordering:
% [dx vx ax dy vy ay dz vz az]'
A = blkdiag(A1, A1, A1);

%% Measurement model
% 3-axis accelerometer measures ax, ay, az
H_acc3 = [0 0 1  0 0 0  0 0 0;
          0 0 0  0 0 1  0 0 0;
          0 0 0  0 0 0  0 0 1];

% Uniaxial accelerometer direction in 3D
u = [1; 0; 0];   % assumed aligned with x-axis
H_uni = [0 0 u(1)  0 0 u(2)  0 0 u(3)];

% Use acceleration measurements only
H = [H_acc3;
     H_uni];

%% Noise covariances
sigma_ax = 0.08;
sigma_ay = 0.08;
sigma_az = 0.08;
sigma_au = 0.05;

R = diag([sigma_ax^2, sigma_ay^2, sigma_az^2, sigma_au^2]);

q_d = 1e-10;
q_v = 1e-6;
q_a = 1e-2;
Q = diag([q_d q_v q_a q_d q_v q_a q_d q_v q_a]);

%% Initialize
xhat = zeros(9,N);
P = diag([1e-6 1e-3 1   1e-6 1e-3 1   1e-6 1e-3 1]);

xhat(:,1) = zeros(9,1);
xhat(3,1) = ax(1);
xhat(6,1) = ay(1);
xhat(9,1) = az(1);

%% Kalman filter
for k = 2:N
    % Predict
    x_pred = A * xhat(:,k-1);
    P_pred = A * P * A' + Q;

    % Measurements
    z_full = [ax(k); ay(k); az(k); au(k)];

    % Allow missing values
    valid = ~isnan(z_full);
    zk = z_full(valid);
    Hk = H(valid,:);
    Rk = R(valid,valid);

    if isempty(zk)
        xhat(:,k) = x_pred;
        P = P_pred;
        continue;
    end

    % Update
    S = Hk * P_pred * Hk' + Rk;
    K = P_pred * Hk' / S;

    xhat(:,k) = x_pred + K * (zk - Hk*x_pred);
    P = (eye(9) - K*Hk) * P_pred;
end

%% Extract filtered accelerations
ax_est = xhat(3,:)';
ay_est = xhat(6,:)';
az_est = xhat(9,:)';

%% What the Kalman filter helped do: difference / residual
rx = ax - ax_est;
ry = ay - ay_est;
rz = az - az_est;

%% Magnitude of acceleration
a_mag_raw  = sqrt(ax.^2     + ay.^2     + az.^2);
a_mag_filt = sqrt(ax_est.^2 + ay_est.^2 + az_est.^2);

%% Peak acceleration
[peak_ax_raw,  idx_ax_raw]  = max(abs(ax), [], 'omitnan');
[peak_ay_raw,  idx_ay_raw]  = max(abs(ay), [], 'omitnan');
[peak_az_raw,  idx_az_raw]  = max(abs(az), [], 'omitnan');

[peak_ax_filt, idx_ax_filt] = max(abs(ax_est), [], 'omitnan');
[peak_ay_filt, idx_ay_filt] = max(abs(ay_est), [], 'omitnan');
[peak_az_filt, idx_az_filt] = max(abs(az_est), [], 'omitnan');

[peak_mag_raw,  idx_mag_raw]  = max(a_mag_raw,  [], 'omitnan');
[peak_mag_filt, idx_mag_filt] = max(a_mag_filt, [], 'omitnan');

%% Quantify what the Kalman filter changed
rmse_x = sqrt(mean(rx.^2, 'omitnan'));
rmse_y = sqrt(mean(ry.^2, 'omitnan'));
rmse_z = sqrt(mean(rz.^2, 'omitnan'));

std_raw_x  = std(ax, 'omitnan');
std_raw_y  = std(ay, 'omitnan');
std_raw_z  = std(az, 'omitnan');

std_filt_x = std(ax_est, 'omitnan');
std_filt_y = std(ay_est, 'omitnan');
std_filt_z = std(az_est, 'omitnan');

pct_red_x = 100 * (1 - std_filt_x/std_raw_x);
pct_red_y = 100 * (1 - std_filt_y/std_raw_y);
pct_red_z = 100 * (1 - std_filt_z/std_raw_z);

valid_x = ~isnan(ax) & ~isnan(ax_est);
valid_y = ~isnan(ay) & ~isnan(ay_est);
valid_z = ~isnan(az) & ~isnan(az_est);

Cx = corrcoef(ax(valid_x), ax_est(valid_x)); corr_x = Cx(1,2);
Cy = corrcoef(ay(valid_y), ay_est(valid_y)); corr_y = Cy(1,2);
Cz = corrcoef(az(valid_z), az_est(valid_z)); corr_z = Cz(1,2);

%% Print results
fprintf('\n============================================\n');
fprintf('KALMAN FILTER RESULTS\n');
fprintf('============================================\n');

fprintf('\nWhat the Kalman filter changed\n');
fprintf('--------------------------------------------\n');
fprintf('X-axis RMSE difference (raw - filtered): %.4f g\n', rmse_x);
fprintf('Y-axis RMSE difference (raw - filtered): %.4f g\n', rmse_y);
fprintf('Z-axis RMSE difference (raw - filtered): %.4f g\n', rmse_z);

fprintf('\nStandard deviation reduction\n');
fprintf('X: %.4f g  -> %.4f g  (%.2f%% reduction)\n', std_raw_x, std_filt_x, pct_red_x);
fprintf('Y: %.4f g  -> %.4f g  (%.2f%% reduction)\n', std_raw_y, std_filt_y, pct_red_y);
fprintf('Z: %.4f g  -> %.4f g  (%.2f%% reduction)\n', std_raw_z, std_filt_z, pct_red_z);

fprintf('\nSignal preservation (correlation)\n');
fprintf('X corr(raw, filtered): %.4f\n', corr_x);
fprintf('Y corr(raw, filtered): %.4f\n', corr_y);
fprintf('Z corr(raw, filtered): %.4f\n', corr_z);

fprintf('\nPeak acceleration by axis\n');
fprintf('X raw peak      : %.4f g at t = %.6f s\n', peak_ax_raw,  t(idx_ax_raw));
fprintf('X filtered peak : %.4f g at t = %.6f s\n', peak_ax_filt, t(idx_ax_filt));

fprintf('Y raw peak      : %.4f g at t = %.6f s\n', peak_ay_raw,  t(idx_ay_raw));
fprintf('Y filtered peak : %.4f g at t = %.6f s\n', peak_ay_filt, t(idx_ay_filt));

fprintf('Z raw peak      : %.4f g at t = %.6f s\n', peak_az_raw,  t(idx_az_raw));
fprintf('Z filtered peak : %.4f g at t = %.6f s\n', peak_az_filt, t(idx_az_filt));

fprintf('\nOverall acceleration magnitude peak\n');
fprintf('Raw magnitude peak      : %.4f g at t = %.6f s\n', peak_mag_raw,  t(idx_mag_raw));
fprintf('Filtered magnitude peak : %.4f g at t = %.6f s\n', peak_mag_filt, t(idx_mag_filt));
fprintf('============================================\n');

%% Plot 1: raw vs filtered acceleration
figure('Color','w','Position',[100 100 1000 750]);

subplot(3,1,1);
plot(t, ax, '.', 'MarkerSize', 4); hold on;
plot(t, ax_est, 'LineWidth', 1.5);
grid on;
xlabel('Time (s)');
ylabel('a_x (g)');
legend('Measured','Kalman filtered','Location','best');
title('X Acceleration');

subplot(3,1,2);
plot(t, ay, '.', 'MarkerSize', 4); hold on;
plot(t, ay_est, 'LineWidth', 1.5);
grid on;
xlabel('Time (s)');
ylabel('a_y (g)');
legend('Measured','Kalman filtered','Location','best');
title('Y Acceleration');

subplot(3,1,3);
plot(t, az, '.', 'MarkerSize', 4); hold on;
plot(t, az_est, 'LineWidth', 1.5);
grid on;
xlabel('Time (s)');
ylabel('a_z (g)');
legend('Measured','Kalman filtered','Location','best');
title('Z Acceleration');

sgtitle('Raw vs Kalman Filtered Acceleration');

%% Plot 2: what the Kalman filter removed
figure('Color','w','Position',[120 120 1000 750]);

subplot(3,1,1);
plot(t, rx, 'LineWidth', 1.2);
grid on;
xlabel('Time (s)');
ylabel('\Delta a_x (g)');
title('Difference: Measured - Filtered (X)');

subplot(3,1,2);
plot(t, ry, 'LineWidth', 1.2);
grid on;
xlabel('Time (s)');
ylabel('\Delta a_y (g)');
title('Difference: Measured - Filtered (Y)');

subplot(3,1,3);
plot(t, rz, 'LineWidth', 1.2);
grid on;
xlabel('Time (s)');
ylabel('\Delta a_z (g)');
title('Difference: Measured - Filtered (Z)');

sgtitle('What the Kalman Filter Removed');

%% Plot 3: overall acceleration magnitude and peak
figure('Color','w','Position',[140 140 1000 450]);
plot(t, a_mag_raw, '.', 'MarkerSize', 4); hold on;
plot(t, a_mag_filt, 'LineWidth', 1.8);

plot(t(idx_mag_raw),  peak_mag_raw,  'ro', 'MarkerSize', 8, 'LineWidth', 1.5);
plot(t(idx_mag_filt), peak_mag_filt, 'ks', 'MarkerSize', 8, 'LineWidth', 1.5);

grid on;
xlabel('Time (s)');
ylabel('|a| (g)');
legend('Raw magnitude','Filtered magnitude','Raw peak','Filtered peak','Location','best');
title('Overall Acceleration Magnitude and Peak');

%% Optional: save figures
% saveas(figure(1), 'accel_raw_vs_kalman.png');
% saveas(figure(2), 'accel_difference_removed_by_kalman.png');
% saveas(figure(3), 'accel_peak_magnitude.png');