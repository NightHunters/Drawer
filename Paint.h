#pragma once
#include "Drawer.h"
#include <math.h>

class Paint//��ͼ��
{
	QPainter *painter;
	QPen pen;
public:
	Paint(QPainter *pa);//���캯��
	void setcolor(int r, int g, int b);//���û�����ɫ
	void drawline_DDA(int x1, int y1, int x2, int y2);//�����ɵ�DDA�㷨
	void drawline_Bresenham(int x1, int y1, int x2, int y2);//�߶����ɵ�Bresenham�㷨
};