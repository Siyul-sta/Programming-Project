#ifndef CALC_H
#define CALC_H

double calc_hours(int sh, int sm, int wh, int wm);
double mean(const double* x, int n);
double stdev(const double* x, int n);
double pearson_r(const double* x, const double* y, int n);

#endif
