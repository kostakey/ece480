clear; clc; close all;

%% Load data
T = readtable('drop_transient_data.csv'); % change this depending on what file you are running

t = T.time(:);
ax = T.accel_x(:);
ay = T.accel_y(:);
az = T.accel_z(:);
au = T.accel_uni(:);

% Strain channel is instrumentation amplifier output voltage
eps_amp = T.strain(:);

N = length(t);
dt = mean(diff(t));

%% Remove initial offsets
n_bias = min(200, floor(N/10));

% Accelerometer bias removal
ax = ax - mean(ax(1:n_bias));
ay = ay - mean(ay(1:n_bias));
az = az - mean(az(1:n_bias));
au = au - mean(au(1:n_bias));

% Strain amplifier parameters
Vbias = 1.65;   % instrumentation amplifier output bias (V)
G_amp = 13;     % instrumentation amplifier gain
GF = 2.04;      % strain gauge factor
Vex = 3.3;      % bridge excitation voltage (V)

% Remove amplifier bias and initial static offset
Vstrain = eps_amp - Vbias;
Vstrain = Vstrain - mean(Vstrain(1:n_bias));

% Convert to physical strain
eps_meas = Vstrain / (G_amp * GF * Vex);   % unitless strain
eps_micro = eps_meas * 1e6;                % microstrain

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

% Strain measurement: epsilon = c_x*dx + c_y*dy + c_z*dz
c_eps = [1200 0 0];   % example: strain mostly tied to x displacement
H_strain = [c_eps(1) 0 0  c_eps(2) 0 0  c_eps(3) 0 0];

H = [H_acc3;
     H_uni;
     H_strain];

%% Noise covariances
sigma_ax = 0.08;
sigma_ay = 0.08;
sigma_az = 0.08;
sigma_au = 0.05;

% Strain noise in microstrain, converted to unitless strain
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

% Initial guess
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
    z_full = [ax(k); ay(k); az(k); au(k); eps_meas(k)];

    % Allow missing values
    valid = ~isnan(z_full);
    zk = z_full(valid);
    Hk = H(valid,:);
    Rk = R(valid,valid);

    % Update
    S = Hk * P_pred * Hk' + Rk;
    K = P_pred * Hk' / S;

    xhat(:,k) = x_pred + K * (zk - Hk*x_pred);
    P = (eye(9) - K*Hk) * P_pred;
end

%% Extract results
dx = xhat(1,:)';
vx = xhat(2,:)';
ax_est = xhat(3,:)';

dy = xhat(4,:)';
vy = xhat(5,:)';
ay_est = xhat(6,:)';

dz = xhat(7,:)';
vz = xhat(8,:)';
az_est = xhat(9,:)';

eps_est = c_eps(1)*dx + c_eps(2)*dy + c_eps(3)*dz;
eps_est_micro = eps_est * 1e6;

%% Plots
figure;
plot(t, ax, '.', 'MarkerSize', 4); hold on;
plot(t, ax_est, 'LineWidth', 1.5);
grid on;
xlabel('Time (s)');
ylabel('a_x (g)');
legend('Measured', 'Estimated');
title('X Acceleration');

% saveas(gcf, 'drop_x_accel.png');

figure;
plot(t, ay, '.', 'MarkerSize', 4); hold on;
plot(t, ay_est, 'LineWidth', 1.5);
grid on;
xlabel('Time (s)');
ylabel('a_y (g)');
legend('Measured', 'Estimated');
title('Y Acceleration');

% saveas(gcf, 'drop_y_accel.png');

figure;
plot(t, az, '.', 'MarkerSize', 4); hold on;
plot(t, az_est, 'LineWidth', 1.5);
grid on;
xlabel('Time (s)');
ylabel('a_z (g)');
legend('Measured', 'Estimated');
title('Z Acceleration');

% saveas(gcf, 'drop_z_accel.png');

figure;
plot(t, eps_micro, '.', 'MarkerSize', 4); hold on;
plot(t, eps_est_micro, 'LineWidth', 1.5);
grid on;
xlabel('Time (s)');
ylabel('Strain (\mue\epsilon)');
legend('Measured', 'Estimated');
title('Strain');

% saveas(gcf, 'drop_strain.png');
