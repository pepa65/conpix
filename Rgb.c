// Rgb.c
// Copyright: rofl0r  License: GNU GPLv3

#include "Rgb.h"

RGB rgb(int r, int g, int b) {
	RGB result;
	result.r = r; // & 0xFF
	result.g = g; // & 0xFF00
	result.b = b; // & 0xFF0000
	return result;
}
