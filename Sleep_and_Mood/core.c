#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"

static int cmp_date(const void* a, const void* b)
{
    const Log* x = (const Log*)a;
    const Log* y = (const Log*)b;
    return strcmp(x->date, y->date);   // YYYY-MM-DD 문자열 비교 = 날짜 비교
}

int find_index_by_date(const Log* arr, int n, const char* date)
{
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].date, date) == 0) {
            return i;   // 날짜가 같은 인덱스 반환
        }
    }
    return -1;          // 없다면 -1
}

int add_log(Log** arr, int* n, Log item)
{
    // 이미 동일한 날짜가 있는지 검사
    int idx = find_index_by_date(*arr, *n, item.date);
    if (idx >= 0) {
        // 덮어쓰기 (동일 날짜 존재)
        (*arr)[idx] = item;
        return 0;
    }

    // 신규 추가 → 공간 확장
    Log* tmp = (Log*)realloc(*arr, sizeof(Log) * (*n + 1));

    if (!tmp) {
        return -1;   // 메모리 확장 실패
    }

    *arr = tmp;               // 실제 배열 포인터 갱신
    (*arr)[*n] = item;        // 새로운 요소 추가
    (*n)++;                   // 개수 증가

    return 0;                 // 성공
}

void sort_by_date(Log* arr, int n)
{
    if (n <= 1) {
        return;   // 정렬할 필요 없음
    }

    // qsort를 이용한 날짜 오름차순 정렬
    qsort(arr, n, sizeof(Log), cmp_date);

    // 정렬 완료 후 확인용(필요시)
    // printf("[정렬 완료]\n");
}
