#include <iostream>
#include <vector>
#include <cmath>
#include <boost/math/constants/constants.hpp>

template<typename Real>
std::vector<Real> eigenvalues(const Real A[3][3])
{
    using boost::math::constants::third;
    using boost::math::constants::pi;
    using boost::math::constants::half;

    static_assert(std::numeric_limits<Real>::is_iec559,
                  "Template argument must be a floating point type.\n");

    std::vector<Real> eigs(3, std::numeric_limits<Real>::quiet_NaN());
    auto p1 = A[0][1]*A[0][1] + A[0][2]*A[0][2] + A[1][2]*A[1][2];
    auto diag_sq = A[0][0]*A[0][0] + A[1][1]*A[1][1] + A[2][2];
    if (p1 == 0 || 2*p1/(2*p1 + diag_sq) < std::numeric_limits<Real>::epsilon())
    {
        eigs[0] = A[0][0];
        eigs[1] = A[1][1];
        eigs[2] = A[2][2];
        return eigs;
    }

    auto q = third<Real>()*(A[0][0] + A[1][1] + A[2][2]);
    auto p2 = (A[0][0] - q)*(A[0][0] - q) + (A[1][1] - q)*(A[1][1] -q) + (A[2][2] -q)*(A[2][2] -q) + 2*p1;
    auto p = std::sqrt(p2/6);
    auto invp = 1/p;
    Real B[3][3];
    B[0][0] = A[0][0] - q;
    B[0][1] = A[0][1];
    B[0][2] = A[0][2];
    B[1][1] = A[1][1] - q;
    B[1][2] = A[1][2];
    B[2][2] = A[2][2] - q;
    auto detB = B[0][0]*(B[1][1]*B[2][2] - B[1][2]*B[1][2])
              - B[0][1]*(B[0][1]*B[2][2] - B[1][2]*B[0][2])
              + B[0][2]*(B[0][1]*B[1][2] - B[1][1]*B[0][1]);
    auto r = invp*invp*invp*half<Real>()*detB;
    if (r >= 1)
    {
        eigs[0] = q + 2*p;
        eigs[1] = q - p;
        eigs[2] = 3*q - eigs[1] - eigs[0];
        return eigs;
    }

    if (r <= -1)
    {
        eigs[0] = q + p;
        eigs[1] = q - 2*p;
        eigs[2] = 3*q - eigs[1] - eigs[0];
        return eigs;
    }

    auto phi = third<Real>()*std::acos(r);
    eigs[0] = q + 2*p*std::cos(phi);
    eigs[1] = q + 2*p*std::cos(phi + 2*third<Real>()*pi<Real>());
    eigs[2] = 3*q - eigs[0] - eigs[1];

    return eigs;
}

int main()
{
    float M[3][3];
    M[0][0] = 1.25e6;
    M[1][0] = 1.25e6;
    M[0][1] = 1.25e6;
    M[1][1] = 1.25e6;
    M[0][2] = 0.0;
    M[2][0] = 0.0;
    M[1][2] = 0.0;
    M[2][1] = 0.0;
    M[2][2] = 0.0;

    auto eigs = eigenvalues<float>(M);
    std::cout << "Eigenvalues: ";
    std::cout.precision(std::numeric_limits<float>::digits10 + 5);
    std::cout << std::fixed << std::scientific;
    for (auto eig : eigs)
    {
        std::cout << eig << ", ";
    }
    std::cout << '\n';
}
