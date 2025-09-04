#include <stdio.h>
#include "game.h"
#include "show.h"

#define GAME_START 1
#define SHOW_INSTR 2
#define QUIT 3

int menu()
{
	int choice;
	puts("1.Start game");
	puts("2.Show instructions");
	puts("3.Quit");
	scanf("%d", &choice);
	return choice;
}

int main()
{	
	int menu_choice;
	menu_choice = menu();
	switch(menu_choice)
	{
		case GAME_START:
			game_start();
			break;
		case SHOW_INSTR:
			show_instr();
			break;
		case QUIT:
		default:
			puts("Goodbye!");
			break;
	}
	return 0;
}
