#include <math.h>
#include "calc.h"
#include <windows.h>
// 총 수면시간 계산
double calc_hours(int sh, int sm, int wh, int wm) {
    int start = sh * 60 + sm;
    int end = wh * 60 + wm;
    if (end <= start) end += 24 * 60; // 자정 넘어가는 경우
    return (end - start) / 60.0;
}

double mean(const double* x, int n) {
    if (n <= 0) return 0;
    double s = 0; for (int i = 0; i < n; i++) s += x[i];
    return s / n;
}

double stdev(const double* x, int n) {
    if (n <= 1) return 0;
    double m = mean(x, n), ss = 0;
    for (int i = 0; i < n; i++) {
        double d = x[i] - m;
        ss += d * d;
    }
    return sqrt(ss / (n - 1));
}

double pearson_r(const double* x, const double* y, int n) {
    if (n <= 1) return 0;

    double mx = mean(x, n), my = mean(y, n);
    double num = 0, sx = 0, sy = 0;

    for (int i = 0; i < n; i++) {
        double dx = x[i] - mx;
        double dy = y[i] - my;
        num += dx * dy;
        sx += dx * dx;
        sy += dy * dy;
    }
    double denom = sqrt(sx * sy);
    if (denom == 0) return 0;

    return num / denom;
}
