#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;


void setPixel(int row, int col, u16 color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, u16 color)
{
	for(int r=0; r<height; r++)
	{
		for(int c=0; c<width; c++)
		{
			setPixel(row+r, col+c, color);
		}
	}
}

int barsCheck(int prow, int pcol, int brow, int bcol, int blen)
{
	if ((pcol <= bcol + blen - 5) && (pcol >= bcol - 5) &&
		((brow - prow == 8) || (brow - prow == 9))) {
		return 1;
	}
	return 0;
}

/* drawimage3
* A function that will draw an arbitrary sized image
* onto the screen (with DMA).
* @param r row to draw the image
* @param c column to draw the image
* @param width width of the image
* @param height height of the image
* @param image Pointer to the first element of the image.
*/
void drawImage3(int r, int c, int width, int height, const u16* image)
{
	for (int rn = 0; rn < height; rn++) {
		DMA[3].src = image + OFFSET(rn, 0, width);;
		DMA[3].dst = videoBuffer + OFFSET(r + rn, c, width);
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT;
	}

}


int boundsCheck(int *var, int bound, int *delta, int size)
{
		if(*var < 0)
		{
			*var = 0;
			*delta = -*delta;
			return 1;
		}
		if(*var > bound-size+1)
		{
			*var = bound-size+1;
			*delta = -*delta;
		}
		return 0;

}

void WaitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}


