#include <stdio.h>
#include <time.h>
#include "input.h"
#include "calc.h"

// 새 기록 입력
void input_new(Log* x) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int when = 0;
    printf("1. 지금 잘 거예요\n");
    printf("2. 지금 일어났어요\n");
    printf("3. 직접 입력\n");
    printf("선택 >> ");
    scanf("%d", &when);

    if (when == 1) {
        // 지금 자는 경우: 오늘 날짜 + 현재 시각을 취침 시간으로만 저장
        sprintf(x->date, "%04d-%02d-%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

        x->sleep_h = tm.tm_hour;
        x->sleep_m = tm.tm_min;

        // 아직 모르는 값들은 임시로 -1 / 0.0
        x->wake_h = -1;
        x->wake_m = -1;
        x->mood = -1;
        x->hours = 0.0;  // 기상 시각 모르면 수면시간 계산 불가

        printf("오늘(%s) 취침 시각을 %02d:%02d로 기록해 뒀습니다.\n",
            x->date, x->sleep_h, x->sleep_m);
    }
    else if (when == 2) {
        // 지금 일어난 경우: 오늘 날짜 + 현재 시각을 기상 시간으로만 저장
        sprintf(x->date, "%04d-%02d-%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

        x->wake_h = tm.tm_hour;
        x->wake_m = tm.tm_min;

        // 나머지는 아직 미입력 상태
        x->sleep_h = -1;
        x->sleep_m = -1;
        x->mood = -1;
        x->hours = 0.0;

        printf("오늘(%s) 기상 시각을 %02d:%02d로 기록해 뒀습니다.\n",
            x->date, x->wake_h, x->wake_m);
    }
    else if (when == 3) {

        int overnight = 0;

        printf("수면이 날짜를 넘겼나요?\n");
        printf("1. 아니요 (같은 날짜에서 잠 → 깸)\n");
        printf("2. 네 (어제 자고 오늘 일어났어요)\n");
        printf("선택 >> ");
        scanf("%d", &overnight);

        printf("취침 날짜 (YYYY-MM-DD): ");
        scanf("%10s", x->date);

        printf("취침 시각 (HH MM): ");
        scanf("%d %d", &x->sleep_h, &x->sleep_m);

        if (overnight == 1) {
            // 같은 날짜 안에서 끝남
            printf("기상 시각 (HH MM): ");
            scanf("%d %d", &x->wake_h, &x->wake_m);
        }
        else {
            // 어제 → 오늘로 넘어가는 경우
            time_t t_now = time(NULL);
            struct tm tm_now = *localtime(&t_now);

            // 기상 날짜는 오늘로 자동 입력
            x->wake_h = -1;
            x->wake_m = -1;

            printf("기상 시각 (HH MM): ");
            scanf("%d %d", &x->wake_h, &x->wake_m);

            // 그래도 date는 "취침 날짜"로 저장하고,
            // calc_hours가 자정 넘김을 자동 처리한다.
        }

        printf("오늘 기분(1~5): ");
        scanf("%d", &x->mood);

        // 총 수면시간 자동 계산
        x->hours = calc_hours(x->sleep_h, x->sleep_m,
            x->wake_h, x->wake_m);

        printf("총 수면시간: %.2f시간\n", x->hours);
    }
}