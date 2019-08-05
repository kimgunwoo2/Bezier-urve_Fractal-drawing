#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

#define H 500
#define W 500
#define maxlevel 255

unsigned char Iarray[H][W][3];

int ROUND(double x)
{
	if (x >= 0)
		return ((int)(x + 0.5));
	else
		return ((int)(x - 0.5));
}
void lineDDA(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1, dy = y2 - y1, step, k;
	float xinc, yinc, x = (float)x1, y = (float)y1;

	if (abs(dx) > abs(dy)) step = abs(dx);
	else step = abs(dy);
	xinc = dx / (float)step;
	yinc = dy / (float)step;

	for (k = 0; k<step; k++)
	{
		x += xinc;
		y += yinc;

		if (x >= 0 && x<H && y >= 0 && y<W)
		{
			Iarray[(int)(x + 0.5)][(int)(y + 0.5)][0] = 0;
			Iarray[(int)(x + 0.5)][(int)(y + 0.5)][1] = 0;
			Iarray[(int)(x + 0.5)][(int)(y + 0.5)][2] = 0;
		}
	}
}
void init() {
	int i, j, k;
	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
			for (k = 0; k < 3; k++)
				Iarray[i][j][k] = 255;
}
void gasket(int x1, int y1, int x2, int y2, int x3, int y3, int n)
{
	int midx[3], midy[3];

	midx[0] = (x1 + x2) / 2;
	midy[0] = (y1 + y2) / 2;

	midx[1] = (x2 + x3) / 2;
	midy[1] = (y2 + y3) / 2;

	midx[2] = (x1 + x3) / 2;
	midy[2] = (y1 + y3) / 2;

	lineDDA(x1, y1, x2, y2);
	lineDDA(x2, y2, x3, y3);
	lineDDA(x3, y3, x1, y1);

	if (n>0) {
		gasket(x1, y1, midx[0], midy[0], midx[2], midy[2], n - 1);
		gasket(x2, y2, midx[0], midy[0], midx[1], midy[1], n - 1);
		gasket(x3, y3, midx[1], midy[1], midx[2], midy[2], n - 1);
	}

}
int main() {
	FILE* fp;
	int i, j, k;
	fp = fopen("sierpinskgasket.ppm", "wb");
	fprintf(fp, "P6\n"); //magic no.: ASCII header and Binary body.. 
	fprintf(fp, "%d %d\n", H, W); //file size 
	fprintf(fp, "%d\n", maxlevel); //max level 
	init();
	gasket(50, 250, 450, 50, 450, 450, 6);



	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
			for (k = 0; k < 3; k++)
				putc(Iarray[i][j][k], fp);
	fclose(fp);
	return 0;
}