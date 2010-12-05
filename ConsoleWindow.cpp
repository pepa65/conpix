/*
 * ConsoleWindow.cpp
 *
 *  Created on: 29.11.2010
 *      Author: rofl
 */
#include <assert.h>

#include "ConsoleWindow.h"
#include "Format.h"


ConsoleWindow::ConsoleWindow(ILogger* _logger) {
	logger = _logger;
	lastattr = 0;
	inittables();
	initscr();
    noecho();
	cbreak();
	keypad(stdscr, TRUE);
	hasColors = has_colors();
	canChangeColors = hasColors ? can_change_color() : false;
	if (hasColors) start_color();
	if (canChangeColors) saveColors();
	maxcol = 0;
	log(format("hascolors: %d" , hasColors));
	log(format("canchangecolors: %d" , canChangeColors));

}

ConsoleWindow::~ConsoleWindow() {
	if (canChangeColors) restoreColors();
	endwin();
}

void ConsoleWindow::saveColors() {
	for (short int i = 0; i < colorpaircount; i++) {
		short int r,g,b;
		color_content(i, &r, &g, &b);
		org_colors[i] = rgb(fromThousand(r), fromThousand(g), fromThousand(b));
	}
	for (short int i = 1; i < colorpaircount; i++) {
		short int f, b;
		pair_content(i, &f, &b);
		assert(f < colorpaircount && b < colorpaircount);
		org_fgcolors[i] = f;
		org_bgcolors[i] = b;
	}
}

void ConsoleWindow::restoreColors() {
	for (short int i = 0; i < colorpaircount; i++) {
		setCursesColor((int)i-1, org_colors[i]);
	}
	for (short int i = 1; i < colorpaircount; i++) {
		init_pair(i, org_fgcolors[i], org_fgcolors[i]);
	}
}

void ConsoleWindow::log(string s) {
	if (logger != NULL) logger->logln(s);
}

void ConsoleWindow::inittables() {
	for (int i = 0; i < colorpaircount; i++) colors[i]=-1;
	actfgcol = -1;
	actbgcol = -1;
}

bool ConsoleWindow::setColor(RGB mycolor, bool fg) {
	log(format("setcolor %d, %d, %d", mycolor.r, mycolor.g, mycolor.b));
	// see if it's the actual color...
	if (fg) {
		if (actfgcol >= 0) {
			if (colors[actfgcol] == (int32_t) mycolor.asInt) return true;
		}
	} else {
		if (actbgcol >= 0) {
			if (colors[actbgcol] == (int32_t) mycolor.asInt) return true;
		}
	}

	// this (c|sh)ould be optimized by using a not-yet-existing-in-c++-hashmap
	for (int i = 0; i < colorpaircount; i++) {
		if (colors[i] == -1) {
				colors[i] = mycolor.asInt;
				setCursesColor(i, mycolor);
		}
		if (colors[i] == -1 || colors[i] == (int32_t) mycolor.asInt ) {
				if(fg) actfgcol = i; else actbgcol = i;
				return true;
		}
	}
	assert (false); // "could not set color");
}

void ConsoleWindow::initoutput() {
	if (actfgcol == -1) setColor(rgb(0xFF, 0xFF, 0xFF),true);
	if (actbgcol == -1) setColor(rgb(0, 0, 0),false);
	int32_t* ptrfg = &colors[actfgcol];
	int32_t* ptrbg = &colors[actbgcol];
	for(int i=0;i<colorpaircount;i++) {
		if(fgcolors[i] == ptrfg) {
				if (bgcolors[i]!=ptrbg)	continue;
				else {
					useColorPair(i);
					return;
				}
		} else if (fgcolors[i] == NULL) {
				fgcolors[i]=ptrfg;
				bgcolors[i]=ptrbg;
				setColorPair(i, actfgcol, actbgcol);
				useColorPair(i);
				return;
		}
	}
	assert(false); // "colorpair not found");
}

int ConsoleWindow::fromThousand(int in) {
	return in == 0 ? 0 : in == 1000 ? 255 : (in * 1000 * 1000) / 3921568;
}


int ConsoleWindow::toThousand(int in) {
	// i dont like floats...
	return in == 0 ? 0 : in == 255 ? 1000 : (in * 3921568) / (1000 * 1000);
}

bool ConsoleWindow::setCursesColor(int colornumber, RGB color) {
	log(format("setCursesColor %d: %d %d %d",colornumber, color.r, color.g, color.b));
	assert(colornumber < colorpaircount);
	// we use rgb values in the range 0-0xFF, while ncurses max is 1000
	if(!canChangeColors) return false;

	int nr = toThousand(color.r);
	int ng = toThousand(color.g);
	int nb = toThousand(color.b);
	return init_color(colornumber+1, nr, ng, nb) != FALSE;
}

bool ConsoleWindow::setColorPair(int pair, int fgcol, int bgcol) {
	log(format("setColorPair %d, fg: %d bg: %d", pair, fgcol, bgcol));
	assert(fgcol < colorpaircount && bgcol < colorpaircount); // "color pair is out of index");
	if (!hasColors) return false;
	fgcolors[pair] = &colors[fgcol];
	bgcolors[pair] = &colors[bgcol];
	return init_pair(pair+1, fgcol+1, bgcol+1) != FALSE;
}

bool ConsoleWindow::useColorPair(int pair) {
	log(format("useColorPair %d", pair));
	assert(pair < colorpaircount);
	if (!hasColors) return false;
	if (lastattr) attroff(lastattr);
	lastattr = COLOR_PAIR(pair+1);
	attron(lastattr);
	return true;
}

void ConsoleWindow::getSize(int& x, int& y){
	x = getmaxx(stdscr);
	y = getmaxy(stdscr);
}

void ConsoleWindow::gotoxy(int x, int y) {
	move(y, x);
}

void ConsoleWindow::addchar(int c, uint attributes) {
	initoutput();
	addch(c | attributes);
}


void ConsoleWindow::printfxy (int x, int y, char* text) {
	initoutput();
	mvprintw(x, y, "%s", text, 0);
}

char ConsoleWindow::getKey() {
	return getch();
}

void ConsoleWindow::sleep(int ms) {
	napms(ms);
}

void ConsoleWindow::reFresh() {
	refresh();
}

