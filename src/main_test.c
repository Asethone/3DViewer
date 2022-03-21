#include <check.h>
#include <stdio.h>

#include "reader.h"
#include "render.h"

#define destroy(data, size)          \
    for (int i = 0; i < size; i++) { \
        free(data[i]);  \
    }                                \
    free(data);

bool isEqualObjData(obj_data_t *first, obj_data_t *second) {
    bool isEqual = true;

    if (first->e_cnt != second->e_cnt || first->v_cnt != second->v_cnt) isEqual = false;

    for (int i = 0; i < first->v_cnt && isEqual; i++) {
        for (int j = 0; j < 3; j++) {
            if (fabs(first->vertices[i][j] - second->vertices[i][j]) > 1e-7) {
                isEqual = false;
                break;
            }
        }
    }

    for (int i = 0; i < first->e_cnt && isEqual; i++) {
        for (int j = 0; j < 4; j++) {
            if (fabs(first->edges[i][j] - second->edges[i][j]) > 1e-7) {
                isEqual = false;
                break;
            }
        }
    }

    return isEqual;
}

bool isEqualRenderData(render_data_t *first, render_data_t *second) {
    bool isEqual = true;

    if (first->v_cnt != second->v_cnt) isEqual = false;

    for (int i = 0; i < first->v_cnt && isEqual; i++) {
        for (int j = 0; j < 3; j++) {
            if (fabs(first->vertices[i][j] - second->vertices[i][j]) > 1e-7) {
                isEqual = false;
                break;
            }
        }
    }

    return isEqual;
}

START_TEST(test_reader_error_1) {
    // arrange
    int expected_result = 1;

    // act
    int actual_result = readFile("models/1234.obj", NULL);

    // assert
    ck_assert_int_eq(actual_result, expected_result);
}
END_TEST

START_TEST(test_reader) {
    // arrange
    double **actual_verts = (double **)malloc(sizeof(double *));

    int **actual_edges = (int **)malloc(sizeof(int *));

    obj_data_t actual_result = {actual_verts, 0, actual_edges, 0};

    int expected_v_cnt = 8;
    int expected_e_cnt = 4;

    double **expected_verts = (double **)malloc(8 * sizeof(double *));
    for (size_t i = 0; i < 8; i++) {
        expected_verts[i] = (double *)malloc(3 * sizeof(double));
    }

    int **expected_edges = (int **)malloc(4 * sizeof(int *));
    for (size_t i = 0; i < 4; i++) {
        expected_edges[i] = (int *)malloc(4 * sizeof(int));
    }

    expected_verts[0][0] = 0;
    expected_verts[0][1] = 0;
    expected_verts[0][2] = 0;

    expected_verts[1][0] = 0;
    expected_verts[1][1] = 0;
    expected_verts[1][2] = 1;

    expected_verts[2][0] = 0;
    expected_verts[2][1] = 1;
    expected_verts[2][2] = 0;

    expected_verts[3][0] = 0;
    expected_verts[3][1] = 1;
    expected_verts[3][2] = 1;

    expected_verts[4][0] = 1;
    expected_verts[4][1] = 0;
    expected_verts[4][2] = 0;

    expected_verts[5][0] = 1;
    expected_verts[5][1] = 0;
    expected_verts[5][2] = 1;

    expected_verts[6][0] = 1;
    expected_verts[6][1] = 1;
    expected_verts[6][2] = 0;

    expected_verts[7][0] = 1;
    expected_verts[7][1] = 1;
    expected_verts[7][2] = 1;

    expected_edges[0][0] = 1;
    expected_edges[0][1] = 2;
    expected_edges[0][2] = 4;
    expected_edges[0][3] = 3;

    expected_edges[1][0] = 1;
    expected_edges[1][1] = 3;
    expected_edges[1][2] = 7;
    expected_edges[1][3] = 5;

    expected_edges[2][0] = 7;
    expected_edges[2][1] = 5;
    expected_edges[2][2] = 6;
    expected_edges[2][3] = 8;

    expected_edges[3][0] = 6;
    expected_edges[3][1] = 2;
    expected_edges[3][2] = 4;
    expected_edges[3][3] = 8;

    obj_data_t expected_result = {expected_verts, expected_v_cnt, expected_edges, expected_e_cnt};

    // act
    readFile("models/cube.obj", &actual_result);

    // assert
    ck_assert_int_eq(isEqualObjData(&actual_result, &expected_result), true);

    // cleanup
    destroy(expected_result.vertices, 8);
    destroy(expected_result.edges, 4);
    destroy(actual_result.vertices, 8);
    destroy(actual_result.edges, 4);
}
END_TEST

START_TEST(test_obj_translate) {
    // arrange
    double **actual_verts = (double **)malloc(sizeof(double *));
    *actual_verts = (double *)malloc(3 * sizeof(double));

    double **expected_verts = (double **)malloc(sizeof(double *));
    *expected_verts = (double *)malloc(4 * sizeof(double));

    actual_verts[0][0] = 5;
    actual_verts[0][1] = 3;
    actual_verts[0][2] = 10;

    expected_verts[0][0] = 6.5;
    expected_verts[0][1] = 5;
    expected_verts[0][2] = 10.5;
    expected_verts[0][3] = 1;

    obj_data_t actual_result = {actual_verts, 1, NULL, 0};
    obj_data_t expected_result = {expected_verts, 1, NULL, 0};

    double trans_data[3] = {1.5, 2, 0.5};

    // act
    translateModel(&actual_result, trans_data);

    // assert
    ck_assert_int_eq(isEqualObjData(&actual_result, &expected_result), true);

    // cleanup
    destroy(expected_result.vertices, 1);
    destroy(actual_result.vertices, 1);
}
END_TEST

START_TEST(test_obj_rotateX) {
    // arrange
    double **actual_verts = (double **)malloc(sizeof(double *));
    *actual_verts = (double *)malloc(3 * sizeof(double));

    double **expected_verts = (double **)malloc(sizeof(double *));
    *expected_verts = (double *)malloc(4 * sizeof(double));

    actual_verts[0][0] = 5;
    actual_verts[0][1] = 3;
    actual_verts[0][2] = 10;

    expected_verts[0][0] = 5;
    expected_verts[0][1] = -2.401923788646683;
    expected_verts[0][2] = 10.16025403784439;
    expected_verts[0][3] = 1;

    obj_data_t actual_result = {actual_verts, 1, NULL, 0};
    obj_data_t expected_result = {expected_verts, 1, NULL, 0};

    // act
    rotateXModel(&actual_result, 30.0);

    // assert
    ck_assert_int_eq(isEqualObjData(&actual_result, &expected_result), true);

    // cleanup
    destroy(expected_result.vertices, 1);
    destroy(actual_result.vertices, 1);
}
END_TEST

START_TEST(test_obj_rotateY) {
    // arrange
    double **actual_verts = (double **)malloc(sizeof(double *));
    *actual_verts = (double *)malloc(3 * sizeof(double));

    double **expected_verts = (double **)malloc(sizeof(double *));
    *expected_verts = (double *)malloc(4 * sizeof(double));

    actual_verts[0][0] = 5;
    actual_verts[0][1] = 3;
    actual_verts[0][2] = 10;

    expected_verts[0][0] = 9.330127018922195;
    expected_verts[0][1] = 3;
    expected_verts[0][2] = 6.16025403784439;
    expected_verts[0][3] = 1;

    obj_data_t actual_result = {actual_verts, 1, NULL, 0};
    obj_data_t expected_result = {expected_verts, 1, NULL, 0};

    // act
    rotateYModel(&actual_result, 30.0);

    // assert
    ck_assert_int_eq(isEqualObjData(&actual_result, &expected_result), true);

    // cleanup
    destroy(expected_result.vertices, 1);
    destroy(actual_result.vertices, 1);
}
END_TEST

START_TEST(test_obj_rotateZ) {
    // arrange
    double **actual_verts = (double **)malloc(sizeof(double *));
    *actual_verts = (double *)malloc(3 * sizeof(double));

    double **expected_verts = (double **)malloc(sizeof(double *));
    *expected_verts = (double *)malloc(4 * sizeof(double));

    actual_verts[0][0] = 5;
    actual_verts[0][1] = 3;
    actual_verts[0][2] = 10;

    expected_verts[0][0] = 2.830127018922195;
    expected_verts[0][1] = 5.098076211353317;
    expected_verts[0][2] = 10;
    expected_verts[0][3] = 1;

    obj_data_t actual_result = {actual_verts, 1, NULL, 0};
    obj_data_t expected_result = {expected_verts, 1, NULL, 0};

    // act
    rotateZModel(&actual_result, 30.0);

    // assert
    ck_assert_int_eq(isEqualObjData(&actual_result, &expected_result), true);

    // cleanup
    destroy(expected_result.vertices, 1);
    destroy(actual_result.vertices, 1);
}
END_TEST

START_TEST(test_obj_scale) {
    // arrange
    double **actual_verts = (double **)malloc(sizeof(double *));
    *actual_verts = (double *)malloc(3 * sizeof(double));

    double **expected_verts = (double **)malloc(sizeof(double *));
    *expected_verts = (double *)malloc(4 * sizeof(double));

    actual_verts[0][0] = 5;
    actual_verts[0][1] = 3;
    actual_verts[0][2] = 10;

    expected_verts[0][0] = 7.5;
    expected_verts[0][1] = 4.5;
    expected_verts[0][2] = 15;
    expected_verts[0][3] = 1;

    obj_data_t actual_result = {actual_verts, 1, NULL, 0};
    obj_data_t expected_result = {expected_verts, 1, NULL, 0};

    // act
    scaleModel(&actual_result, 1.5);

    // assert
    ck_assert_int_eq(isEqualObjData(&actual_result, &expected_result), true);

    // cleanup
    destroy(expected_result.vertices, 1);
    destroy(actual_result.vertices, 1);
}
END_TEST

START_TEST(test_render_model) {
    // arrange
    double **actual_verts = (double **)malloc(sizeof(double *));
    *actual_verts = (double *)malloc(3 * sizeof(double));

    actual_verts[0][0] = 5;
    actual_verts[0][1] = 3;
    actual_verts[0][2] = 10;

    obj_data_t obj_data = {actual_verts, 1, NULL, 0};

    render_data_t *actual_render_data = newRenderData(&obj_data);
    render_data_t *expected_render_data = newRenderData(&obj_data);

    expected_render_data->vertices[0][0] = -3.53553390593274;
    expected_render_data->vertices[0][1] = -3.674234614174767;
    expected_render_data->vertices[0][2] = 0;
    expected_render_data->vertices[0][3] = 1;

    // act
    renderModel(actual_render_data);

    // assert
    ck_assert_int_eq(isEqualRenderData(actual_render_data, expected_render_data), true);

    // cleanup
    destroy(expected_render_data->vertices, 1);
    destroy(actual_render_data->vertices, 1);
    destroy(actual_verts, 1);
    g_free(expected_render_data);
    g_free(actual_render_data);
}
END_TEST

int main(void) {
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    srunner_set_fork_status(sr, CK_NOFORK);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, test_reader);
    tcase_add_test(tc1_1, test_reader_error_1);
    tcase_add_test(tc1_1, test_obj_translate);
    tcase_add_test(tc1_1, test_obj_rotateX);
    tcase_add_test(tc1_1, test_obj_rotateY);
    tcase_add_test(tc1_1, test_obj_rotateZ);
    tcase_add_test(tc1_1, test_obj_scale);
    tcase_add_test(tc1_1, test_render_model);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
