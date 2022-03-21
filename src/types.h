#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

typedef struct {
    double **vertices;
    int v_cnt;
    int **edges;
    int e_cnt;
} obj_data_t;

enum { NO_SUCH_FILE = 1, W_OBJ_DATA, W_TRANS_DATA, W_ROT_DATA, W_SCALE_DATA, NO_MODEL };

#define ERR_LIST                                                                          \
    {                                                                                     \
        "No such OBJ file!", "Wrong data format in OBJ file!", "Wrong translation data!", \
            "Wrong rotation data!", "Wrong scale data!", "Choose model first!"            \
    }

#ifndef USER_APP
#define USER_APP "./"
#endif

#endif  // SRC_TYPES_H_
