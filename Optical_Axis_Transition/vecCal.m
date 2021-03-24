function n_phi = vecCal(n, theta, phi)
%输入：
%   n           3*1向量
%   theta       两向量夹角，弧度制，>pi/2，<pi
%   phi         绕n旋转角度，弧度制

n = n / norm(n);

alpha = atan(n(2) / norm([n(1),n(3)]));

m = - tan(pi - theta + alpha) * norm([n(1),n(3)]);

n0 = [-n(1); m; -n(3)];
n0 = n0 / norm(n0);

n_cross = [    0, -n(3),  n(2);
            n(3),     0, -n(1);
           -n(2),  n(1),     0];
R_phi = cos(phi) * eye(3) + (1 - cos(phi)) * n * n' - sin(phi) * n_cross; %#ok<MHERM>

n_phi = R_phi' * n0;
end