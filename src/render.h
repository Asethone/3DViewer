#ifndef SRC_RENDER_H_
#define SRC_RENDER_H_

#include <gtk/gtk.h>

#include "matrix.h"
#include "types.h"
#include "settings.h"

typedef struct {
    double **vertices;
    int v_cnt;
    obj_data_t *obj_data;
} render_data_t;

render_data_t *newRenderData(obj_data_t *obj_data);
void renderModel(render_data_t *render_data);
void translateModel(obj_data_t *obj_data, double *trans_data);
void rotateXModel(obj_data_t *obj_data, double angle);
void rotateYModel(obj_data_t *obj_data, double angle);
void rotateZModel(obj_data_t *obj_data, double angle);
void scaleModel(obj_data_t *obj_data, double scale);

#endif  // SRC_RENDER_H_
