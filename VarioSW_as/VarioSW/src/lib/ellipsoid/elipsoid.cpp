    
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
#include "eig/eig3.h"

#define VERBOSE 0

 
float sign(float x) {
    return (x > 0) - (x < 0);
}
 
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

void print_matrix(float *M, int ni, int nj){

    for (int i = 0; i<ni; ++i) {
        for (int j = 0; j<nj; ++j){
            printf("%f\t", M[i * nj + j]);
        }
        printf("\n");
    }
    printf("\n");
}


float D_6_k_i(float *x, float *y, float *z, int k, int i) {
    switch (k) {
        case 0: return x[i] * x[i] + y[i] * y[i] - 2 * z[i] * z[i];
        case 1: return x[i] * x[i] + z[i] * z[i] - 2 * y[i] * y[i];
        case 2: return 2 * x[i];                // optimize away
        case 3: return 2 * y[i];                // optimize away
        case 4: return 2 * z[i];                // optimize away
        case 5: return 1;                       // same here
        default: return 0;
    }
}

int compute_axis_aligned_v(float *x, float *y, float *z, int n, float v[10]) {
        
    /**
     * The right hand side 
     */
    float d[6] = {0};

    for (int i = 0; i < n; ++i) {
        float d2_k = x[i] * x[i] + y[i] * y[i] + z[i] * z[i];
    
        d[0] += d2_k * (x[i] * x[i] + y[i] * y[i] - 2 * z[i] * z[i]);
        d[1] += d2_k * (x[i] * x[i] + z[i] * z[i] - 2 * y[i] * y[i]);
        d[2] += d2_k * 2 * x[i];                // optimize away
        d[3] += d2_k * 2 * y[i];                // optimize away
        d[4] += d2_k * 2 * z[i];                // optimize away
        d[5] += d2_k;                       // same here
    }
    
    
    /**
     * Matrix to be solved M = D' * D */
    float M[6][6] = {0};
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < 6; ++i) {
            for (int j = i; j < 6; ++j) {
                M[i][j] += D_6_k_i(x,y,z, i, k) * D_6_k_i(x,y,z, j, k);
            }
        }
    }
    //Symmetric matrix
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < i; ++j) {
            M[i][j] = M[j][i];
        }
    }
        
#if VERBOSE
    printf("M = \n");
    print_matrix((float *) M, 6, 6);
    printf("d = \n");
    print_matrix(d, 6, 1);
#endif
    
    /**
     * Solve the lin system
     */
    float u[6] = {0};    
    int flag = solve_lin_system((float *) M, d, 6, u);
    
    if (flag) {
        return flag;
    }
    
    /*    
    v(1) = u(1) +     u(2) - 1;
    v(2) = u(1) - 2 * u(2) - 1;
    v(3) = u(2) - 2 * u(1) - 1;
    v = [ v(1) v(2) v(3) 0 0 0 u( 3 : 6 )' ];
    */
    v[0] = u[0] +     u[1] - 1;
    v[1] = u[0] - 2 * u[1] - 1;
    v[2] = u[1] - 2 * u[0] - 1;
    v[3] = 0;
    v[4] = 0;
    v[5] = 0;
    
    for (int i=6; i<10; ++i){
        v[i] = u[i-4];
    }
    
#if VERBOSE    
    printf("v = \n");
    print_matrix(v, 1, 10);
#endif
    
    return flag;
}

/**
 * Axis algined elipsoid fit of an array of points
 * @param x |
 * @param y | - coordinate arrays of @param n floating points
 * @param z |
 * @param n length of coord arrays
 * @param res return array of length 6, rx, ry, rz, cx, cy, cz
 */
int fit_elipsoid(float *x, float *y, float *z, int n, float res_cent[3], float res_radii[3]) {
    
    float v[10] = {0};
   
    int flag = 0;
   
    flag = compute_axis_aligned_v(x, y, z, n, v);
    if (flag) {
        return flag;
    }
   
    float center[3] = {0};
    
    if (v[0] == 0.0 || v[1] == 0.0 || v[2] == 0.0) {
        return -1;
    }
    
    center[0] = - v[6] / v[0];
    center[1] = - v[7] / v[1];
    center[2] = - v[8] / v[2];

#if VERBOSE
    printf("center = \n");
    print_matrix(center, 3, 1);
#endif
    
    float eig_val[3] = {-v[0], -v[1], -v[2]};
    
    float R44 = v[9]    + center[0] * v[6]
                        + center[1] * v[7]
                        + center[2] * v[8];   

#if 1
    printf("R(4,4) = %f\n", R44);
    printf("eig_val: (%f, %f, %f)\n\n", eig_val[0] / R44, eig_val[1] / R44, eig_val[2] / R44);
#endif
    
    /**
     * Compute the radii
     * radii = sqrt( 1 ./ diag( abs( evals ) ) );
     * sgns = sign( diag( evals ) );
     * radii = radii .* sgns;
     */
    float radii[3] = {0};
    for (int i=0; i<3; ++i) {
        float signum = sign(eig_val[i]);
        float abs_val = signum * eig_val[i];
        if ( abs_val >  0.000001) {
            radii[i] = signum / sqrt(abs_val / R44);
        } else {
            printf("Bad eigenvalue %d: %f\n", i, eig_val[i]);
            flag = -1;
        }
    }

    res_radii[0] = radii[0];
    res_radii[1] = radii[1];
    res_radii[2] = radii[2];            

    res_cent[0] = center[0];
    res_cent[1] = center[1];
    res_cent[2] = center[2];            

    return flag;
}

/******************************
 * TESTING
 */

#ifdef TEST
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
    const char solution[] = "\
/***************************\n\
 * Matlab solution:         \n\
 *                          \n\
 * ans =                    \n\
 *                          \n\
 *    0.7727                \n\
 *    0.4545                \n\
 *   -0.2273                \n\
 **************************/";
     
    float M[9] = {
        1, 2, 3,
        7,-7, 1,
        3, 2, 1
    };
    
    float b[3] = {1,2,3};
    
    float v[3] = {0};
    
    int ret = solve_lin_system(M, b, 3, v);
    
    printf("Solution to the system is: \n\t%f \nx = \t%f \n\t%f\n\n", v[0], v[1], v[2]);
    printf("%s", solution);
}

void test_eig(){

static const char solution[] = "\
/*********************************\n\
 * Matlab solution                \n\
 * e_vec =                        \n\
 *                                \n\
 *     -0.2436    0.5774   -0.7793\n\
 *      0.5531   -0.5774   -0.6006\n\
 *     -0.7967   -0.5774   -0.1787\n\
 *                                \n\
 *                                \n\
 * e_val =                        \n\
 *                                \n\
 *     1.9172         0         0 \n\
 *          0    4.0000         0 \n\
 *          0         0   14.0828 \n\
 */";
 
    float A[3][3] = {
            10, 5, 1,
            5, 7, 2,
            1, 2, 3
    };

    float V[3][3] = {0};

    float d[3] = {0};

    eigen_decomposition(A, V, d);

    printf("A =\n");
    for (int i = 0; i<3; ++i) {
        for (int j = 0; j<3; ++j){
            printf("%f\t", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("V =\n");
    for (int i = 0; i<3; ++i) {
        for (int j = 0; j<3; ++j){
            printf("%f\t", V[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("d =\n");
    for (int j = 0; j<3; ++j){
        printf("%f\t", d[j]);
    }
    printf("\n");

    printf("%s", solution);
}

void test_elipsoid() {
    
    
    float x[12] = {1,0,0,-.9, .4, 0.0, 0.0,  0.01 ,-.99,-.4, 0.0,  0.0};
    float y[12] = {0,2,0,-.1,-.8, 0.0,-1.9,  2.004, .01, .8, 0.01, 1.9};
    float z[12] = {0,0,3,  0,0.0,-3.1, 2.9,  3.04 ,  0, .01, 3.3, -2.9};
     
    float res_radii[3] = {0};
    float res_cent[3] = {0};
    int ret = fit_elipsoid(x, y, z, 12, res_cent, res_radii);
    
    //printf("The result is:\n eig_v = \n\n %f %f %f\n %f %f %f\n %f %f %f\n\n radii = %f\t%f\t%f\n\n center = %f\t%f\t%f\n\n return code: %d\n", res[0], res[1], res[2], res[3], res[4], res[5], res[6], res[7], res[8], res[9], res[10], res[11], res[12], res[13], res[14], ret);
    
}

/*
int main(){
     
//     testLinSolv();
//     test_eig();
    test_elipsoid();
    return 0;
    
 }
 */
 
#endif
