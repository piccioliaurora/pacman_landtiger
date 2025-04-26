#include "pacman.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include <stdbool.h>

#define VALUE_SPILLS 10
#define VALUE_PPILLS 50

//240x360 display panel

void initStandardPills (StandardPills *sp){
	sp->value = VALUE_SPILLS;
}
void initPowerPills (PowerPills *pp) {
	pp->value = VALUE_PPILLS;
}

void initGameSpace() {
	GUI_Text(10, 40, (unsigned char*) "Game Over In", 0xffff, Black);
	GUI_Text (15, 40, (unsigned char*)  , 0xffff, Black);
	GUI_Text(10, 320, (unsigned char*) "Score", 0xffff, Black);
	GUI_Text(15, 320, (unsigned char*) , 0xffff, Black);
}

