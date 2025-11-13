#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include "core.h"
#include "io.h"
#include "calc.h"
#include "report.h"
#include "input.h"

int main() {
	
	printf("==== Sleep & Mood ====\n");
	int menu = 0; // 메뉴 선택번호
	int running = 1; // 반복문 1이면 실행, 0이면 종료
	int n = 0; // 현재 기록 개수
	Log* arr = NULL; // 수면기록 구조체 배열(core.h에서 정의)
	if (load_csv(&arr, &n, "sleep_log.csv") == 0) {
		printf("[데이터 로드 완료]\n");
	}
	else {
		printf("[새 데이터로 시작합니다]\n");
	}
	while (running) {
		printf("\n1. 새 기록 입력\n2. 데이터 조회\n3. 저장 후 종료\n");
		printf("메뉴 선택 >> ");
		scanf("%d", &menu);

		if (menu == 1) {
			//입력
			Log x;
			input_new(&x);
			add_log(&arr, &n, x);
			sort_by_data(arr, n);
		}
		else if (menu == 2) {
			//조회
			char d[11];
			printf("조회 날짜(YYYY-MM-DD): ");
			scanf("%10s", d);
			print_daily(arr, n, d);
		}
		else if (menu == 3) {
			save_csv(arr, n, "sleep_log.csv");
			running = 0;
			// 저장 및 종료
			save_csv(arr, n, "sleep_log.csv");
			printf("[자동 저장 완료] 프로그램을 종료합니다. \n");
		}
		else {
			printf("잘못된 입력입니다. \n");
		}
	}


	return 0;
}