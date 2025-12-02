#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "report.h"
#include "calc.h"

// 일간 조회
void print_daily(const Log* arr, int n, const char* date) {
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].date, date) == 0) {
            printf("[%s]\n", arr[i].date);
            printf("취침: %02d:%02d\n", arr[i].sleep_h, arr[i].sleep_m);
            printf("기상: %02d:%02d\n", arr[i].wake_h, arr[i].wake_m);
            printf("수면시간: %.2f시간\n", arr[i].hours);
            printf("기분: %d\n", arr[i].mood);
            return;
        }
    }
    printf("해당 날짜 기록 없음.\n");
}

// 최근 7일 분석
void report_week(const Log* arr, int n) {
    if (n == 0) { printf("데이터 없음\n"); return; }

    double h[7];
    int m[7];
    int cnt = 0;

    // 뒤에서부터 최대 7개, 그 중에서 hours > 0 인 것만 사용
    for (int i = n - 1; i >= 0 && cnt < 7; i--) {
        if (arr[i].hours > 0.0) {          // ✅ 미완성 기록(0.0) 건너뛰기
            h[cnt] = arr[i].hours;
            m[cnt] = arr[i].mood;
            cnt++;
        }
    }

    if (cnt == 0) {
        printf("최근 7일 안에 완성된 기록이 없습니다.\n");
        return;
    }

    double avg_h = mean(h, cnt);
    double sum_m = 0;
    for (int i = 0; i < cnt; i++) sum_m += m[i];
    double avg_m = sum_m / cnt;

    double maxh = h[0], minh = h[0];
    for (int i = 1; i < cnt; i++) {
        if (h[i] > maxh) maxh = h[i];
        if (h[i] < minh) minh = h[i];
    }

    printf("=== 최근 %d일(완성된 기록 기준) ===\n", cnt);
    printf("평균 수면: %.2f시간\n", avg_h);
    printf("평균 기분: %.2f\n", avg_m);
    printf("최대 수면: %.2f시간\n", maxh);
    printf("최소 수면: %.2f시간\n", minh);
}


// 월간 분석
void report_month(const Log* arr, int n, int year, int month) {
    char target[8];
    sprintf(target, "%04d-%02d", year, month);

    double h[31];
    int cnt = 0;

    for (int i = 0; i < n; i++) {
        if (strncmp(arr[i].date, target, 7) == 0) {
            h[cnt] = arr[i].hours;
            cnt++;
        }
    }

    if (cnt == 0) { printf("그 달 기록 없음\n"); return; }

    double avg = mean(h, cnt);
    double maxh = h[0], minh = h[0];

    for (int i = 1; i < cnt; i++) {
        if (h[i] > maxh) maxh = h[i];
        if (h[i] < minh) minh = h[i];
    }

    printf("=== %s 월간 ===\n", target);
    printf("기록: %d일\n", cnt);
    printf("평균 수면시간: %.2f\n", avg);
    printf("최대: %.2f\n", maxh);
    printf("최소: %.2f\n", minh);
}
// 상관 분석 + 수면 구간별 감정 비교
void report_corr(const Log* arr, int n) {
    if (n < 3) {
        printf("데이터 부족\n");
        return;
    }

    double h[1000], mm[1000];
    int cnt = (n > 1000 ? 1000 : n);

    for (int i = 0; i < cnt; i++) {
        h[i] = arr[i].hours;
        mm[i] = (double)arr[i].mood;
    }

    double r = pearson_r(h, mm, cnt);

    printf("=== 수면시간-기분 상관 분석 ===\n");
    printf("표본 개수: %d\n", cnt);
    printf("상관계수 r = %.3f\n", r);
    printf("(r이 1에 가까울수록 '많이 잘수록 기분↑', -1에 가까울수록 '많이 잘수록 기분↓')\n");

    // r 값 해석
    if (r > 0.7)      printf("→ 강한 양의 상관: 수면시간이 늘수록 기분이 좋아지는 경향이 큽니다.\n");
    else if (r > 0.3) printf("→ 약한 양의 상관: 대체로 많이 잘수록 기분이 좋아지는 편입니다.\n");
    else if (r > -0.3)printf("→ 거의 상관 없음: 수면시간과 기분이 크게 연결되어 보이진 않습니다.\n");
    else if (r > -0.7)printf("→ 약한 음의 상관: 많이 잘수록 오히려 기분이 살짝 나빠지는 패턴이 있습니다.\n");
    else              printf("→ 강한 음의 상관: 수면시간이 많을수록 기분이 나빠지는 경향이 큽니다.\n");

    // ---- 여기서부터는 "실제 구간별로 어떻게 달랐는지" 보여주는 부분 ----

    int short_cnt = 0, good_cnt = 0;
    double short_mood_sum = 0.0, good_mood_sum = 0.0;

    for (int i = 0; i < cnt; i++) {
        if (h[i] < 7.0) {           // 7시간 미만
            short_cnt++;
            short_mood_sum += mm[i];
        }
        else {                    // 7시간 이상
            good_cnt++;
            good_mood_sum += mm[i];
        }
    }

    printf("\n=== 수면시간 구간별 평균 기분 ===\n");
    if (short_cnt > 0) {
        printf("7시간 미만인 날(%d일)의 평균 기분: %.2f점\n",
            short_cnt, short_mood_sum / short_cnt);
    }
    else {
        printf("7시간 미만으로 잔 날이 없습니다.\n");
    }

    if (good_cnt > 0) {
        printf("7시간 이상 잔 날(%d일)의 평균 기분: %.2f점\n",
            good_cnt, good_mood_sum / good_cnt);
    }
    else {
        printf("7시간 이상 잔 날이 없습니다.\n");
    }

    // 한 줄 요약
    if (short_cnt > 0 && good_cnt > 0) {
        double avg_short = short_mood_sum / short_cnt;
        double avg_good = good_mood_sum / good_cnt;

        printf("\n=== 한 줄 요약 ===\n");
        if (avg_good > avg_short + 0.3) {
            printf("→ 전반적으로 7시간 이상 잤을 때 기분이 더 좋은 편입니다.\n");
        }
        else if (avg_short > avg_good + 0.3) {
            printf("→ 오히려 짧게 잔 날의 기분 점수가 더 높게 나왔습니다.\n");
        }
        else {
            printf("→ 7시간 기준으로 나눠 봐도 기분 차이가 크게 나지는 않습니다.\n");
        }
    }

    printf("\n");
}

// 텍스트 그래프
void plot_bars(const Log* arr, int n) {
    if (n == 0) { printf("데이터 없음\n"); return; }

    double maxh = arr[0].hours;
    for (int i = 1; i < n; i++)
        if (arr[i].hours > maxh) maxh = arr[i].hours;

    if (maxh <= 0) maxh = 1;

    printf("=== 텍스트 그래프 ===\n");
    for (int i = 0; i < n; i++) {
        int len = (int)(arr[i].hours / maxh * 30 + 0.5);
        printf("%s | ", arr[i].date);
        for (int k = 0; k < len; k++) putchar('*');
        printf(" (%.2f)\n", arr[i].hours);
    }
}

// 수면시간 분포 히스토그램
void plot_histogram(const Log* arr, int n)
{
    if (n == 0) {
        printf("기록이 없습니다.\n");
        return;
    }

    // 구간: 0~2, 2~4, 4~6, 6~8, 8~10, 10시간 이상
    int bins[6] = { 0 };

    for (int i = 0; i < n; i++) {
        double h = arr[i].hours;

        if (h < 2) bins[0]++;
        else if (h < 4) bins[1]++;
        else if (h < 6) bins[2]++;
        else if (h < 8) bins[3]++;
        else if (h < 10) bins[4]++;
        else bins[5]++;
    }

    printf("=== 수면시간 히스토그램 ===\n");
    printf(" 0~2h   | "); for (int i = 0; i < bins[0]; i++) putchar('*'); printf(" (%d)\n", bins[0]);
    printf(" 2~4h   | "); for (int i = 0; i < bins[1]; i++) putchar('*'); printf(" (%d)\n", bins[1]);
    printf(" 4~6h   | "); for (int i = 0; i < bins[2]; i++) putchar('*'); printf(" (%d)\n", bins[2]);
    printf(" 6~8h   | "); for (int i = 0; i < bins[3]; i++) putchar('*'); printf(" (%d)\n", bins[3]);
    printf(" 8~10h  | "); for (int i = 0; i < bins[4]; i++) putchar('*'); printf(" (%d)\n", bins[4]);
    printf("10h+    | "); for (int i = 0; i < bins[5]; i++) putchar('*'); printf(" (%d)\n", bins[5]);
}


// 수면 부족 경고
void warn_irregular(const Log* arr, int n) {
    if (n < 3) { printf("데이터 부족\n"); return; }

    int streak = 0;
    for (int i = n - 1; i >= 0; i--) {
        if (arr[i].hours <= 5) streak++;
        else streak = 0;

        if (streak >= 3) {
            printf("⚠ 최근 %d일 연속 5시간 이하 수면!\n", streak);
            return;
        }
    }
    printf("경고 없음\n");
}

// 권장 루틴
// 권장 루틴 + 조언
void recommend_routine(const Log* arr, int n) {
    if (n == 0) {
        printf("데이터 없음\n");
        return;
    }

    int sum_sh = 0, sum_sm = 0, sum_wh = 0, sum_wm = 0;

    for (int i = 0; i < n; i++) {
        sum_sh += arr[i].sleep_h;
        sum_sm += arr[i].sleep_m;
        sum_wh += arr[i].wake_h;
        sum_wm += arr[i].wake_m;
    }

    // 평균 취침/기상 시각
    int avg_sh = sum_sh / n;
    int avg_sm = sum_sm / n;
    int avg_wh = sum_wh / n;
    int avg_wm = sum_wm / n;

    // 평균 수면시간 계산 (분 단위 → 시간)
    int Smin = avg_sh * 60 + avg_sm;
    int Wmin = avg_wh * 60 + avg_wm;
    int diff = (Wmin - Smin + 1440) % 1440;   // 자정 넘김 고려
    double avg_hours = diff / 60.0;

    printf("=== 권장 루틴 분석 ===\n");
    printf("평균 취침 시각: %02d:%02d\n", avg_sh, avg_sm);
    printf("평균 기상 시각: %02d:%02d\n", avg_wh, avg_wm);
    printf("평균 수면 시간: %.2f시간\n", avg_hours);

    // 1) 수면 시간(7~9시간 기준) 코멘트
    if (avg_hours < 6.0) {
        printf("→ 수면 시간이 많이 부족합니다. 최소 7시간 이상 자는 패턴으로 바꾸는 걸 추천합니다.\n");
    }
    else if (avg_hours < 7.0) {
        printf("→ 살짝 부족한 편입니다. 7시간 이상으로 조금만 더 늘려보는 게 좋겠습니다.\n");
    }
    else if (avg_hours <= 9.0) {
        printf("→ 권장 수면 시간(7~9시간) 범위 안에 있습니다. 수면 시간은 괜찮은 편입니다.\n");
    }
    else if (avg_hours <= 10.0) {
        printf("→ 수면 시간이 조금 긴 편입니다. 컨디션이 괜찮다면 7~9시간 정도로 줄이는 것도 좋습니다.\n");
    }
    else {
        printf("→ 수면 시간이 많이 긴 편입니다. 낮 시간 졸림이나 생활 리듬을 한 번 점검해보는 걸 추천합니다.\n");
    }

    // 2) 취침 시각(22~23시 권장) 코멘트
    int bed = Smin;  // 평균 취침 시각(분)

    if (bed >= 22 * 60 && bed < 23 * 60 + 60) {
        printf("→ 취침 시각이 오후 10~11시 사이로, 권장 수면 시간대에 가깝습니다. 좋은 패턴입니다.\n");
    }
    else if (bed < 21 * 60) {
        printf("→ 너무 이른 시간(오후 9시 이전)에 자는 편입니다. 생활 리듬을 기준으로 다시 조절해보는 것도 좋겠습니다.\n");
    }
    else if (bed < 22 * 60) {
        printf("→ 오후 9~10시 사이에 자는 편입니다. 크게 나쁘진 않지만, 10~11시 사이로 조금 늦춰도 좋겠습니다.\n");
    }
    else if (bed < 24 * 60) {
        printf("→ 오후 11시 이후에 자는 편입니다. 가능하다면 22~23시 사이로 조금 앞당기는 걸 추천합니다.\n");
    }
    else {
        // 이론상 안 나오지만 혹시 모를 오버플로 방지
        printf("→ 취침 시각이 많이 늦은 편입니다. 22~23시 사이로 맞추는 걸 강하게 추천합니다.\n");
    }

    // 3) 간단한 최종 한 줄 정리
    if (avg_hours >= 7.0 && avg_hours <= 9.0 &&
        bed >= 22 * 60 && bed < 23 * 60 + 60) {
        printf("\n종합: 수면 시간과 취침 시각 모두 권장 범위에 있어요. 지금 패턴을 잘 유지하면 좋겠습니다.\n");
    }
    else {
        printf("\n종합: 수면 시간은 7~9시간, 취침은 오후 10~11시 사이를 목표로 패턴을 조금씩 조정해보면 좋겠습니다.\n");
    }
}

