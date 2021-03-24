function n_phi = vecCal(n, theta, phi)
%���룺
%   n           3*1����
%   theta       �������нǣ������ƣ�>pi/2��<pi
%   phi         ��n��ת�Ƕȣ�������

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