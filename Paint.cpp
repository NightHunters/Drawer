#include "Paint.h"

Paint::Paint(QPainter *pa) :painter(pa)//构造函数
{
	pen.setColor(QColor(0, 0, 0, 255));
	painter->setPen(pen);
}
void Paint::setcolor(int r, int g, int b)//设置画笔颜色
{
	pen.setColor(QColor(r, g, b, 255));
	painter->setPen(pen);
}
void Paint::drawline_DDA(int x1, int y1, int x2, int y2)//线生成的DDA算法
{
	float increx, increy, x, y;
	int steps, i;
	if (abs(x2 - x1) > abs(y2 - y1))
		steps = abs(x2 - x1);
	else
		steps = abs(y2 - y1);

	increx = (float)(x2 - x1) / steps;
	increy = (float)(y2 - y1) / steps;
	x = x1;
	y = y1;

	for (i = 1; i <= steps; i++)
	{
		painter->drawPoint(x, y); //在(x，y)处，以color色画点
		x += increx;
		y += increy;
	}
}
void Paint::drawline_Bresenham(int x1, int y1, int x2, int y2)//线段生成的Bresenham算法
{

}
