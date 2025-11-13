#pragma once
#ifndef CORE_H
#define CORE_H

typedef struct {
    char date[11];      // "YYYY-MM-DD"
    int  sleep_h, sleep_m;
    int  wake_h, wake_m;
    double hours;        // 총 수면시간(시간 단위)
    int    mood;         // 감정 점수 (1~5)
} Log;

//날짜로 인덱스  
int find_index_by_date(const Log* arr, int n, const char* date);
// 로그 추가하기
int add_log(Log** arr, int* n, Log item);
//날짜 정렬
void sort_by_date(Log* arr, int n);

#endif
