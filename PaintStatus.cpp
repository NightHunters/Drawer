#include "PaintStatus.h"

PaintStatus::PaintStatus():color(0,0,0)
{
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
	confirm_points = 0;
	num = 0;
	vec.clear();
}

void PaintStatus::add_one_comfirm_point()
{
	confirm_points++;
}
