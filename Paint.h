#pragma once
#include<qpainter.h>
#include <math.h>
#include<cmath>
#include<iostream>
using namespace std;

class Paint :public QPainter//继承QPainter，自己实现画图的实现
{
public:
	Paint(QPaintDevice* d);
	void drawline_DDA(int x1, int y1, int x2, int y2);//线生成的DDA算法
	void drawline_Bresenham(int x1, int y1, int x2, int y2);//线段生成的Bresenham算法
	void drawpolygon_DDA(int n, int x[], int y[]);//绘制多边形的DDA算法
	void drawpolygon_Bresenham(int n, int x[], int y[]);//绘制多边形的Bresenham算法
	void drawellipse(int x, int y, int rx, int ry);//绘制椭圆

	double BEZ(int k, int n, double u);//BEZ函数，用来辅助绘制贝塞尔曲线
	void drawcurse_Bezier(int n, int x[], int y[]);//绘制曲线的贝塞尔算法

	double F_0_3(double t);
	double F_1_3(double t);
	double F_2_3(double t);
	double F_3_3(double t);
	void drawcurse_B_spline(int n, int x[], int y[]);//绘制曲线的三次均匀B样条算法
};