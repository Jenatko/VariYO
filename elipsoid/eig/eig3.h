
/* Eigen-decomposition for symmetric 3x3 real matrices.
   Public domain, copied from the public domain Java library JAMA. */

#ifndef _eig_h

/* Symmetric matrix A => eigenvectors in columns of V, corresponding
   eigenvalues in d. */
void eigen_decomposition(float A[3][3], float V[3][3], float d[3]);

#endif
