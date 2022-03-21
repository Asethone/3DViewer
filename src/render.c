#include "render.h"

#if defined(TEST)
settings_t options = {0, 1, 0, 0, 1, 0, 0, 0};
#else
extern settings_t options;
#endif

static void applyTransformMatrix(obj_data_t *obj_data, matrix_t *transform_matrix);
static matrix_t getProjectionMatrix();
static matrix_t getTranslateMatrix(const double *trans_data);
static matrix_t getRotateXMatrix(double angle);
static matrix_t getRotateYMatrix(double angle);
static matrix_t getRotateZMatrix(double angle);
static matrix_t getScaleMatrix(double scale);

// Allocate memory for render_data
render_data_t *newRenderData(obj_data_t *obj_data) {
    render_data_t *render_data = g_new0(render_data_t, 1);
    render_data->v_cnt = obj_data->v_cnt;
    render_data->obj_data = obj_data;
    render_data->vertices = (double **)malloc(sizeof(double *) * render_data->v_cnt);
    for (int i = 0; i < render_data->v_cnt; i++) {
        render_data->vertices[i] = (double *)malloc(sizeof(double) * 4);
    }
    return render_data;
}

void renderModel(render_data_t *render_data) {
    matrix_t projection_matrix = getProjectionMatrix();
    // Make projection transition
    for (int i = 0; i < render_data->v_cnt; i++) {
        matrix_t coords_vector = s21_create_matrix(4, 1);
        coords_vector.matrix[0][0] = render_data->obj_data->vertices[i][0];
        coords_vector.matrix[1][0] = render_data->obj_data->vertices[i][1];
        coords_vector.matrix[2][0] = render_data->obj_data->vertices[i][2];
        coords_vector.matrix[3][0] = 1.0;

        matrix_t result = s21_mult_matrix(&projection_matrix, &coords_vector);

        render_data->vertices[i][0] = result.matrix[0][0];
        render_data->vertices[i][1] = result.matrix[1][0];
        render_data->vertices[i][2] = result.matrix[2][0];
        render_data->vertices[i][3] = result.matrix[3][0];

        s21_remove_matrix(&coords_vector);
        s21_remove_matrix(&result);
    }
    s21_remove_matrix(&projection_matrix);
}

void translateModel(obj_data_t *obj_data, double *trans_data) {
    matrix_t translation_matrix = getTranslateMatrix(trans_data);
    applyTransformMatrix(obj_data, &translation_matrix);
}

void rotateXModel(obj_data_t *obj_data, double angle) {
    matrix_t rotation_matrix = getRotateXMatrix(angle * M_PI / 180);
    applyTransformMatrix(obj_data, &rotation_matrix);
}

void rotateYModel(obj_data_t *obj_data, double angle) {
    matrix_t rotation_matrix = getRotateYMatrix(angle * M_PI / 180);
    applyTransformMatrix(obj_data, &rotation_matrix);
}

void rotateZModel(obj_data_t *obj_data, double angle) {
    matrix_t rotation_matrix = getRotateZMatrix(angle * M_PI / 180);
    applyTransformMatrix(obj_data, &rotation_matrix);
}

void scaleModel(obj_data_t *obj_data, double scale) {
    matrix_t scale_matrix = getScaleMatrix(scale);
    applyTransformMatrix(obj_data, &scale_matrix);
}

static void applyTransformMatrix(obj_data_t *obj_data, matrix_t *transform_matrix) {
    for (int i = 0; i < obj_data->v_cnt; i++) {
        matrix_t coords_vector = s21_create_matrix(4, 1);
        coords_vector.matrix[0][0] = obj_data->vertices[i][0];
        coords_vector.matrix[1][0] = obj_data->vertices[i][1];
        coords_vector.matrix[2][0] = obj_data->vertices[i][2];
        coords_vector.matrix[3][0] = 1.0;

        matrix_t result = s21_mult_matrix(transform_matrix, &coords_vector);

        obj_data->vertices[i][0] = result.matrix[0][0];
        obj_data->vertices[i][1] = result.matrix[1][0];
        obj_data->vertices[i][2] = result.matrix[2][0];

        s21_remove_matrix(&coords_vector);
        s21_remove_matrix(&result);
    }
    s21_remove_matrix(transform_matrix);
}

static matrix_t getProjectionMatrix() {
    matrix_t A = s21_create_matrix(4, 4);
    if (options.projection == PARALLEL) {
        A.matrix[0][0] = 0.707106781186548;
        A.matrix[0][2] = -0.707106781186548;
        A.matrix[1][0] = -0.408248290463863;
        A.matrix[1][1] = 0.816496580927726;
        A.matrix[1][2] = -0.408248290463863;
        A.matrix[3][3] = 1.0;
    } else {
        A.matrix[0][0] = 1;
        A.matrix[1][1] = 1;
        A.matrix[2][2] = 1;
        A.matrix[2][3] = -0.001;
        A.matrix[3][3] = 1;
    }
    return A;
}

static matrix_t getTranslateMatrix(const double *trans_data) {
    matrix_t A = s21_create_matrix(4, 4);
    A.matrix[0][0] = 1.0;
    A.matrix[0][3] = trans_data[0];
    A.matrix[1][1] = 1.0;
    A.matrix[1][3] = trans_data[1];
    A.matrix[2][2] = 1.0;
    A.matrix[2][3] = trans_data[2];
    A.matrix[3][3] = 1.0;
    return A;
}

static matrix_t getRotateXMatrix(double angle) {
    matrix_t A = s21_create_matrix(4, 4);
    A.matrix[0][0] = 1.0;
    A.matrix[1][1] = cos(angle);
    A.matrix[1][2] = -sin(angle);
    A.matrix[2][1] = sin(angle);
    A.matrix[2][2] = cos(angle);
    A.matrix[3][3] = 1.0;
    return A;
}

static matrix_t getRotateYMatrix(double angle) {
    matrix_t A = s21_create_matrix(4, 4);
    A.matrix[0][0] = cos(angle);
    A.matrix[0][2] = sin(angle);
    A.matrix[1][1] = 1.0;
    A.matrix[2][0] = -sin(angle);
    A.matrix[2][2] = cos(angle);
    A.matrix[3][3] = 1.0;
    return A;
}

static matrix_t getRotateZMatrix(double angle) {
    matrix_t A = s21_create_matrix(4, 4);
    A.matrix[0][0] = cos(angle);
    A.matrix[0][1] = -sin(angle);
    A.matrix[1][0] = sin(angle);
    A.matrix[1][1] = cos(angle);
    A.matrix[2][2] = 1.0;
    A.matrix[3][3] = 1.0;
    return A;
}

static matrix_t getScaleMatrix(double scale) {
    matrix_t A = s21_create_matrix(4, 4);
    A.matrix[0][0] = scale;
    A.matrix[1][1] = scale;
    A.matrix[2][2] = scale;
    A.matrix[3][3] = 1.0;
    return A;
}
