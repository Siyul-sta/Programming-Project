#include<stdio.h>
#include<input.h>
#include<calc.h>

void input_new(Log* x) {
	printf("날짜 (YYYY-MM-DD): ");
	scanf("%10s, x->data");
	printf("취침 시각 (HH MM): ");
	scanf("%d %d", &x->sleep_h, &x->sleep_m);
	printf("기상 시각 (HH MM): ");
	scanf("%d %d", &x->wake_h, &x->wake_m);
	printf("오늘의 기분 (1~5): "); 
	scanf("%d", &x->mood);
	x->hours = calc_hours(x->sleep_h, x->sleep_m, x->wake_h, x->wake_m);
	printf("총 수면시간: %.2f시간\n", x->hours);
	printf("오늘의 기분 (1~5): "); 
	scanf("%d", &x->mood);
	x->hours = calc_hours(x->sleep_h, x->sleep_m, x->wake_h, x->wake_m);
	printf("총 수면시간: %.2f시간\n", x->hours);
	return;
}