    
#include <stdio.h>
#include <stdlib.h>
 
 
float * submatrix(float *M, int n, int k){
    float * subM = (float *) malloc((n-1) * (n-1) * sizeof (float));
    
    int jj = 0;
    for (int i = 1; i<n; i++) {
        jj = 0;
        for (int j = 0; j<n; j++) {
            if (j == k) {
                jj = 1;
                continue;
            }
            subM[(i-1) * (n-1) + (j - jj)] = M[i*n + j];
        }
    }
    
    return subM;
}
 
 
float determinant(float *M, int n){
        
    if (n < 1){
        printf("This shold not be possible!\n");
        return 1;
    }
    
    if (n == 1) {
        return M[0];
    }
    
    float result = 0;
    float sign = -1.0;
    for (int i = 0; i < n; i++) {
        sign *= -1.0;
        float * subM = submatrix(M, n, i);
        result += sign * M[i] * determinant( subM, n-1);
        free(subM);
    }
    
    return result;
}


/**
 * Solving linear system using Cramer's rule
 * 
 * @param M matrix n x n
 * @param b right hand side 1 x n
 * @param n size of the matrix 
 * @param x solution
 */
int solve_lin_system(float * M, float * b, int n, float * x){
    
    if (n<1) return -1;
    
    if (n == 1) {
        if (M[0] == 0) {
            return -1;
        }
        x[0] = b[0] / M[0];
        return 0;
    }
    
    float detM = determinant(M, n);
    if (detM == 0) {
        return -1;
    }
    
    float * v = (float *) malloc (n * sizeof(float));
    
    for (int i = 0; i<n; ++i){
        for (int j =0; j < n; ++j){
            v[j] = M[j * n + i];
            M[j * n + i] = b[j];
        }
        float detMi = determinant(M, n);

        x[i] = detMi / detM;
        
        for (int j =0; j < n; ++j){
            M[j * n + i] = v[j];
        }
    }
    
    free(v);
    return 0;
}
 
/*
D = [ x .* x + y .* y - 2 * z .* z, ...
        x .* x + z .* z - 2 * y .* y, ...
        2 * x .* y, ...
        2 * x .* z, ...
        2 * y .* z, ...
        2 * x, ...
        2 * y, ...
        2 * z, ...
        1 + 0 * x ];  % ndatapoints x 9 ellipsoid parameterspo
        
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
    float * d2 = (float *) malloc(sizeof(float) * n);
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
    
    free(D);

    
    /**
     * Solve the lin system
     */
    float u[9] = {0};    
    int flag = solve_lin_system((float *)M, d, 9, u);
    
    if (flag) {
        return flag;
    }
    
    float v[10] = {0};
    /*    
    v(1) = u(1) +     u(2) - 1;
    v(2) = u(1) - 2 * u(2) - 1;
    v(3) = u(2) - 2 * u(1) - 1;
    v( 4 : 10 ) = u( 3 : 9 );
    */
    v[0] = u[0] +     u[1] - 1;
    v[1] = u[0] - 2 * u[1] - 1;
    v[2] = u[1] - 2 * u[0] - 1;
    
    for (int i=3; i<10; ++i){
        v[i] = u[i-1];
    }
    
    /**
    % form the algebraic form of the ellipsoid
    A = [ v(1) v(4) v(5) v(7); ...
      v(4) v(2) v(6) v(8); ...
      v(5) v(6) v(3) v(9); ...
      v(7) v(8) v(9) v(10) ];
      */
    float A[16] = {
        v(1), v(4), v(5), v(7), 
        v(4), v(2), v(6), v(8),
        v(5), v(6), v(3), v(9),
        v(7), v(8), v(9), v(10)
    };
    
    /*
    % find the center of the ellipsoid
    center = -A( 1:3, 1:3 ) \ v( 7:9 );
    */
    
    
    
    return flag;
}


void testSubM(float * M2) {
    
    for (int i = 0; i<3; ++i){
        float * subM = submatrix(M2, 3, i);
        printf("\nSubmatrix %d of M2 = \n %f\t%f\n %f\t%f\n", i, subM[0], subM[1], subM[2], subM[3]);
        printf("SubSubmatrix: \n");
        for (int j = 0; j<2; ++j){    
            float * subsubM = submatrix(subM, 2, j);
            printf("\t%f\n", subsubM[0]);
            free(subsubM);
        }
        free(subM);
    }
    
}

void testDet(){
        float M[9] = {
        1,0,0,
        0,1,0,
        0,0,1
    };
    
    float M2[9] = {
        11,12,13,
        21,22,23,
        31,32,33
    };
    
//     testSubM(M2);    
//     testSubM(M);
    
    printf("Determinat of I = %f\n", determinant(M, 3));
    printf("Determinat of M2 = %f\n", determinant(M2, 3));
}

void testLinSolv(){
    /***************************
     * Matlab solution:
     * 
     * ans =
     *   
     *    0.7727
     *    0.4545
     *   -0.2273
     */
    float M[9] = {
        1, 2, 3,
        7,-7, 1,
        3, 2, 1
    };
    
    float b[3] = {1,2,3};
    
    float v[3] = {0};
    
    int ret = solve_lin_system(M, b, 3, v);
    
    printf("Solution to the system is: \n\t%f \nx = \t%f \n\t%f\n\n", v[0], v[1], v[2]);
    
}

int main(){
     
    //testLinSolv();
    
    float x[10] = {1,2,3,4,5,6,7,8,9,0};
    float y[10] = {0,1,2,3,4,5,6,7,8,9};
    float z[10] = {1,2,3,4,5,6,7,8,9,10};
     
    float res[3] = {0,0,0};
    int ret = fit_elipsoid(x, y, z, 10, res);
    
    printf("Hello world!\n");
    printf("The result is: %f, %f, %f,\n%f %f %f\n %f %f %f\n return code: %d\n", res[0], res[1], res[2], res[3], res[4], res[5], res[6], res[7], res[8], ret);
    return 0;
    
 }
 
 
