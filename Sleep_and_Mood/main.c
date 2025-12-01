#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "core.h"
#include "calc.h"
#include "input.h"
#include "io.h"
#include "report.h"

int main(void)
{
    Log* arr = NULL;
    int n = 0;
    int menu = 0;
    int running = 1;

    // 이전 기록 불러오기
    if (load_csv(&arr, &n, "sleep_log.csv") == 0)
        printf("[기록 불러오기 완료]\n");
    else
        printf("[새 데이터로 시작]\n");

    while (running)
    {
        printf("\n===== Sleep & Mood Analyzer =====\n");
        printf("1. 새 기록 입력\n");
        printf("2. 일간 조회\n");
        printf("3. 주간 분석\n");
        printf("4. 월간 분석\n");
        printf("5. 상관 분석\n");
        printf("6. 텍스트 그래프\n");
        printf("7. 수면 부족 경고\n");
        printf("8. 권장 수면 루틴\n");
        printf("9. 기록 수정\n");
        printf("10. 기록 삭제\n");
        printf("11. 전체 초기화\n");
        printf("12. 저장 후 종료\n");
        printf("13. 수면시간 히스토그램\n");
        printf("14. 위에 로그 지우기\n");
        printf("메뉴 >> ");
        scanf("%d", &menu);

        if (menu == 1) {
            Log x;
            input_new(&x);
            add_log(&arr, &n, x);
            sort_by_date(arr, n);
        }
        else if (menu == 2) {
            char d[11];
            printf("조회 날짜: ");
            scanf("%10s", d);
            print_daily(arr, n, d);
        }
        else if (menu == 3) {
            report_week(arr, n);
        }
        else if (menu == 4) {
            int y, m;
            printf("연도 월 입력(YYYY MM): ");
            scanf("%d %d", &y, &m);
            report_month(arr, n, y, m);
        }
        else if (menu == 5) {
            report_corr(arr, n);
        }
        else if (menu == 6) {
            plot_bars(arr, n);
        }
        else if (menu == 7) {
            warn_irregular(arr, n);
        }
        else if (menu == 8) {
            recommend_routine(arr, n);
        }
        else if (menu == 9) {
            char d[11];
            Log x;
            printf("수정할 날짜: "); scanf("%10s", d);
            input_new(&x);
            if (edit_log(arr, n, d, x) == 0) printf("수정 완료\n");
            else printf("날짜 없음\n");
        }
        else if (menu == 10) {
            char d[11];
            printf("삭제할 날짜: "); scanf("%10s", d);
            if (delete_log(arr, &n, d) == 0) printf("삭제 완료\n");
            else printf("날짜 없음\n");
        }
        else if (menu == 11) {
            reset_all(&arr, &n);
        }
        else if (menu == 12) {
            save_csv(arr, n, "sleep_log.csv");
            printf("저장 후 종료합니다.\n");
            running = 0;
        }
        else if (menu == 13) {
            plot_histogram(arr, n);
        }
        else if (menu == 14) {
            printf("위의 로그들을 지웁니다.\n");
            Sleep(1000);
            system("cls");
            printf("지우기 완료\n");
        }
        else {
            printf("잘못 입력함\n");
        }
    }

    free(arr);
    return 0;
}
