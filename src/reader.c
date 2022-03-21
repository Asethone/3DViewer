#include "reader.h"

// Open and read file. If file doesn't exits or has wrong data format, returns error code.
int readFile(const char *filename, obj_data_t *data) {
    int retVal = 0;
    char full_name[255] = "";
    snprintf(full_name, sizeof(full_name), USER_APP "%s", filename);
    FILE *fp = fopen(full_name, "rt");
    if (fp) {
        retVal = fillData(fp, data);
        fclose(fp);
    } else {
        retVal = NO_SUCH_FILE;
    }
    return retVal;
}

// Fill data structure. If file has wrong data format, returns error code.
int fillData(FILE *fp, obj_data_t *data) {
    int retVal = 0;
    char *line = (char *)malloc(255);
    while (fgets(line, 255, fp)) {
        // Vertices parsing
        if (*line == 'v' && *(line + 1) == ' ') {
            (data->v_cnt)++;
            data->vertices = (double **)realloc(data->vertices, sizeof(double **) * data->v_cnt);
            data->vertices[data->v_cnt - 1] = (double *)malloc(sizeof(double) * 3);
            char *lp = line + 2;
            data->vertices[data->v_cnt - 1][0] = strtod(lp, &lp);
            if ((*lp) != ' ') {
                retVal = W_OBJ_DATA;
                break;
            }
            data->vertices[data->v_cnt - 1][1] = strtod(lp, &lp);
            if ((*lp) != ' ') {
                retVal = W_OBJ_DATA;
                break;
            }
            data->vertices[data->v_cnt - 1][2] = strtod(lp, &lp);
            // Edges parsing
        } else if (*line == 'f' && *(line + 1) == ' ') {
            (data->e_cnt)++;
            data->edges = (int **)realloc(data->edges, sizeof(int **) * data->e_cnt);
            data->edges[data->e_cnt - 1] = (int *)malloc(sizeof(int));
            char *lp = line + 2;
            int i = 0;
            do {
                i++;
                data->edges[data->e_cnt - 1] = (int *)realloc(data->edges[data->e_cnt - 1], sizeof(int) * i);
                int f = (int)strtol(lp, &lp, 10);
                data->edges[data->e_cnt - 1][i - 1] = f;
            } while (*lp != '\n' && *lp != EOF && *lp != 0);
            // Zero-terminate
            i++;
            data->edges[data->e_cnt - 1] = (int *)realloc(data->edges[data->e_cnt - 1], sizeof(int) * i);
            data->edges[data->e_cnt - 1][i - 1] = 0;
        }
    }
    free(line);

    return retVal;
}
