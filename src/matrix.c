#include "matrix.h"

matrix_t s21_create_matrix(int rows, int columns) {
    matrix_t A = {NULL, rows, columns, ZERO_MATRIX};
    if (rows > 0 && columns > 0) {
        A.matrix = (double **)malloc(rows * sizeof(double *));
        for (int i = 0; i < rows; i++) {
            A.matrix[i] = (double *)malloc(columns * sizeof(double));
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (A.matrix[i]) A.matrix[i][j] = 0;
            }
        }
    } else {
        A.matrix_type = INCORRECT_MATRIX;
    }
    return A;
}

void s21_remove_matrix(matrix_t *A) {
    if (A->matrix) {
        for (int i = 0; i < A->rows; i++) {
            free(A->matrix[i]);
        }
        free(A->matrix);
    }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
    int result = SUCCESS;
    if (equalSizes(A, B)) {
        for (int i = 0; i < A->rows && result == SUCCESS; i++) {
            for (int j = 0; j < A->columns && result == SUCCESS; j++) {
                if (A->matrix[i][j] != B->matrix[i][j]) result = FAILURE;
            }
        }
    } else {
        result = FAILURE;
    }
    return result;
}

matrix_t createIdentityMatrix(int size) {
    matrix_t A = s21_create_matrix(size, size);
    for (int i = 0; i < size; i++) {
        A.matrix[i][i] = 1;
    }
    A.matrix_type = IDENTITY_MATRIX;
    return A;
}

void setMatrixType(matrix_t *A) {
    matrix_t zero = s21_create_matrix(A->rows, A->columns);
    if (s21_eq_matrix(A, &zero)) {
        A->matrix_type = ZERO_MATRIX;
    } else if (squareMatrix(A)) {
        matrix_t ident = createIdentityMatrix(A->rows);
        if (s21_eq_matrix(A, &ident)) {
            A->matrix_type = IDENTITY_MATRIX;
        } else {
            A->matrix_type = CORRECT_MATRIX;
        }
        s21_remove_matrix(&ident);
    } else {
        A->matrix_type = CORRECT_MATRIX;
    }
    s21_remove_matrix(&zero);
}

matrix_t s21_mult_matrix(matrix_t *A, matrix_t *B) {
    matrix_t result = {NULL, 0, 0, INCORRECT_MATRIX};
    if (canMult(A, B)) {
        result = s21_create_matrix(A->rows, B->columns);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < B->columns; j++) {
                for (int c = 0; c < A->columns; c++)
                    result.matrix[i][j] += A->matrix[i][c] * B->matrix[c][j];
            }
        }
        setMatrixType(&result);
    }
    return result;
}

