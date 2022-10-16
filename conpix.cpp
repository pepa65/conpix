// conpix.cpp
// Display images in 256 colors xterm
// Copyright: rofl0r	License: GNU GPLv3

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

#include "Lepton.h"
#include "Logger.h"
#include "Format.h"
#include "ConsoleWindow.h"


using namespace std;

bool fexists(const char *filename) {
	ifstream ifile(filename);
	return ifile.is_open();
}

int main(int argc, char** argv) {
	string filename = "";
	bool scaleFullScreen = false;
	string arg;
	for (int i = 1; i<argc; i++) {
		arg = argv[i];
		if (arg == "-f") scaleFullScreen=true;
		else filename = arg;
	}

	if (!fexists(filename.c_str())) {
		string self = argv[0];
		cout << "Filename needed!" << endl << endl;
		cout << "conpix - Display images in 256 colors xterm" << endl;
		cout << "Usage:  " << self << " [-f] <imagefile.jpg|png|bmp|tiff>" << endl;
		cout << "  -f:    Stretch to fullscreen" << endl;
		cout << "Recommended before usage:  export TERM=xterm-256color" << endl;
		exit(1);
	}

	SimpleFileLogger* l = new SimpleFileLogger("log.txt");
	l->clear();

	ConsoleWindow* t = new ConsoleWindow(l);
	int cx; int cy;
	t->getSize(cx, cy);

	Pix* pngfile = pixRead(filename.c_str());

	float xfactor = 1.0; float yfactor = 1.0; int w; int h;
	pixGetDimensions(pngfile, &w, &h, NULL);
	Pix* ping;

	if(scaleFullScreen) {
		xfactor = (cx * 1.0) / (w * 1.0);
		yfactor = (cy * 1.0) / (h * 1.0);
		ping = pixScale(pngfile, xfactor, yfactor);
	} else {
		if (cx *2 < w) xfactor = (cx * 1.0) / (w * 1.0);
		if (cy < h) yfactor = (cy * 1.0) / (h * 1.0);
		float factor = (xfactor < yfactor) ? xfactor: yfactor;
		if (cx *2 < w || cy < h)
			ping = pixScale(pngfile, factor * 2.0, factor);
		else // Scale to double width so the proportion of the font is correct
			ping = pixScale(pngfile, 2.0, 1.0 );
	}
	pixDestroy(&pngfile);
	Pix* palette = pixOctreeColorQuant(ping, 240, 1);
	if (palette == NULL) { cout << "palette is nul"; return 1; }

	Pix* pix32;
	pix32 = pixConvertTo32(palette);

	l->logln(format("pal_number, pal_nalloc, pal_bits: %d, %d, %d" , palette->colormap->n, palette->colormap->nalloc, palette->colormap->depth));

	int iterX = pix32->w;
	int iterY = pix32->h;
	int* bufptr = (int*) pix32->data;
	if (bufptr == NULL) {
		cout << "bufptr is null";
		return 1;
	}

	for (int iy=0;iy<iterY;iy++){
		for (int ix=0;ix<iterX;ix++){
			t->setColor(*((RGB*) bufptr), false);
			t->gotoxy(ix, iy);
			t->addchar(' ', 0);
			bufptr++;
		}
	}
	pixDestroy(&palette);
	pixDestroy(&pix32);

	t->refresh();
	t->getKey();

	delete(t);
	delete(l);
	return EXIT_SUCCESS;
}
