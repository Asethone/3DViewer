#ifndef SRC_MATRIX_H_
#define SRC_MATRIX_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    CORRECT_MATRIX = 0,
    INCORRECT_MATRIX = 1,
    IDENTITY_MATRIX = 2,
    ZERO_MATRIX = 3
} matrix_type_t;

typedef struct matrix_struct {
    double **matrix;
    int rows;
    int columns;
    matrix_type_t matrix_type;
} matrix_t;

#define SUCCESS 1
#define FAILURE 0

#define validate2(A, B) \
    ((A) && (B) && (A)->matrix_type != INCORRECT_MATRIX && (B)->matrix_type != INCORRECT_MATRIX)
#define equalSizes(A, B) (validate2(A, B) && (A)->rows == (B)->rows && (A)->columns == (B)->columns)
#define canMult(A, B) (validate2(A, B) && (A)->columns == (B)->rows)
#define validate(A) ((A) && (A)->matrix_type != INCORRECT_MATRIX)
#define squareMatrix(A) ((A)->rows == (A)->columns)

matrix_t s21_create_matrix(int rows, int columns);
void s21_remove_matrix(matrix_t *A);
int s21_eq_matrix(matrix_t *A, matrix_t *B);
matrix_t s21_sum_matrix(matrix_t *A, matrix_t *B);
matrix_t s21_sub_matrix(matrix_t *A, matrix_t *B);
matrix_t s21_mult_number(matrix_t *A, double number);
matrix_t s21_mult_matrix(matrix_t *A, matrix_t *B);
matrix_t s21_transpose(matrix_t *A);
matrix_t s21_calc_complements(matrix_t *A);
double s21_determinant(matrix_t *A);
matrix_t s21_inverse_matrix(matrix_t *A);

#endif  // SRC_MATRIX_H_
