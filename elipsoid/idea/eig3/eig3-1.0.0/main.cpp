//
// Created by juraj on 26/06/19.
//

#include <iostream>

#include "eig3.h"

using namespace std;

int main() {

    cout << "Hello world!";


    double A[3][3] = {
            10, 5, 1,
             5, 7, 2,
             1, 2, 3
    };

    double V[3][3] = {0};

    double d[3] = {0};

    eigen_decomposition(A, V, d);

    cout << "A = " << endl;

    for (auto &i : A) {
        for (double j : i) {
            cout << j << " ";
        }
        cout << endl;
    }

    cout << "V = " << endl;

    for (auto &i : V) {
        for (double j : i) {
            cout << j << " ";
        }
        cout << endl;
    }

    cout << "d = " << endl;

    for (double k : d) {
        cout << k << ", ";
    }

    cout << endl;
}