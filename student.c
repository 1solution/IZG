/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id: $
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - ibobak@fit.vutbr.cz, orderedDithering
 */

#include "student.h"
#include "globals.h"

#include <time.h>

const int M[] = {
    0, 204, 51, 255,
    68, 136, 187, 119,
    34, 238, 17, 221,
    170, 102, 153, 85
};

const int M_SIDE = 4;

/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).
 Ukol za 0.25 bodu */
S_RGBA getPixel(int x, int y)
{
	if (x >= 0 && x <= width && y >= 0 && y <= height)
		return *(frame_buffer + x + width*y); // preved na 1d a vrat: ukazatel na buffer + vzdalenost kde se prave nachazim, postupne po radcich
	return COLOR_BLACK; //vraci barvu (0, 0, 0)
}
/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.
 Ukol za 0.25 bodu */
void putPixel(int x, int y, S_RGBA color)
{
	if (x >= 0 && x <= width && y >= 0 && y <= height)
		*(frame_buffer + x + width*y) = color; // prevod na 1d
}
/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na odstiny sedi. Vyuziva funkce GetPixel a PutPixel.
 Ukol za 0.5 bodu */
void grayScale() {
	S_RGBA pixel; // pixel, kterym budu nahrazovat
	int color; // ulozi vyslednou barvu pixelu
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			pixel = getPixel(x, y); // nacti pixel
			color = ROUND(pixel.red*0.299 + pixel.green*0.587 + pixel.blue*0.114); // prevod na sedou
			pixel.red = color;
			pixel.green = color;
			pixel.blue = color;
			putPixel(x, y, pixel);
		}
	}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu maticoveho rozptyleni.
 Ukol za 1 bod */

void orderedDithering() {
	grayScale(); // preved nejdriv na odstiny sede
	S_RGBA pixel;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			pixel = getPixel(x, y); // nacti konkretni pixel
			int m_x = x % M_SIDE;
			int m_y = y % M_SIDE;
			if (pixel.red > M[m_x + (m_y*M_SIDE)]) // porovnat jednu hodnotu pixelu s rozdel. matici, prevedeno na 1d pole
				putPixel(x, y, COLOR_WHITE);
			else
				putPixel(x, y, COLOR_BLACK);
		}
	}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu distribuce chyby.
 Ukol za 1 bod */

void error(float e, int x, int y) { // funkce k distribuci chyby okolnich pixelu prave zpracovavaneho pixelu
	S_RGBA pixel = getPixel(x, y);
	int color = ROUND(pixel.red + e); // zapracuj chybu
	if (color < 0) // podteceni
		color = 0;
	if (color > 255) // preteceni
		color = 255;
	pixel.red = color;
	pixel.green = color;
	pixel.blue = color;
	putPixel(x, y, pixel);
}

void errorDistribution() {
	grayScale(); // preved na sedou
	for (int y = 0; y < height - 1; y++) {
		for (int x = 0; x < width - 1; x++) {
			S_RGBA pixel = getPixel(x, y);
			int color = pixel.red;

			if (pixel.red <= 127) // prah = 127
				pixel = COLOR_BLACK;
			else
				pixel = COLOR_WHITE;

			int e = color - pixel.red;

			error(3 * e / 8.0, x + 1, y); // vpravo vedle
			error(3 * e / 8.0, x, y + 1); // po dnim
			error(2 * e / 8.0, x + 1, y + 1); // ob nej
			putPixel(x, y, pixel);
		}
	}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci metody prahovani.
 Demonstracni funkce */
void thresholding(int Threshold)
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > Threshold)
				putPixel(x, y, COLOR_WHITE);
			else
				putPixel(x, y, COLOR_BLACK);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci nahodneho rozptyleni. 
 Vyuziva funkce GetPixel, PutPixel a GrayScale.
 Demonstracni funkce. */
void randomDithering()
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Inicializace generatoru pseudonahodnych cisel */
	srand((unsigned int)time(NULL));

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);
			
			/* Porovname hodnotu cervene barevne slozky s nahodnym prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > rand()%255)
			{
				putPixel(x, y, COLOR_WHITE);
			}
			else
				putPixel(x, y, COLOR_BLACK);
		}
}
/*****************************************************************************/
/*****************************************************************************/