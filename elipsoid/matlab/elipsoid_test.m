M2 = [11,12,13;
        21,22,23;
        31,32,33];
    
M = [1, 2, 3;
        7,-7, 1;
        3, 2, 1];
    
b = [1;2;3];

x = [1,0,0,-.9, .4, 0.0, 0.0,  0.01 ,-.99,-.4, 0.0,  0.0];
y = [0,2,0,-.1,-.8, 0.0,-1.9,  2.004, .01, .8, 0.01, 1.9];
z = [0,0,3,  0,0.0,-3.1, 2.9,  3.04 ,  0, .01, 3.3, -2.9];
   

X = [   10, 5, 1;...
        5, 7, 2;...
        1, 2, 3];


%% Test determinant
det(M2)

%% test lin solver
M \ b

%% test eig
[e_vec, e_val] = eig(X)

%% test elipsoid fit
[center, radii, evecs, pars ] = ellipsoid_fit([x', y', z']);
[center2, radii2, evecs2, pars2 ] = ellipsoid_fit_tweek([x', y', z']);

any ([...
center' - center2',...
radii' - radii2',...
evecs(:)' - evecs2(:)',...
pars' - pars2'] > 1.0e-14)
