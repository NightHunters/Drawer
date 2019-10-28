#pragma once
#include "Drawer.h"
#include <math.h>

class Paint
{
	QPainter *painter;
	//QPixmap *pix;
	QPen pen;
	//QColor color;
public:
	Paint(QPainter *pa);//构造函数
	void setcolor(int r, int g, int b);//设置画笔颜色
	void drawline_DDA(int x1, int y1, int x2, int y2);//线生成的DDA算法
	void drawline_Bresenham(int x1, int y1, int x2, int y2);//线段生成的Bresenham算法
};