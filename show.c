#include <stdio.h>
#include "show.h"

void show_instr()
{
	puts("This is traditional snake game.");
	puts("This game will run only on linux so far!!!");
	puts("");	
	puts("Movement: w = up, s = down, d = right, a = left");
	puts("Leave game: x");
	puts("");
	puts("Scoring system:");
	puts("Each time snake eats fruit('$') your score grows by 10");
	puts("If you reach score 1000 -> gameover (YOU WON THIS GAME !!!). *But i don't think you can :)*");
	puts("");
	puts("You can choose speed(refresh rate). Slow = 10 fps, Medium = 15 fps, Fast = 30 fps");
	puts("The game field fits to your terminal window, ");
	puts("BUT..");
	puts("If you have < 6 lines or < 18 columns -> game won't run on your terminal");
	puts("");
	puts("Have fun! And report any bugs you catch :3");
	getchar();
}
