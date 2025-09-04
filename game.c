#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <term.h>
#include <time.h>
#include "game.h"

#define SLOW 1
#define MEDIUM 2
#define FAST 3
#define ERR_SPEED -1

#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's'
#define GAMEOVER 'x'
#define MAX_SCORE 1000

#define MIN_WIDTH 18
#define MIN_HEIGHT 6

#define MICRO_MULTIPLIER 1000 

const int slow_rt = 100;
const int medium_rt = 67;
const int fast_rt = 33;

int width, height;

int fruitx, fruity; 
int headx, heady;

int tailX[100];
int tailY[100];
int tail_len = 0;

int gameover;
int refresh_time;

char key;
char prev_key = RIGHT;
int score; 

int ti;


void check_coords()
{
	for(int i = tail_len; i >= 0; i--)
	{
		if(fruitx == tailX[i] && fruity == tailY[i])
		{
			fruit_new_coords();
		}
	}
}

void fruit_new_coords()
{
	/* Formula: rand() % (greatest - lowest + 1) + lowest */
	fruitx = rand() % (width - 2) + 1;
	fruity = rand() % (height - 5) + 1;
	check_coords();
}

int get_rt()
{
	int ch;
	puts("Choose speed:");
	puts("1. Slow");
	puts("2. Medium");
	puts("3. Fast");
	scanf("%d", &ch);
	switch(ch)	
	{
		case SLOW:
			return slow_rt;
		case MEDIUM:
			return medium_rt;
		case FAST:
			return fast_rt;
		default:
			return ERR_SPEED;
	}
	
}

int check_term_valid()
{
	char* termname = getenv("TERM");
	char termbuf[1024];
	if(tgetent(termbuf, termname) < 1)
		return -1;
	return 0;
}

int setup_wh()
{
	width = tgetnum("co");
	height = tgetnum("li");
	if(width < MIN_WIDTH || height < MIN_HEIGHT){
		return -1;
	}
	return 0;
}

int setup()
{

	srand(time(NULL));
	gameover = 0;
	score = 0;
	ti = 0;

	if(check_term_valid() < 0)
	{
		puts("Could not start on your terminal!");
		return -1;
	}

	if(setup_wh() < 0)
	{
		puts("Terminal window too small!");
		return -1;	
	}
	
	
	refresh_time = get_rt();
	if(refresh_time < 0) return -1;

	fruit_new_coords();	

	headx = width / 2;
	heady = (height - 2) / 2 - 1;

	return 0;
}

void input()
{
	/* Fixes holding-one-key problem */
	while((key = tolower(getsymv())) == prev_key);

	switch(key)
	{
		case RIGHT:
			key = RIGHT;	
			break;
		case LEFT:
			key = LEFT;	
			break;
		case UP:
			key = UP;	
			break;
		case DOWN:
			key = DOWN;	
			break;
		case GAMEOVER:
			gameover = 1;
			break;
		default: 
			key = prev_key;
			break;
	}
		
}

void draw()
{
	system("clear");
	for(int i = 0; i < width; i++)
	{
		printf("#");
	} 
	for(int i = 0; i < height - 4; i++)
	{
		for(int j = 0; j < width;  j++)
		{
			if(j == 0 || j == width-1) printf("#");
			else if(i == heady && j == headx)
			{
				printf("O");
			}
			else if(i == fruity && j == fruitx)  
			{
				if(fruit_is_eaten()) printf(" ");
				else printf("$");
			}
			else 
			{
				int is_tail = 0;
				
				for(int t = 1; t <= tail_len; t++)
				{	
					
					if(j == tailX[t] && i == tailY[t])
					{
						if(t == tail_len) printf("+");
						else printf("o");
						is_tail = 1;
					}
				}
				if(!is_tail) printf(" ");	
			}
		}
		printf("\r\n");
	}
	for(int i = 0; i < width; i++)
	{
		printf("#");
	} 
	printf("\r\n");
	printf("Score:  %d\r\n", score);
}

int fruit_is_eaten()
{
	return (headx == fruitx && heady == fruity);
}

void tail_grow()
{	
		
	if(score <= 10)
	{
		switch(prev_key)
		{
			case RIGHT:
				tailX[ti] = tailX[ti-1] - 1;
				tailY[ti] = tailY[ti-1];
			case LEFT:
				tailX[ti] = tailX[ti-1] + 1;
				tailY[ti] = tailY[ti-1];
			case UP:
				tailX[ti] = tailX[ti-1];
				tailY[ti] = tailY[ti-1] + 1;
			case DOWN:
				tailX[ti] = tailX[ti-1];
				tailY[ti] = tailY[ti-1] - 1;
		}
	}	
	else
	{
		if(tailX[ti - 1] == tailX[ti - 2])
		{
			if(tailY[ti - 1] > tailY[ti - 2]){
				tailY[ti] = tailY[ti - 1] + 1;
				tailX[ti] = tailX[ti - 1];
			}
			else
			{
				tailY[ti] = tailY[ti - 1] - 1;
				tailX[ti] = tailX[ti - 1];
			}
		}
		else if(tailY[ti - 1] == tailY[ti - 2])
		{
			
			if(tailX[ti - 1] > tailX[ti - 2]){
				tailY[ti] = tailY[ti - 1];
				tailX[ti] = tailX[ti - 1] + 1;
			}
			else
			{
				tailY[ti] = tailY[ti - 1];
				tailX[ti] = tailX[ti - 1] - 1;
			}
		}
	}
}

void tail_move()
{
	/* if(){}; Обработка поворота на месте  */
	for(int i = tail_len; i >= 1; i--)
	{
		tailX[i] = tailX[i-1];
		tailY[i] = tailY[i-1];
	}	
}

int collision()
{
	if(headx == 0 || headx == (width - 1) || heady == -1 || heady == (height - 4)) return 1;
	if(tail_len >= 3)
	{
		for(int i = 3; i <= tail_len; i++)
		{
			if(headx == tailX[i] && heady == tailY[i])
			{
				return 1;
			}
		}
	}
	return 0;
}

void check_turn()
{
	if(prev_key == RIGHT && key == LEFT) key = prev_key;
	if(prev_key == LEFT && key == RIGHT) key = prev_key;
	if(prev_key == UP && key == DOWN) key = prev_key;
	if(prev_key == DOWN && key == UP) key = prev_key;
}

void logic()
{
	tailX[0] = headx;
	tailY[0] = heady;	

	if(collision() || score == MAX_SCORE) 
	{
		gameover = 1;
		return;
	}

	if(fruit_is_eaten()){
		score += 10;
		ti++;	
		tail_len++;
		fruit_new_coords();
		tail_grow();
	}
	
	check_turn();
	
	if(key == LEFT)
	{
		headx--;
	}
	if(key == RIGHT){
		headx++;
	}	
	if(key == UP){
		heady--;
	}
	if(key == DOWN){
		heady++;
	}

	prev_key = key;

	tail_move();	
}

void gameover_draw()
{
	int gameover_x = width / 2 - 4;
	int gameover_y = (height - 2) / 2 - 1;
	int score_x = gameover_x - 3;
	int score_y = gameover_y + 1;
	system("clear");
	for(int i = 0; i < width; i++)
	{
		printf("#");
	} 
	for(int i = 0; i < height - 3; i++)
	{
		for(int j = 0; j < width;  j++)
		{
			if(j == 0 || j == width-1) printf("#");
			else if(i == gameover_y && j == gameover_x)
				printf("GAMEOVER");
			else if(i == gameover_y && (j - gameover_x) > 0 && (j - gameover_x) < 8)
				continue;
			else if(i == score_y && j == score_x)
				if(score < MAX_SCORE)
					printf("Your score: %3d", score);
				else 
					printf("YOU WON THE GAME");
			else if(i == score_y && (j - score_x) > 0 && (j - score_x) < 15)
				continue;
			else
				printf(" ");
		}
		printf("\r\n");
	}
	for(int i = 0; i < width; i++)
	{
		printf("#");
	} 
	printf("\r\n");
	getchar();
}

void game_loop()
{
	enable_raw_mode();
	while(!gameover){
		draw();
		input();
		logic();
		usleep(MICRO_MULTIPLIER * refresh_time);	
	}
	disable_raw_mode();	
}

void game_start()
{
	if(setup() < 0){
		puts("Could not start game!");
		exit(0);
	}
	game_loop();

	if(gameover){
		usleep(MICRO_MULTIPLIER * 200);
		gameover_draw();
	}
}

