#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

typedef enum { WHITE, CYAN, GOLDEN } el_color_t;
typedef enum { SOLID, DASHED } line_t;
typedef enum { NONE, CIRCLE, SQUARE } v_shape_t;
typedef enum { PARALLEL, CENTRAL } projection_t;
typedef enum { DARK_GREY, LIGHT_GREY, BLUE } bg_color_t;

typedef struct {
    el_color_t e_color;
    int e_thickness;
    line_t e_type;
    el_color_t v_color;
    int v_size;
    v_shape_t v_shape;
    projection_t projection;
    bg_color_t bg_color;
} settings_t;

//    Function
void init_settings(char* filename);
void save_preferences_in_file(char* filename);
#endif  // SRC_SETTINGS_H_
