#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "io.h"

// 저장
int save_csv(const Log* arr, int n, const char* path) {
    FILE* fp = fopen(path, "w");
    if (!fp) return -1;

    fprintf(fp, "date,sleep_h,sleep_m,wake_h,wake_m,hours,mood\n");
    for (int i = 0; i < n; i++)
        fprintf(fp, "%s,%d,%d,%d,%d,%.2f,%d\n",
            arr[i].date,
            arr[i].sleep_h, arr[i].sleep_m,
            arr[i].wake_h, arr[i].wake_m,
            arr[i].hours,
            arr[i].mood);

    fclose(fp);
    return 0;
}

// 불러오기
int load_csv(Log** arr, int* n, const char* path) {
    FILE* fp = fopen(path, "r");
    if (!fp) return -1;

    char line[256];
    *arr = NULL; *n = 0;

    fgets(line, 256, fp); 

    while (fgets(line, 256, fp)) {
        Log x;
        if (sscanf(line, "%10[^,],%d,%d,%d,%d,%lf,%d",
            x.date,
            &x.sleep_h, &x.sleep_m,
            &x.wake_h, &x.wake_m,
            &x.hours, &x.mood) == 7)
        {
            Log* tmp = realloc(*arr, sizeof(Log) * (*n + 1));
            if (!tmp) { fclose(fp); return -1; }
            *arr = tmp;
            (*arr)[*n] = x;
            (*n)++;
        }
    }

    fclose(fp);
    return 0;
}
