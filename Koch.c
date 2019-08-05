#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define W 500
#define H 500
#define maxP 100000
#define maxlevel 255
#define N 5
unsigned char Iarray[H][W][3];
double point_x[maxP] = { 0.3, 0.3, 0.9, 0.3 };
double point_y[maxP] = { 0.1, 0.9, 0.5, 0.1 };
double temp_x[maxP] = { 0., };
double temp_y[maxP] = { 0., };
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
int main()
{
	FILE* fp;
	fp = fopen("Koch.ppm", "wb");
	fprintf(fp, "P6\n"); //magic no.: ASCII header and Binary body.. 
	fprintf(fp, "%d %d\n", H, W); //file size 
	fprintf(fp, "%d\n", maxlevel); //max level 
	init();


	 
	int tp = 4;


	// Koch Curve

	for (int i = 0; i<tp; i++) {
		temp_x[i] = point_x[i];
		temp_y[i] = point_y[i];

	}

	for (int st = 0; st < N; st++) {

		tp += (tp - 1) * 3;
		if (tp > maxP) break;

		for (int i = 0; i < tp; i += 4) {
			temp_x[i] = point_x[i / 4];
			temp_y[i] = point_y[i / 4];
		}

		for (int i = 0; i < tp; i++) {
			if ((i % 4) == 1) {
				// 1st Point
				temp_x[i] = temp_x[i - 1] + (temp_x[i + 3] - temp_x[i - 1]) / 3.;
				temp_y[i] = temp_y[i - 1] + (temp_y[i + 3] - temp_y[i - 1]) / 3.;
				// 3rd Point
				temp_x[i + 2] = temp_x[i + 3] - (temp_x[i + 3] - temp_x[i - 1]) / 3.;
				temp_y[i + 2] = temp_y[i + 3] - (temp_y[i + 3] - temp_y[i - 1]) / 3.;

			}
			else if ((i % 4) == 2) {
				temp_x[i] = (temp_x[i + 1] - temp_x[i - 1]) * 1 / 2. - (temp_y[i + 1] - temp_y[i - 1]) * sqrt(3.) / 2. + temp_x[i - 1];
				temp_y[i] = (temp_x[i + 1] - temp_x[i - 1]) * sqrt(3.) / 2. + (temp_y[i + 1] - temp_y[i - 1]) * 1 / 2. + temp_y[i - 1];
			}

		}

		for (int i = 0; i < tp; i++) {
			point_x[i] = temp_x[i];
			point_y[i] = temp_y[i];
			//printf("%d :   %f %f\n", i, px[i], py[i]);
		}
	}
	for (int i = 0; i < tp - 1; i++) {
		lineDDA((ROUND)(point_x[i] * H), (ROUND)(point_y[i] * W), (ROUND)(point_x[i + 1] * H), (ROUND)(point_y[i + 1] * W));
	}


	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
			for (int k = 0; k < 3; k++)
				putc(Iarray[i][j][k], fp);
	fclose(fp);
	return 0;
}