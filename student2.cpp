/******************************************************************************
 * Laborator 02 - Zaklady pocitacove grafiky - IZG
 * isolony@fit.vutbr.cz
 *
 * $Id: $
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * ivelas@fit.vutbr.cz
 * itomesek@fit.vutbr.cz
 */

#include "student.h"
#include "globals.h"

#include <time.h>

/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).*/
S_RGBA getPixel(int x, int y)
{
	if (x > 0 && y > 0 && x < width && y < height)
		return frame_buffer[y * width + x];
	else
		return makeBlackColor();
}

/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.*/
void putPixel(int x, int y, S_RGBA color)
{
	if (x > 0 && y > 0 && x < width && y < height)
		frame_buffer[y * width + x] = color;
}


#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Ukol za 2b
////////////////////////////////////////////////////////////////////////////////
#define FRAC_BITS 8 // DDA fixed point
void drawLine (int x1, int y1, int x2, int y2) { // zadany dva body, ktere definuji primku
	int dx = (x2-x1); // vzdalenost na ose x
	int dy = (y2-y1); // vzdalenost na ose y

	if(!dx && !dy) { // oba body zadane stejne, primku tvori pouze bod
		putPixel(x1, y2, COLOR_GREEN);
		return;
    	}
	
    	if(ABS(dy)>ABS(dx)) { // usecka klesa moc rychle, vymenit x a y souradnice
		SWAP(x1, y1); // timhle se dostane do spravne poloviny kvadrantu
		SWAP(x2, y2); // pak pri zapisu do bufferu taky prohodit
    	}
    	if(x1>x2) { // usecka smeruje zprava doleva, staci vymenit body mezi sebou
		SWAP(x1, x2); 
		SWAP(y1, y2);
    	}

	int y = y1 << FRAC_BITS;
	int k = (dy << FRAC_BITS) / dx;

	for (int x = x1; x <= x2; x++) {
        if(ABS(dy)>ABS(dx))
            putPixel(y >> FRAC_BITS,x , COLOR_GREEN); // znovu prohodit v pripade ze usecka byla ve spatne casti kvadrantu
        else
            putPixel( x, y >> FRAC_BITS, COLOR_GREEN); // nemenit, ani v pripade kdy usecka smerovala zprava doleva (body uz jsme mezi sebou prohodili nebo to neni potreba)
        y += k;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Ukol za 1b
////////////////////////////////////////////////////////////////////////////////
void put8PixelsOfCircle(int x, int y, S_RGBA color) {
    putPixel(-x,-y,color); // vykresleni osmi bodu podle zadani
    putPixel(-x,y,color);
    putPixel(x,-y,color);
    putPixel(x,y,color);
    putPixel(-y,-x,color);
    putPixel(-y,x,color);
    putPixel(y,-x,color);
    putPixel(y,x,color);    
}

void drawCircle (int s1, int s2, int R)
{
	S_RGBA col;
	col.blue = 0;
	col.green = 0;
	col.red = 255;

	int x = 0, y = R;
	int P = 1-R, X2 = 3, Y2 = 2*R-2;
	while (x <= y) {
		put8PixelsOfCircle(x, y, col);
		if (P >= 0) {
			P += -Y2;
			Y2 -= 2;
			y--;
		}
		P += X2;
		X2 += 2;
		x++;
	}
	return;
}

/*****************************************************************************/
/*****************************************************************************/
