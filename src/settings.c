#include "settings.h"

extern settings_t options;

void init_settings(char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp != NULL) {
        char* str_file = NULL;
        size_t size = 255;
        // getline(&str_file, &size, fp);
        while (getline(&str_file, &size, fp) > 0) {
            if (strncmp(str_file, "EDGES_COLOR=", 12) == 0) {
                if (strcmp(str_file + strlen("EDGES_COLOR="), "white\n") == 0) {
                    options.e_color = WHITE;
                } else if (strcmp(str_file + strlen("EDGES_COLOR="), "cyan\n") == 0) {
                    options.e_color = CYAN;
                } else if (strcmp(str_file + strlen("EDGES_COLOR="), "golden\n") == 0) {
                    options.e_color = GOLDEN;
                } else {
                    printf("Error1");
                }
            } else if (strcmp(str_file, "EDGES_THICKNESS=") == str_file[strlen("EDGES_THICKNESS=")]) {
                int thicknes = atoi(str_file + strlen("EDGES_THICKNESS="));
                if (thicknes > 0 && thicknes < 6) {
                    options.e_thickness = thicknes;
                } else {
                    printf("Error2");
                }
            } else if (strcmp(str_file, "EDGES_TYPE=") == str_file[strlen("EDGES_TYPE=")]) {
                if (strcmp(str_file + strlen("EDGES_TYPE="), "solid\n") == 0) {
                    options.e_type = SOLID;
                } else if (strcmp(str_file + strlen("EDGES_TYPE="), "dashed\n") == 0) {
                    options.e_type = DASHED;
                } else {
                    printf("Error3");
                }
            } else if (strcmp(str_file, "VERTICES_COLOR=") == str_file[strlen("VERTICES_COLOR=")]) {
                if (strcmp(str_file + strlen("VERTICES_COLOR="), "white\n") == 0) {
                    options.v_color = WHITE;
                } else if (strcmp(str_file + strlen("VERTICES_COLOR="), "cyan\n") == 0) {
                    options.v_color = CYAN;
                } else if (strcmp(str_file + strlen("VERTICES_COLOR="), "golden\n") == 0) {
                    options.v_color = GOLDEN;
                } else {
                    printf("Error4");
                }
            } else if (strcmp(str_file, "VERTICES_SIZE=") == str_file[strlen("VERTICES_SIZE=")]) {
                int v_size = atoi(str_file + strlen("VERTICES_SIZE="));
                if (v_size > 0 && v_size < 6) {
                    options.v_size = v_size;
                } else {
                    printf("Error5");
                }
            } else if (strcmp(str_file, "VERTICES_SHAPE=") == str_file[strlen("VERTICES_SHAPE=")]) {
                if (strcmp(str_file + strlen("VERTICES_SHAPE="), "none\n") == 0) {
                    options.v_shape = NONE;
                } else if (strcmp(str_file + strlen("VERTICES_SHAPE="), "square\n") == 0) {
                    options.v_shape = SQUARE;
                } else if (strcmp(str_file + strlen("VERTICES_SHAPE="), "circle\n") == 0) {
                    options.v_shape = CIRCLE;
                } else {
                    printf("Error6");
                }
            } else if (strcmp(str_file, "PROJECTION_TYPE=") == str_file[strlen("PROJECTION_TYPE=")]) {
                if (strcmp(str_file + strlen("PROJECTION_TYPE="), "parallel\n") == 0) {
                    options.projection = PARALLEL;
                } else if (strcmp(str_file + strlen("PROJECTION_TYPE="), "central\n") == 0) {
                    options.projection = CENTRAL;
                } else {
                    printf("Error7");
                }
            } else if (strcmp(str_file, "BACKGROUND_COLOR=") == str_file[strlen("BACKGROUND_COLOR=")]) {
                if (strcmp(str_file + strlen("BACKGROUND_COLOR="), "dark_grey\n") == 0) {
                    options.bg_color = DARK_GREY;
                } else if (strcmp(str_file + strlen("BACKGROUND_COLOR="), "light_grey\n") == 0) {
                    options.bg_color = LIGHT_GREY;
                } else if (strcmp(str_file + strlen("BACKGROUND_COLOR="), "blue\n") == 0) {
                    options.bg_color = BLUE;
                } else {
                    printf("Error8");
                }
            } else {
                printf("Error9");
            }
        }
        if (str_file != NULL) free(str_file);
        // printf("%d|%d",options.e_color,options.e_thickness);
        fclose(fp);
    } else {
        printf("Not open");
    }
}

void save_preferences_in_file(char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp != NULL) {
        fprintf(fp, "EDGES_COLOR=%s\n",
                options.e_color == WHITE    ? "white"
                : options.e_color == CYAN   ? "cyan"
                : options.e_color == GOLDEN ? "golden"
                                            : NULL);
        fprintf(fp, "EDGES_THICKNESS=%d\n", options.e_thickness);
        fprintf(fp, "EDGES_TYPE=%s\n",
                options.e_type == SOLID    ? "solid"
                : options.e_type == DASHED ? "dashed"
                                           : NULL);
        fprintf(fp, "VERTICES_COLOR=%s\n",
                options.v_color == WHITE    ? "white"
                : options.v_color == CYAN   ? "cyan"
                : options.v_color == GOLDEN ? "golden"
                                            : NULL);
        fprintf(fp, "VERTICES_SIZE=%d\n", options.v_size);
        fprintf(fp, "VERTICES_SHAPE=%s\n",
                options.v_shape == NONE     ? "none"
                : options.v_shape == SQUARE ? "square"
                : options.v_shape == CIRCLE ? "circle"
                                            : NULL);
        fprintf(fp, "PROJECTION_TYPE=%s\n",
                options.projection == CENTRAL    ? "central"
                : options.projection == PARALLEL ? "parallel"
                                                 : NULL);
        fprintf(fp, "BACKGROUND_COLOR=%s\n",
                options.bg_color == DARK_GREY    ? "dark_grey"
                : options.bg_color == LIGHT_GREY ? "light_grey"
                : options.bg_color == BLUE       ? "blue"
                                                : NULL);
        fclose(fp);
    }
}
