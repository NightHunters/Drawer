#include "PaintStatus.h"

PaintStatus::PaintStatus():color(0,0,0)
{
	Min.x = 16384;
	Min.y = 16384;
	Max.x = -1024;
	Max.y = -1024;
	confirm_points = 0;
	num = 0;
	status = 0;
}

void PaintStatus::add_point(int x, int y)
{
	Point p(x, y);
	vec.push_back(p);
	num++;
}

void PaintStatus::update_point(int n, int x, int y)
{
	if (n < num)
	{
		vector<Point>::iterator it = vec.begin();
		it += n;
		it->x = x;
		it->y = y;
	}
}

void PaintStatus::refresh()//ˢ�µ�ǰ״̬
{
	Min.x = 16384;
	Min.y = 16384;
	Max.x = -1024;
	Max.y = -1024;
	confirm_points = 0;
	num = 0;
	vec.clear();
}

void PaintStatus::add_one_comfirm_point()
{
	confirm_points++;
}

void PaintStatus::calculate_area()
{
	Min.x = 16384;
	Min.y = 16384;
	Max.x = -1024;
	Max.y = -1024;
	vector<Point>::iterator it = vec.begin();
	for (int i = 0; i < confirm_points; i++)
	{
		if ((it + i)->x < Min.x)
			Min.x = (it + i)->x;
		if ((it + i)->x > Max.x)
			Max.x = (it + i)->x;
		if ((it + i)->y < Min.y)
			Min.y = (it + i)->y;
		if ((it + i)->y > Max.y)
			Max.y = (it + i)->y;
	}
}
