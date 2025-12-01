#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "core.h"

// 날짜 비교용
static int cmp_date(const void* a, const void* b) {
    const Log* x = a;
    const Log* y = b;
    return strcmp(x->date, y->date);
}

// 날짜로 기록 찾기
int find_index_by_date(const Log* arr, int n, const char* date) {
    for (int i = 0; i < n; i++)
        if (strcmp(arr[i].date, date) == 0)
            return i;
    return -1;
}

// 추가(같은 날짜면 덮어쓰기)
int add_log(Log** arr, int* n, Log item) {
    int idx = find_index_by_date(*arr, *n, item.date);
    if (idx >= 0) { (*arr)[idx] = item; return 0; }

    Log* tmp = realloc(*arr, sizeof(Log) * (*n + 1));
    if (!tmp) return -1;

    *arr = tmp;
    (*arr)[*n] = item;
    (*n)++;
    return 0;
}

// 정렬
void sort_by_date(Log* arr, int n) {
    if (n > 1) qsort(arr, n, sizeof(Log), cmp_date);
}

// 수정
int edit_log(Log* arr, int n, const char* date, Log newData) {
    int idx = find_index_by_date(arr, n, date);
    if (idx < 0) return -1;
    arr[idx] = newData;
    return 0;
}

// 삭제
int delete_log(Log* arr, int* n, const char* date) {
    int idx = find_index_by_date(arr, *n, date);
    if (idx < 0) return -1;
    for (int i = idx; i < *n - 1; i++)
        arr[i] = arr[i + 1];
    (*n)--;
    return 0;
}

// 전체 초기화
void reset_all(Log** arr, int* n) {
    free(*arr);
    *arr = NULL;
    *n = 0;
}
