#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <math.h>
#define H 500
#define W 500
#define LOD 1000
unsigned char Iarray[H][W][3];
#define maxlevel 255
int point_x[10] = {0,};
int point_y[10] = {0,};
double bef_x, bef_y;
int ROUND(double x)
{
	if (x >= 0)
		return (int)(x + 0.5);
	else
		return (int)(x - 0.5);
}
unsigned int Factorial(unsigned int n)
{
	if (n <= 1)
		return 1;
	else
		return n * Factorial(n - 1);
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

int  Combination(int n, int k)  // 이항정리
{
	return ROUND(Factorial(n) / (Factorial(k)*Factorial(n - k)));
}

void bezier(int n, double u)
{
	double px = 0, py = 0;
	double bazi[10] = {0,};
	for (int i = 0; i <=n; i++) {
		bazi[i] = Combination(n, i)*pow(u, i)*pow(1-u, n - i);
	}

	for (int i = 0; i <= n; i++) {
		px += point_x[i] * bazi[i];
		py += point_y[i] * bazi[i];
	}
	
	lineDDA(ROUND(bef_y), ROUND(bef_x), ROUND(py), ROUND(px));
	bef_x = px; 
	bef_y = py;

}
void Pu(int n)
{
	double u = 0.;
	bef_x = point_x[0];
	bef_y = point_y[0];
	
	for (int i = 1; i < LOD; i++) {
		u = i / (double)(LOD - 1);
		bezier(n, u);
	}

}
int main()
{
	int n;
	FILE* fp;
	fp = fopen("bezier.ppm", "wb");
	fprintf(fp, "P6\n"); //magic no.: ASCII header and Binary body.. 
	fprintf(fp, "%d %d\n", H, W); //file size 
	fprintf(fp, "%d\n", maxlevel); //max level 
	init();
	printf("제어점의 수를 입력하시오 : ");
	scanf("%d", &n);

	for (int i = 0; i < n; i++) {
		printf("%d 번째 제어점의 좌표를 입력하시오 (x,y) : ", i + 1);
		scanf("%d %d", &(point_x[i]), &(point_y[i]));
	}
	Pu(n-1);

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
			for (int k = 0; k < 3; k++)
				putc(Iarray[i][j][k], fp);
	fclose(fp);

}

