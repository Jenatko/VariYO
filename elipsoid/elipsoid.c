
#include <stdio.h>
#include <stdlib.h>
 
/*
D = [ x .* x + y .* y - 2 * z .* z, ...
        x .* x + z .* z - 2 * y .* y, ...
        2 * x .* y, ...
        2 * x .* z, ...
        2 * y .* z, ...
        2 * x, ...
        2 * y, ...
        2 * z, ...
        1 + 0 * x ];  % ndatapoints x 9 ellipsoid parameters
        
        % solve the normal system of equations
d2 = x .* x + y .* y + z .* z; % the RHS of the llsq problem (y's)
u = ( D' * D ) \ ( D' * d2 );  % solution to the normal equations
*/
 
int fit_elipsoid(float *x, float *y, float *z, int n, float *res) {

    /**
     * The overdetermined system to be solved. */
    float * D;
    D = (float *) malloc(sizeof(float) * n * 9);
    
    for (int i = 0; i < n; ++i) {
        D[9 * i + 0] = x[i] * x[i] + y[i] * y[i] - 2 * z[i] * z[i];
        D[9 * i + 1] = x[i] * x[i] + z[i] * z[i] - 2 * y[i] * y[i];
        D[9 * i + 2] = 2 * x[i] * y[i];
        D[9 * i + 3] = 2 * x[i] * z[i];
        D[9 * i + 4] = 2 * y[i] * z[i];
        D[9 * i + 5] = 2 * x[i];                // optimize away
        D[9 * i + 6] = 2 * y[i];                // optimize away
        D[9 * i + 7] = 2 * z[i];                // optimize away
        D[9 * i + 8] = 1;                       // same here
    }
    
    /**
     * The right hand side */
    float * d2;
    d2 = (float *) malloc(sizeof(float) * n);
    for (int i = 0; i < n; ++i) {
        d2[i] = x[i] * x[i] + y[i] * y[i] + z[i] * z[i];
    }
    
    /**
     * Matrix to be solved M = D' * D */
    float M[9][9] = {0};
    for (int i = 0; i < 9; ++i) {
        for (int j = i; j < 9; ++j) {
            for (int k = 0; k < n; ++k) {
                M[i][j] += D[9 * k + i] * D[9 * k + j];
            }
        }
    }
    
    //Symmetric matrix
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < i; ++j) {
            M[j][i] = M[i][j];
        }
    }
    
    /**
     * The right hand side */
    float d[9] = {0};
    for (int j = 0; j < 9; ++j) {
        for (int k = 0; k < n; ++k) {
            d[j] += D[9 * k + j] * d2[k];
        }
    }
    
    
    
    float sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += D[9*i];
    }
    
    res[0] = sum;
    return 0;
}

 
int main(){
     
    float x[10] = {1,2,3,4,5,6,7,8,9,0};
    float y[10] = {0,1,2,3,4,5,6,7,8,9};
    float z[10] = {1,2,3,4,5,6,7,8,9,10};
     
    float res[3] = {0,0,0};
    int ret = fit_elipsoid(x, y, z, 10, res);
    
    printf("Hello world!\n");
    printf("The result is: %f, %f, %f, return code: %d\n", res[0], res[1], res[2], ret);
    return 0;
 }
 
