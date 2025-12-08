#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include "../include/item.h"
#include "../include/backpack.h"
#include "../include/event.h"
#include "../include/player.h"
#include "../include/map.h"
#include "../include/shop.h"
#define OPTIONS 3

int main() {
	int choice = 0;
	while (1) {
		system("cls");
		fflush(stdin);
		choice += OPTIONS;
		choice %= OPTIONS;
		printf("%d", choice);
		if (choice == 0) {
			printf("\033[43;34;5m\033[4m----------Welcome to the RelicsSeeker!----------\n\n\033[0m");
			printf("           \033[47;30;5m>1. Start Game\n\033[0m");
			printf("           2. Load Game\n");
			printf("           3. Exit\n");
		}
		else if (choice == 1) {
			printf("\033[43;34;5m\033[4m----------Welcome to the RelicsSeeker!----------\n\n\033[0m");
			printf("           1. Start Game\n");
			printf("           \033[47;30;5m>2. Load Game\033[0m\n");
			printf("           3. Exit\n");
		}
		else {
			printf("\033[43;34;5m\033[4m----------Welcome to the RelicsSeeker!----------\n\n\033[0m");
			printf("           1. Start Game\n");
			printf("           2. Load Game\n");
			printf("           \033[47;30;5m>3. Exit\033[0m\n");
		}
		char ch = _getch();
		if (ch == 'w' || ch == 'W') {
			choice--;
		}
		else if (ch == 's' || ch == 'S') {
			choice++;
		}
		else if (ch == '\r') {
			switch (choice) {
			case 0:
				printf("this is case 0");
				break;
			case 1:
				printf("this is case 1");
				break;
			case 2:
				printf("this is case 2");
				break;

			}
		}
	}
	return 0;
}