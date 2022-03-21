#ifndef SRC_VIEWER_H_
#define SRC_VIEWER_H_

#include <gtk/gtk.h>

#include "reader.h"
#include "render.h"
#include "settings.h"

#define insideOfScreen(X, WIDTH, HEIGHT) ()

typedef enum { TRANSLATE = 1, ROTATE, SCALE } transform_type_t;

void viewModel(const char *filename);
void viewTransformModel(double *data, transform_type_t type);

void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data);
void freeData();

void errorMessage(int err_code);

#endif  // SRC_VIEWER_H_
