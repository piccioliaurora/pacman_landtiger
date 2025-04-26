#include "LPC17xx.h"
#include "pacman.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include "RIT/RIT.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

int score = 0;
int timer = 60;
int lives = 1;
int gameOver;
int pacmanX, pacmanY;
int totPills = NUM_TOT_PILLS;

void initGame(void){
		LCD_Initialization();
		LCD_Clear(Black);

		drawMaze();
		initStandardPills();	
		setStartingPosition();
		drawPacman();
		displayGameStatus();
		displayStart();
}

void initStandardPills(void){
		int row, col;
		int placedStandardPills = 0;

    for (row = 0; row < MAZE_ROWS; row++) {
        for (col = 0; col < MAZE_COLUMNS; col++) {
            if (maze[row][col] == PATH && placedStandardPills < totPills) {
                maze[row][col] = STANDARD_PILL; 
                placedStandardPills++;
            }
        }
    }
		drawPills();
}

void initPowerPills(void){
    int placedPowerPills = 0;
    while (placedPowerPills < NUM_POWER_PILLS) {
        int row = rand() % MAZE_ROWS;
        int col = rand() % MAZE_COLUMNS;

        if (maze[row][col] == STANDARD_PILL) {
            maze[row][col] = POWER_PILL;
            placedPowerPills++;
        }
    }
		drawPills();
}

void displayGameStatus(void) {
		char scoreText[20];
    char timerText[20];
	
		GUI_Text(10, 1, (unsigned char*) "GAME OVER IN", White, Black);
		if (timer < 10){
				sprintf(timerText, "0%ds", timer);
		} else {
				sprintf(timerText, "%ds", timer);
		}
    GUI_Text(10, 21, (unsigned char*) timerText, White, Black);
		
		GUI_Text(180, 1, (unsigned char*) "SCORE", White, Black);
		sprintf(scoreText, "%d", score);
    GUI_Text(180, 21, (unsigned char*) scoreText, White, Black);
		
		int i;
    int iconRadius = 7; 
    int iconSpacing = 30;
		int yPosition = MAX_Y - 10;

    for (i = 0; i < lives; i++) {
        int xPosition = 10 + i * iconSpacing; 
        drawPacmanIcon(xPosition, yPosition, iconRadius);
    }
}


void displayStart(void){
		GUI_Text(92, 160, (unsigned char*) " START ", Red, White);	
}

int displayPause(int x){
		if (x==1){
				disable_timer(1);
				disable_timer(2);
				GUI_Text(92, 160, (unsigned char*) " PAUSE ", Red, White);		
		} else {
				enable_timer(1);
				enable_timer(2);
				clearSection(92, 160);
		}
		return x;
}

void displayGameOver(void){
		GUI_Text(74, 160, (unsigned char*) " GAME OVER !", Red, White);	
		disable_timer(1);
		disable_timer(2);
		disable_RIT();
}

void displayVictory(void){
		GUI_Text(76, 160, (unsigned char*) " VICTORY ! ", Red, White);	
		disable_timer(1);
		disable_timer(2);
		disable_RIT();
}

void drawMaze(void) {
		int totalMazeHigh = MAZE_ROWS*SCALE;
		int verticalPadding = 40;
		
		int row,col;
		int y, x;
		
		for (row = 0; row < MAZE_ROWS; row++) {
        for (col = 0; col < MAZE_COLUMNS; col++) {
            int xStart = col * SCALE;
            int yStart = verticalPadding + (row * SCALE);

            uint16_t color;
            if (maze[row][col] == PATH || maze[row][col] == PATH_WITHOUT_PILLS || maze[row][col] == TELEPORT_D || maze[row][col] == TELEPORT_S) {
                color = Blue;
            } else if (maze[row][col] == WALL) {
                color = Black;
						}

            for (y = yStart; y < yStart + SCALE; y++) {
                for (x = xStart; x < xStart + SCALE; x++) {
                    LCD_SetPoint(x, y, color);
                }
            }
        }
    }
}

void drawPills(void) {
		int verticalPadding = 40;
		int row, col;
	
    for (row = 0; row < MAZE_ROWS; row++) {
        for (col = 0; col < MAZE_COLUMNS; col++) {
            int xStart = col * SCALE;
            int yStart = verticalPadding + (row * SCALE);

            if (maze[row][col] == STANDARD_PILL) {
                drawStandardPill(xStart + SCALE / 2, yStart + SCALE / 2);
            } else if (maze[row][col] == POWER_PILL) {
                drawPowerPill(xStart + SCALE / 2, yStart + SCALE / 2);
            }
        }
    }
}

void drawStandardPill(int xCenter, int yCenter) {
    int halfSize = SCALE / 6;
	  int x,y;
    for (y = yCenter - halfSize; y <= yCenter + halfSize; y++) {
        for (x = xCenter - halfSize; x <= xCenter + halfSize; x++) {
            LCD_SetPoint(x, y, White);
        }
    }
}

void drawPowerPill(int xCenter, int yCenter) {
    int halfSize = SCALE / 3;
		int x,y;
    for (y = yCenter - halfSize; y <= yCenter + halfSize; y++) {
        for (x = xCenter - halfSize; x <= xCenter + halfSize; x++) {
            LCD_SetPoint(x, y, Red);
        }
    }
}

void drawPacmanIcon(int xCenter, int yCenter, int radius) {
    int x, y;
    for (y = -radius; y <= radius; y++) {
        for (x = -radius; x <= radius; x++) {
            if ((x * x + y * y) <= (radius * radius)) {
                if (y > 0 || x > y / 2) { 
                    LCD_SetPoint(xCenter + x, yCenter + y, Yellow);
                }
            }
        }
    }
}

void setStartingPosition(void) {
    int row, col;
		for (row = 0; row < MAZE_ROWS; row++) {
        for (col = 0; col < MAZE_COLUMNS; col++) {
            if (maze[row][col] == PATH_WITHOUT_PILLS) {
                pacmanX = row;
                pacmanY = col;
                return; 
            }
        }
    }
}

void drawPacman(void) {
    int xStart = pacmanY * SCALE; 
    int yStart = pacmanX * SCALE + 40;  

    drawPacmanIcon(xStart + SCALE / 2, yStart + SCALE / 2, SCALE / 2.5);  
}

void clearPacman(int xCenter, int yCenter){
		int xStart = yCenter * SCALE; 
    int yStart = xCenter * SCALE + 40;
    int x, y;

    for (y = yStart; y < yStart + SCALE; y++) {
        for (x = xStart; x < xStart + SCALE; x++) {
            LCD_SetPoint(x, y, Blue);
        }
    }
}

void clearSection(int xSec, int ySec) {
    int i, j;
    int width = 60;
    int height = 20; 

    for (i = ySec; i < ySec + height; i++) { 
        for (j = xSec; j < xSec + width; j++) { 
            int col = j / SCALE;     
            int row = (i - 40) / SCALE;  
					
            uint16_t color;
            if (maze[row][col] == WALL) {
                color = Black;
            } else if (maze[row][col] == PATH || maze[row][col] == PATH_WITHOUT_PILLS ||
                       maze[row][col] == TELEPORT_D || maze[row][col] == TELEPORT_S) {
                color = Blue;
            } else {
                color = White; 
            }
            LCD_SetPoint(j, i, color); 
        }
    }
}
void checkPosition(void){
		int nextPos;
    
		switch (direction) {
			case 'u':  
				nextPos=maze[pacmanX-1][pacmanY]; 
				movePacman(pacmanX-1, pacmanY, nextPos);
			break;
      case 'd':  
				nextPos=maze[pacmanX+1][pacmanY]; 
			  movePacman(pacmanX+1, pacmanY, nextPos);
			break;
      case 'l':  
				nextPos=maze[pacmanX][pacmanY-1]; 
			  movePacman(pacmanX, pacmanY-1, nextPos);
			break;
      case 'r':  
				nextPos=maze[pacmanX][pacmanY+1]; 
				movePacman(pacmanX, pacmanY+1, nextPos);
			break;
			default: break;
    }
}
	
void movePacman(int x, int y, int nextPos) {
		if (nextPos != WALL) {
			clearPacman(pacmanX, pacmanY);
			
			if (nextPos == TELEPORT_S) {
					 x=13;
					 y=23;
			} else if (nextPos == TELEPORT_D) {
					 x=13;
					 y=0;
			}
			if (nextPos == STANDARD_PILL) {
           score += 10;
					 totPills--;
           maze[x][y] = PATH;
			} else if (nextPos == POWER_PILL) {
           score += 50;
					 totPills--;
           maze[x][y] = PATH;
			} 

			if(score==1000 || score==2000){
						lives ++;
			}
			
			if (totPills == 0){
						displayVictory();
			}
			
			pacmanX = x;
			pacmanY = y;
			drawPacman();
	}
}



