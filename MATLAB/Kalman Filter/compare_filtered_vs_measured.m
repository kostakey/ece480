clear; clc; close all;

%% Load data
T = readtable('wrench_transient_data.csv'); % change this depending on what file you are running

t = T.time(:);
ax = T.accel_x(:);
ay = T.accel_y(:);
az = T.accel_z(:);
au = T.accel_uni(:);
eps_amp = T.strain(:);

N = length(t);
dt = mean(diff(t), 'omitnan');

%% Remove initial offsets
n_bias = min(200, floor(N/10));

ax = ax - mean(ax(1:n_bias), 'omitnan');
ay = ay - mean(ay(1:n_bias), 'omitnan');
az = az - mean(az(1:n_bias), 'omitnan');
au = au - mean(au(1:n_bias), 'omitnan');

%% Strain conversion
Vbias = 1.65;
G_amp = 13;
GF = 2.04;
Vex = 3.3;

Vstrain = eps_amp - Vbias;
Vstrain = Vstrain - mean(Vstrain(1:n_bias), 'omitnan');

eps_meas = Vstrain / (G_amp * GF * Vex);
eps_micro = eps_meas * 1e6;

fprintf('NaNs in eps_amp   : %d\n', sum(isnan(eps_amp)));
fprintf('NaNs in Vstrain   : %d\n', sum(isnan(Vstrain)));
fprintf('NaNs in eps_meas  : %d\n', sum(isnan(eps_meas)));
fprintf('NaNs in eps_micro : %d\n', sum(isnan(eps_micro)));

%% State model
A1 = [1 dt 0.5*dt^2;
      0 1 dt;
      0 0 1];

A = blkdiag(A1, A1, A1);

%% Measurement model
H_acc3 = [0 0 1  0 0 0  0 0 0;
          0 0 0  0 0 1  0 0 0;
          0 0 0  0 0 0  0 0 1];

u = [1; 0; 0];
H_uni = [0 0 u(1)  0 0 u(2)  0 0 u(3)];

c_eps = [1200 0 0];
H_strain = [c_eps(1) 0 0  c_eps(2) 0 0  c_eps(3) 0 0];

H = [H_acc3;
     H_uni;
     H_strain];

%% Noise
sigma_ax = 0.08;
sigma_ay = 0.08;
sigma_az = 0.08;
sigma_au = 0.05;

sigma_eps_micro = 10;
sigma_eps = sigma_eps_micro * 1e-6;

R = diag([sigma_ax^2, sigma_ay^2, sigma_az^2, sigma_au^2, sigma_eps^2]);

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
    x_pred = A * xhat(:,k-1);
    P_pred = A * P * A' + Q;

    z_full = [ax(k); ay(k); az(k); au(k); eps_meas(k)];

    valid = ~isnan(z_full);
    zk = z_full(valid);
    Hk = H(valid,:);
    Rk = R(valid,valid);

    if isempty(zk)
        xhat(:,k) = x_pred;
        P = P_pred;
        continue;
    end

    S = Hk * P_pred * Hk' + Rk;
    K = P_pred * Hk' / S;

    xhat(:,k) = x_pred + K * (zk - Hk*x_pred);
    P = (eye(9) - K*Hk) * P_pred;
end

%% Extract estimates
ax_est = xhat(3,:)';
ay_est = xhat(6,:)';
az_est = xhat(9,:)';

dx = xhat(1,:)';
dy = xhat(4,:)';
dz = xhat(7,:)';

eps_est = c_eps(1)*dx + c_eps(2)*dy + c_eps(3)*dz;
eps_est_micro = eps_est * 1e6;

%% Metrics
valid_eps = ~isnan(eps_micro) & ~isnan(eps_est_micro);
valid_ax  = ~isnan(ax) & ~isnan(ax_est);
valid_ay  = ~isnan(ay) & ~isnan(ay_est);
valid_az  = ~isnan(az) & ~isnan(az_est);

fprintf('Valid strain samples : %d\n', nnz(valid_eps));

if nnz(valid_eps) > 0
    rmse_eps = sqrt(mean((eps_micro(valid_eps) - eps_est_micro(valid_eps)).^2));
else
    rmse_eps = NaN;
end

rmse_ax = sqrt(mean((ax(valid_ax) - ax_est(valid_ax)).^2));
rmse_ay = sqrt(mean((ay(valid_ay) - ay_est(valid_ay)).^2));
rmse_az = sqrt(mean((az(valid_az) - az_est(valid_az)).^2));

if nnz(valid_eps) > 1
    C = corrcoef(eps_micro(valid_eps), eps_est_micro(valid_eps));
    corr_eps = C(1,2);
else
    corr_eps = NaN;
end

if nnz(valid_ax) > 1
    C = corrcoef(ax(valid_ax), ax_est(valid_ax));
    corr_ax = C(1,2);
else
    corr_ax = NaN;
end

if nnz(valid_ay) > 1
    C = corrcoef(ay(valid_ay), ay_est(valid_ay));
    corr_ay = C(1,2);
else
    corr_ay = NaN;
end

if nnz(valid_az) > 1
    C = corrcoef(az(valid_az), az_est(valid_az));
    corr_az = C(1,2);
else
    corr_az = NaN;
end

fprintf('\nComparison Metrics\n');
fprintf('------------------\n');
fprintf('Strain RMSE      : %.3f microstrain\n', rmse_eps);
fprintf('Strain Corr      : %.4f\n', corr_eps);
fprintf('Ax RMSE          : %.4f\n', rmse_ax);
fprintf('Ax Corr          : %.4f\n', corr_ax);
fprintf('Ay RMSE          : %.4f\n', rmse_ay);
fprintf('Ay Corr          : %.4f\n', corr_ay);
fprintf('Az RMSE          : %.4f\n', rmse_az);
fprintf('Az Corr          : %.4f\n', corr_az);

%% Plots
figure;
plot(t, eps_est_micro, 'b-', 'LineWidth', 1.5); hold on;
plot(t, eps_micro, 'k.', 'MarkerSize', 8);
grid on;
xlabel('Time (s)');
ylabel('Strain (\mue)');
legend('Filtered', 'Measured');
title('Strain Comparison');

figure;
if nnz(valid_eps) > 0
    plot(t(valid_eps), eps_micro(valid_eps) - eps_est_micro(valid_eps), 'LineWidth', 1.2);
    grid on;
end
xlabel('Time (s)');
ylabel('Measured - Filtered (\mue)');
title('Strain Residual');

figure;
if nnz(valid_eps) > 1
    scatter(eps_micro(valid_eps), eps_est_micro(valid_eps), 12, 'filled'); hold on;
    xmin = min(eps_micro(valid_eps));
    xmax = max(eps_micro(valid_eps));
    plot([xmin xmax], [xmin xmax], 'r--', 'LineWidth', 1.2);
    grid on;
    axis equal;
    legend('Samples', 'y = x', 'Location', 'best');
end
xlabel('Measured Strain (\mue)');
ylabel('Filtered Strain (\mue)');
title('Measured vs Filtered Strain');