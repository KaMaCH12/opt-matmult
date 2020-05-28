/*
 * Matrix multiplication with and without blocking.
 *
 * Intel® Core™ i5-6600 CPU @ 3.30GHz
 *
 * $ ./matmult -n 1024 -v 0
 * Time elapsed: 3.052755 seconds.
 * $ ./matmult -n 1024 -v 1
 * Time elapsed: 0.746337 seconds.
 * $ ./matmult -n 1024 -v 2
 * Time elapsed: 9.882309 seconds.
 * $ ./matmult -n 1024 -v 3
 * Time elapsed: 0.698795 seconds.
 */
#include "matmult.h"

/* Useful macro for accessing row-major 2D arrays of size n×n. */
#define M(a, i, j) a[(i) * n + (j)]

/* ijk (& jik) */
void matmult0(int n, T_p a, T_p b, T_p c) {
  /* TODO: Fill in this procedure! */
}

/* kij (& ikj) */
void matmult1(int n, T_p a, T_p b, T_p c) {
  /* TODO: Fill in this procedure! */
}

/* jki (& kji) */
void matmult2(int n, T_p a, T_p b, T_p c) {
  /* TODO: Fill in this procedure! */
}

/* BLOCK*BLOCK tiled version */
void matmult3(int n, T_p a, T_p b, T_p c) {
  /* TODO: Fill in this procedure! */
}
