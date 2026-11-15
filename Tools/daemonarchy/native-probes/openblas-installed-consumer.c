/* SPDX-License-Identifier: BSD-2-Clause */
#include <cblas.h>
#include <lapack.h>
#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static void require(int condition, const char *case_name)
{
    if (!condition) {
        fprintf(stderr, "FAIL %s\n", case_name);
        exit(1);
    }
}

static void real_result(const double *actual, const double *expected,
    size_t count, const char *case_name)
{
    for (size_t i = 0; i < count; ++i)
        require(isfinite(actual[i]) &&
            fabs(actual[i] - expected[i]) <= 1e-12 * (1 + fabs(expected[i])),
            case_name);
    printf("PASS %s\n", case_name);
}

static void row_major(void)
{
    const double a[] = {1, 2, 3, 4, 5, 6};
    const double b[] = {7, 8, 9, 10, 11, 12};
    double c[] = {-1, -1, -1, -1};
    const double expected[] = {57.5, 63.5, 138.5, 153.5};
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        2, 2, 3, 1, a, 3, b, 2, 0.5, c, 2);
    real_result(c, expected, 4, "dgemm row-major alpha/beta");
}

static void column_transpose(void)
{
    const double a[] = {1, 2, 3, 4, 5, 6};
    const double b[] = {7, 9, 11, 8, 10, 12};
    double c[4] = {0};
    const double expected[] = {58, 139, 64, 154};
    cblas_dgemm(CblasColMajor, CblasTrans, CblasNoTrans,
        2, 2, 3, 1, a, 3, b, 3, 0, c, 2);
    real_result(c, expected, 4, "dgemm column-major transpose");
}

static void pivoted_solver(void)
{
    double a[] = {0, 1, 2, 2, -2, 3, 1, -3, 1};
    double rhs[] = {-1, -4, -1};
    const double expected[] = {1, -2, 3};
    const lapack_int n = 3, nrhs = 1;
    lapack_int pivots[3] = {0}, info = -1;
    LAPACK_dgesv(&n, &nrhs, a, &n, pivots, rhs, &n, &info);
    require(info == 0, "dgesv status");
    for (size_t i = 0; i < 3; ++i)
        require(pivots[i] >= 1 && pivots[i] <= n, "dgesv pivots");
    require(pivots[0] != 1, "dgesv row interchange");
    real_result(rhs, expected, 3, "dgesv pivoted solution");
}

static void singular_solver(void)
{
    double a[] = {1, 2, 2, 4}, rhs[] = {3, 6};
    const lapack_int n = 2, nrhs = 1;
    lapack_int pivots[2] = {0}, info = -1;
    LAPACK_dgesv(&n, &nrhs, a, &n, pivots, rhs, &n, &info);
    require(info == 2, "dgesv singular status");
    puts("PASS dgesv singular status");
}

static void complex_conjugate(void)
{
    const double complex a[] = {1 + 2*I, 3 - I, -2 + 0.5*I, 4 + 3*I};
    const double complex b[] = {2 - I, -1 + 2*I, 3, 0.5 - I};
    double complex c[] = {I, -1 + 2*I, 2 - I, 3 + 0.25*I};
    double complex expected[4];
    const double complex alpha = 0.5 + 0.25*I, beta = -1 + 0.5*I;
    for (size_t row = 0; row < 2; ++row) {
        for (size_t col = 0; col < 2; ++col) {
            double complex sum = 0;
            for (size_t k = 0; k < 2; ++k)
                sum += conj(a[k*2 + row]) * b[k*2 + col];
            expected[row*2 + col] = alpha * sum + beta * c[row*2 + col];
        }
    }
    cblas_zgemm(CblasRowMajor, CblasConjTrans, CblasNoTrans,
        2, 2, 2, &alpha, a, 2, b, 2, &beta, c, 2);
    for (size_t i = 0; i < 4; ++i)
        require(isfinite(creal(c[i])) && isfinite(cimag(c[i])) &&
            cabs(c[i] - expected[i]) <= 1e-12 * (1 + cabs(expected[i])),
            "zgemm conjugate transpose");
    puts("PASS zgemm conjugate transpose");
}

int main(int argc, char **argv)
{
    require(argc == 2, "one expected thread-count argument required");
    int threads = atoi(argv[1]);
    require(threads == 1 || threads == 2, "expected threads 1 or 2");
    printf("OpenBLAS config: %s\n", openblas_get_config());
    printf("OpenBLAS core: %s\n", openblas_get_corename());
    printf("OpenBLAS parallel=%d threads=%d processors=%d\n",
        openblas_get_parallel(), openblas_get_num_threads(), openblas_get_num_procs());
    require(openblas_get_parallel() == OPENBLAS_OPENMP, "default OpenMP runtime");
    require(openblas_get_num_threads() == threads, "requested OpenMP thread count");
    row_major();
    column_transpose();
    pivoted_solver();
    singular_solver();
    complex_conjugate();
    puts("PASS all 5 installed-library numerical cases");
    return 0;
}
