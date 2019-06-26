M2 = [11,12,13;
        21,22,23;
        31,32,33];
    
M = [1, 2, 3;
        7,-7, 1;
        3, 2, 1];
    
b = [1;2;3];

x = [1,2,3,4,5,6,7,8,9,0];
y = [0,1,2,3,4,5,6,7,8,9];
z = [1,2,3,4,5,6,7,8,9,10];
   

%% Test determinant
det(M2)

%% test lin solver
M \ b

%% test elipsoid fit
[center, radii, evecs, pars ] = ellipsoid_fit([x', y', z']);
[center2, radii2, evecs2, pars2 ] = ellipsoid_fit_tweek([x', y', z']);

any ([...
center' - center2',...
radii' - radii2',...
evecs(:)' - evecs2(:)',...
pars' - pars2'])