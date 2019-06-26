//
// Created by juraj on 26/06/19.
//

#include <stdio.h>

#include <math.h>

#include "eig3.h"

int main() {

    printf("Hello world! sqrt(2) = %f", sqrt(2.0));

    double A[3][3] = {
            10, 5, 1,
            5, 7, 2,
            1, 2, 3
    };

    double V[3][3] = {0};

    double d[3] = {0};

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


    return 0;
}