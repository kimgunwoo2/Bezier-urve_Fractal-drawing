#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define W 500
#define H 500
#define maxlevel 255

#define PI 3.141592
unsigned char Iarray[H][W][3];
double getRadian(double num) {
	return num * (PI / 180);
}
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
void BST(int n, int x1, int y1, int x2, int y2, int mode)
{
	int theta = 30;
	double r = 0.65;
	double rad = 0;
	int xr, yr, x_nextr, y_nextr, x_nextl, y_nextl;
	int dx = (x2 - x1);
	int dy = (y2 - y1);
	if (n == 0) return;
	

	
	if (mode == 0) {

		xr = x1 + (ROUND)((1 - r) * dx);
		yr = y1 + (ROUND)((1 - r) * dy);

		// Right tree 
		rad = getRadian(180 - theta);
		x_nextr = (ROUND)(cos(rad)*(xr - x2) - sin(rad)*(yr - y2) + x2);
		y_nextr = (ROUND)(sin(rad)*(xr - x2) + cos(rad)*(yr - y2) + y2);
		lineDDA(x2, y2, x_nextr, y_nextr);
		BST(n - 1, x2, y2, x_nextr, y_nextr, 0);
		//Left tree
		rad = getRadian(180 + theta);
		x_nextl = (ROUND)(cos(rad)*(xr - x2) - sin(rad)*(yr - y2) + x2);
		y_nextl = (ROUND)(sin(rad)*(xr - x2) + cos(rad)*(yr - y2) + y2);
		lineDDA(x2, y2, x_nextl, y_nextl);
		BST(n - 1, x2, y2, x_nextl, y_nextl, 0);
	}
	else {
		
		r = r + (rand() % 3 - 1) / 10.;
		xr = x1 + (ROUND)((1 - r) * dx);
		yr = y1 + (ROUND)((1 - r) * dy);

		theta = rand() % 30 + 15;
		rad = getRadian(180 - theta);
		x_nextr = (ROUND)(cos(rad)*(xr - x2) - sin(rad)*(yr - y2) + x2);
		y_nextr = (ROUND)(sin(rad)*(xr - x2) + cos(rad)*(yr - y2) + y2);
		lineDDA(x2, y2, x_nextr, y_nextr);
		BST(n - 1, x2, y2, x_nextr, y_nextr, 1);

		r = r + (rand() % 3 - 1) / 10.;
		xr = x1 + (ROUND)((1 - r) * dx);
		yr = y1 + (ROUND)((1 - r) * dy);

		theta = rand() % 30 + 15;
		rad = getRadian(180 + theta);
		x_nextl = (ROUND)(cos(rad)*(xr - x2) - sin(rad)*(yr - y2) + x2);
		y_nextl = (ROUND)(sin(rad)*(xr - x2) + cos(rad)*(yr - y2) + y2);
		lineDDA(x2, y2, x_nextl, y_nextl);
		BST(n - 1, x2, y2, x_nextl, y_nextl, 1);
	}

	
	

}
void bound_tree(int x1, int y1, int x2, int y2) {
	lineDDA(x1, y1, x2, y2);
}
int main()
{
	FILE* fp;
	fp = fopen("tree_normal.ppm", "wb");
	fprintf(fp, "P6\n"); //magic no.: ASCII header and Binary body.. 
	fprintf(fp, "%d %d\n", H, W); //file size 
	fprintf(fp, "%d\n", maxlevel); //max level 
	init();
	bound_tree(499, 250, 400, 250);
	BST(10, 499, 250, 400, 250, 0);

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
			for (int k = 0; k < 3; k++)
				putc(Iarray[i][j][k], fp);
	fclose(fp);


	fp = fopen("tree_random.ppm", "wb");
	fprintf(fp, "P6\n"); //magic no.: ASCII header and Binary body.. 
	fprintf(fp, "%d %d\n", H, W); //file size 
	fprintf(fp, "%d\n", maxlevel); //max level 
	init();
	bound_tree(499, 250, 400, 250);
	BST(10, 499, 250, 400, 250, 1);
	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
			for (int k = 0; k < 3; k++)
				putc(Iarray[i][j][k], fp);
	fclose(fp);
	return 0;
}