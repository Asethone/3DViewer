#ifndef SRC_READER_H_
#define SRC_READER_H_

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

int readFile(const char *filename, obj_data_t *data);
int fillData(FILE *fp, obj_data_t *data);

#endif  // SRC_READER_H_
