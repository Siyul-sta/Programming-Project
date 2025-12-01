#ifndef REPORT_H
#define REPORT_H

#include "core.h"
void plot_histogram(const Log* arr, int n);
void print_daily(const Log* arr, int n, const char* date);
void report_week(const Log* arr, int n);
void report_month(const Log* arr, int n, int year, int month);
void report_corr(const Log* arr, int n);
void plot_bars(const Log* arr, int n);
void warn_irregular(const Log* arr, int n);
void recommend_routine(const Log* arr, int n);

#endif
