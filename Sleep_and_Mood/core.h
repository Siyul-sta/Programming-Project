#ifndef CORE_H
#define CORE_H

typedef struct {
    char date[11];     // YYYY-MM-DD
    int  sleep_h, sleep_m;
    int  wake_h, wake_m;
    double hours;      // 총 수면시간
    int mood;          // 기분(1~5)
} Log;

int find_index_by_date(const Log* arr, int n, const char* date);
int add_log(Log** arr, int* n, Log item);
int edit_log(Log* arr, int n, const char* date, Log item);
int delete_log(Log* arr, int* n, const char* date);

void sort_by_date(Log* arr, int n);
void reset_all(Log** arr, int* n);

#endif
