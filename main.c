#include "myLib.h"
#include "text.h"
#include "welcome.h"
#include "gameBG.h"
//#include "end_win.h"
#include "end_lose.h"
#include <stdio.h>
#include <stdlib.h>


#define MAXNUMBAR 10
#define GLOBALVOL -1
#define BARTHICKNESS 3
#define PLAYERHEIGHT 10
#define PLAYERWIDTH 5
#define PLAYERSPEED 2
#define FALLINGSPEED 1

typedef struct {
	int row;
	int col;
	int length;
	u16 color;
} MOVBAR;

typedef struct {
	int row;
	int col;
	int height;
	int width;
	int rd;
	int health;
	int state; // 0: free falling  1: on normal bars
	u16 color;
} PLAYER;

typedef enum {WELCOME, PLAYING, ENDLOSE} STATE;

MOVBAR addBar();
void updatePlayer(PLAYER *pptr, MOVBAR *barsptr);


int main() {
	char welbuf1[50];
	char welbuf2[50];
	char socbuf[20];
	char healbuf[20];
	char insbuf1[20];
	char insbuf2[20];
	char end1[40];
	char end2[40];
	int barCount = 0;

	PLAYER player, oldPlayer;

	MOVBAR bars[MAXNUMBAR];
	MOVBAR oldBars[MAXNUMBAR];
	MOVBAR *cur;

	REG_DISPCTL = MODE3 | BG2_ENABLE;

	STATE gamestate = WELCOME;

	int score = 0;
	sprintf(welbuf2, "PRESS SELECT ANYTIME TO MAIN");

	while(1) // Game Loop
	{	
		switch(gamestate)
		{
			case WELCOME:
				drawImage3(0, 0, 240, 160, welcome);
				sprintf(welbuf1, "PRESS ENTER TO START");
				drawString(90, 60, welbuf1, RED);
				drawString(100, 40, welbuf2, YELLOW);
				while (gamestate == WELCOME)
				{
					if (KEY_DOWN_NOW(BUTTON_START))
					{
						gamestate = PLAYING;
					}
				}
				break;

			case PLAYING:

				drawImage3(0, 0, 240, 160, gameBG);
				player.row = 10;
				player.col = 140;
				player.height = PLAYERHEIGHT;
				player.width = PLAYERWIDTH;
				player.rd = GLOBALVOL;
				player.health = 10;
				player.state = 0;
				player.color = YELLOW;
				oldPlayer = player;

				score = 0;

				u16 bgcolor = BLACK;
				int gameCount = 0;
				//int barCount = 0;
				int health = 10;
				int pause = 0;

				sprintf(insbuf1, "A: PAUSE");
				sprintf(insbuf2, "B: RESUME");
				drawString(120, 0, insbuf1, RED);
				drawString(130, 0, insbuf2, RED);

				while (gamestate == PLAYING)
				{
					if (KEY_DOWN_NOW(BUTTON_SELECT))
					{
							gamestate = WELCOME;
					}
					if (!pause)
					{
						gameCount++;
						if (KEY_DOWN_NOW(BUTTON_A))
						{
							pause = 1;
						}
						if(KEY_DOWN_NOW(BUTTON_RIGHT))
						{
							if (player.col < 233) {	
								player.col += PLAYERSPEED;
							}
						}
						if(KEY_DOWN_NOW(BUTTON_LEFT))
						{
							if (player.col > 60) {	
								player.col -= PLAYERSPEED;
							}
						}
						health = player.health;
						updatePlayer(&player, bars);
						sprintf(socbuf, "Score:%3d", score);
						sprintf(healbuf, "Health:%2d", health);

						WaitForVblank();
						if (gameCount > 40) {
							if ((rand() % 3) == 0) {
								MOVBAR newBar = addBar();
								gameCount = gameCount - 40;
								score++;
								barCount = barCount % MAXNUMBAR;
								bars[barCount] = newBar;
								oldBars[barCount] = newBar;
								barCount++;
							}
						}
						for (int i = 0; i < MAXNUMBAR; i++) {
							cur = bars + i;
							cur->row = cur->row + GLOBALVOL;
							if (cur->row > 7) {
								drawRect(oldBars[i].row, oldBars[i].col, BARTHICKNESS, oldBars[i].length, bgcolor);
							}
						}
						drawRect(oldPlayer.row, oldPlayer.col, oldPlayer.height, oldPlayer.width, bgcolor);

						if (health <= 0)
						{
							gamestate = ENDLOSE;
						}

						for (int i = 0; i < MAXNUMBAR; i++) {
							cur = bars + i;
							if (cur->row > 8) {
								drawRect(cur->row, cur->col, BARTHICKNESS, cur->length, cur->color);
							}
							oldBars[i] = bars[i];
						}
						drawRect(player.row, player.col, player.height, player.width, player.color);
						oldPlayer = player;
						drawRect(140, 0, 20, 58, BROWN);
						drawString(140, 0, socbuf, YELLOW);
						drawString(150, 0, healbuf, YELLOW);
					} else {
						if (KEY_DOWN_NOW(BUTTON_B))
						{
							pause = 0;
						}
					}
				}
				break;
			// case ENDWIN:
			// 	drawImage3(0, 0, 240, 160, end_win);
			// 	while (gamestate == ENDWIN)
			// 	{
			// 		if (KEY_DOWN_NOW(BUTTON_SELECT))
			// 			{
			// 				gamestate = WELCOME;
			// 			}
			// 	}
			// 	break;
			case ENDLOSE:
				drawImage3(0, 0, 240, 160, end_lose);
				if (score < 50)
				{
					sprintf(end1, "SCORE: %d  30S!", score);
					sprintf(end2, "NEED PRACTICE");
				} else if (score < 100)
				{
					sprintf(end1, "SCORE: %d  2MIN!", score);
					sprintf(end2, "HALFMAN");
				} else if (score < 200)
				{
					sprintf(end1, "SCORE: %d  7MIN!", score);
					sprintf(end2, "REAL MAN");
				} else if (score < 300)
				{
					sprintf(end1, "SCORE: %d  20MIN!", score);
					sprintf(end2, "SUPER MAN");
				} else
				{
					sprintf(end1, "SCORE: %d  TAISHUANGLE!", score);
				}
				drawString(30, 125, end1, BLACK);
				drawString(40, 125, end2, BLACK);
				drawString(150, 40, welbuf2, RED);
				while (gamestate == ENDLOSE)
				{
					if (KEY_DOWN_NOW(BUTTON_SELECT))
						{
							gamestate = WELCOME;
						}
				}
				break;
		}
	}
}

MOVBAR addBar() {
	MOVBAR newBar;
	newBar.length = 30 + rand() % 30;
	newBar.row = 160;
	newBar.col = 60 + rand() % 130;
	newBar.color = RED;
	return newBar;
}

void updatePlayer(PLAYER *pptr, MOVBAR *barsptr) {
	MOVBAR *curb;
	if (pptr->state == 0) {
		pptr->row += FALLINGSPEED;
	} else if (pptr->state == 1) {
		pptr->row += GLOBALVOL;
	}
	pptr->state = 0;
	for (int i = 0; i < MAXNUMBAR; i++) {
		curb = barsptr + i;
		if (barsCheck(pptr->row, pptr->col, curb->row, curb->col, curb->length)) {
			pptr->state = 1;
		}
	}

	if (pptr->row == 7) {
		pptr->health--;
		pptr->state = 0;
	} else if (pptr->row == 155) {
		pptr->health -= 3;
		pptr->row = 10;
		pptr->col = 140;
	}
}
