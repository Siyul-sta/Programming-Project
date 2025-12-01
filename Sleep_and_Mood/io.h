#ifndef IO_H
#define IO_H
#include "core.h"

int save_csv(const Log* arr, int n, const char* path);
int load_csv(Log** arr, int* n, const char* path);

#endif
