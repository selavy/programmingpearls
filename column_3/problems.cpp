#include "catch.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstring>

namespace ch3 {

using Real = double;

template <size_t K>
using Reals = std::array<Real, K>;

// TODO(peter): I think I should be able to vectorize this
template <int K, int M>
Reals<M> recurrence(Reals<K> as, Reals<K+1> cs) noexcept
{
    Reals<M> output;
    memcpy(&output[0], &as[0], sizeof(as[0])*K);
    for (int n = K; n < M; ++n) {
        // Calculate the new value
        Real r = cs[K];
        for (int i = 0; i < K; ++i) {
            r += as[K-i-1]*cs[i];
        }
        // TODO(peter): fold propogate into the calculate new value stage
        // Propogate the values backward
        for (int i = 0; i < K-1; ++i) {
            as[i] = as[i+1];
        }
        as[K-1] = r;
        output[n] = r;
    }
    return output;
}

// TODO(peter): definitely should be able to vectorize this one
void rec2(int K, int M, double* a, const double* c) noexcept {
    for (int n = K; n < M; ++n) {
        a[n] = c[K];
        for (int i = 0; i < K; ++i) {
            a[n] += a[n-i-1]*c[i];
        }
    }
}

}

TEST_CASE("Problem #2", "k-th order linear recurrence")
{
    // A kth-order linear recurrence with constant coefficients defines a series as
    // a_n = c_1*a_n-1 + c_2*a*_n-2 + ... + c_k*a_n-k + c_k+1
    // where c_1, ..., c_k+1 are real numbers. Write a program that with input k, a_1, ...,
    // a_k, c_1, ..., c_k+1, and m produces the output a_1 through a_m. How difficult is that
    // program compared to a program that evaluates one particular fifth-order recurrence,
    // but does so without using arrays?

    SECTION("Simple case")
    {
        constexpr auto K = 1;
        constexpr auto M = 4;
        ch3::Reals<K>   as = { 2 };
        ch3::Reals<K+1> cs = { 2, 1 };
        auto output = ch3::recurrence<K, M>(as, cs);

        auto c_1 = cs[0];
        auto c_2 = cs[1];
        auto a_1 = as[0];
        REQUIRE(output[0] == a_1);
        auto a_2 = c_1*a_1 + c_2;
        REQUIRE(output[1] == a_2);
        auto a_3 = c_1*a_2 + c_2;
        REQUIRE(output[2] == a_3);
        auto a_4 = c_1*a_3 + c_2;
        REQUIRE(output[3] == a_4);

        REQUIRE(output == ch3::Reals<M>{2, 5, 11, 23});

        // Version #2
        double A[M];
        memcpy(&A[0], &as[0], sizeof(as[0])*as.size());
        ch3::rec2(K, M, &A[0], &cs[0]);
        REQUIRE(A[0] ==  2);
        REQUIRE(A[1] ==  5);
        REQUIRE(A[2] == 11);
        REQUIRE(A[3] == 23);
    }

    SECTION("Harder case")
    {
        constexpr auto K = 3;
        constexpr auto M = 6;
        ch3::Reals<K>   a = { 1, 5, 7 };
        ch3::Reals<K+1> c = { 2, 3, 4, 5 };
        ch3::Reals<M> out = ch3::recurrence<K, M>(a, c);

        auto c_1 = c[0];
        auto c_2 = c[1];
        auto c_3 = c[2];
        auto c_4 = c[3];

        auto a_1 = a[0];
        auto a_2 = a[1];
        auto a_3 = a[2];

        REQUIRE(out[0] == a_1);
        REQUIRE(out[1] == a_2);
        REQUIRE(out[2] == a_3);

        auto a_4 = c_1*a_3 + c_2*a_2 + c_3*a_1 + c_4;
        REQUIRE(out[3] == a_4);

        auto a_5 = c_1*a_4 + c_2*a_3 + c_3*a_2 + c_4;
        REQUIRE(out[4] == a_5);

        auto a_6 = c_1*a_5 + c_2*a_4 + c_3*a_3 + c_4;
        REQUIRE(out[5] == a_6);

        // Version #2
        double A[M];
        memcpy(&A[0], &a[0], sizeof(a[0])*a.size());
        ch3::rec2(K, M, &A[0], &c[0]);
        REQUIRE(A[0] == a_1);
        REQUIRE(A[1] == a_2);
        REQUIRE(A[2] == a_3);
        REQUIRE(A[3] == a_4);
        REQUIRE(A[4] == a_5);
        REQUIRE(A[5] == a_6);
    }
}
