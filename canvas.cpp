#include "canvas.h"

canvas::canvas(QWidget *parent) :QWidget(parent)
{
	reference_point.x = 0;
	reference_point.y = 0;
	captured_point = -5;
	transform_mode = 0;
	setMouseTracking(true);
}

canvas::~canvas()
{
	
}

void canvas::copy_status()
{
	copy_p = current_p;
}

void canvas::calculate_rotate_point()
{
	current_p.calculate_area();
	rotate_point.x = int((current_p.Min.x + current_p.Max.x) / 2.0 + 0.5);
	rotate_point.y = current_p.Min.y - 20;
}

bool canvas::is_able_change()
{
	if (vec.size() == 0 && current_p.vec.size() == 0)
		return true;
	return false;
}

void canvas::reset()
{
	vec.clear();
	current_p.refresh();
	current_p.status = 0;
	reference_point.x = 0;
	reference_point.y = 0;
	captured_point = -5;
	transform_mode = 0;
	update();
}

QColor canvas::get_color()
{
	return current_p.color;
}

void canvas::To_clip_line()
{
	if (current_p.status == 30)
		current_p.status = 40;
}

void canvas::Do_the_clip()//by_Cohen-Sutherland
{
	int xL = clip_point1.x;
	int yB = clip_point1.y;
	int xR = clip_point2.x;
	int yT = clip_point2.y;
	vector<Point>::iterator it = current_p.vec.begin();
	double x1 = it->x;
	double y1 = it->y;
	double x2 = (it + 1)->x;
	double y2 = (it + 1)->y;
	for (int i = 0; i < 4; i++)
	{
		int area_code1 = 0, area_code2 = 0;//区域码
		if (x1 < xL)
			area_code1 |= 0b0001;
		if (x1 > xR)
			area_code1 |= 0b0010;
		if (y1 < yB)
			area_code1 |= 0b0100;
		if (y1 > yT)
			area_code1 |= 0b1000;

		if (x2 < xL)
			area_code2 |= 0b0001;
		if (x2 > xR)
			area_code2 |= 0b0010;
		if (y2 < yB)
			area_code2 |= 0b0100;
		if (y2 > yT)
			area_code2 |= 0b1000;
		if (area_code1 == 0 && area_code2 == 0)//线段完全在窗口内
		{
			it->x = int(x1 + 0.5);
			it->y = int(y1 + 0.5);
			(it + 1)->x = int(x2 + 0.5);
			(it + 1)->y = int(y2 + 0.5);
			current_p.status = 30;
			update();
			return;
		}
		if ((area_code1 & area_code2) != 0)
		{
			Revoke_current();
			return;
		}
		if (area_code1 == 0)//P1与P2交换
		{
			double temp = 0;
			temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
			temp = area_code1;
			area_code1 = area_code2;
			area_code2 = temp;
		}
		if ((area_code1 & 0b1000) != 0)
		{
			x1 = (yT - y1)*(x1 - x2) / (y1 - y2) + x1;
			y1 = yT;
		}
		else if ((area_code1 & 0b0100) != 0)
		{
			x1 = (yB - y1)*(x1 - x2) / (y1 - y2) + x1;
			y1 = yB;
		}
		else if ((area_code1 & 0b0010) != 0)
		{
			y1 = (xR - x1)*(y1 - y2) / (x1 - x2) + y1;
			x1 = xR;
		}
		else if ((area_code1 & 0b0001) != 0)
		{
			y1 = (xL - x1)*(y1 - y2) / (x1 - x2) + y1;
			x1 = xL;
		}
	}
}


void canvas::Capture_cursor_line(int x, int y)
{
	current_p.calculate_area();
	if (x > current_p.Min.x - 11 && x<current_p.Max.x + 11 && y>current_p.Min.y - 11 && y < current_p.Max.y + 11)
	{
		int minlen = 16384;
		vector<Point>::iterator it = current_p.vec.begin();
		for (int t = 0; t < current_p.confirm_points; t++)
		{
			int c_len = ((it + t)->x - x)*((it + t)->x - x) + ((it + t)->y - y)*((it + t)->y - y);
			if (c_len < minlen)
			{
				minlen = c_len;
				captured_point = t;
			}
		}
		if (minlen < 121)
		{
			transform_mode = 4;
			return;
		}
		if (x > current_p.Min.x && x<current_p.Max.x && y>current_p.Min.y&& y < current_p.Max.y)
		{
			int x1 = it->x;//判断距离,利用距离公式
			int y1 = it->y;
			int x2 = (it + 1)->x;
			int y2 = (it + 1)->y;
			int dx = x1 - x2;
			int dy = y1 - y2;
			int p1 = dy*x - dx*y + x1*y2 - x2*y1;
			int p2 = 36 * (dy*dy + dx*dx);
			int p3 = p1*p1 - p2;
			if (p3 < 0)
			{
				transform_mode = 1;
				return;
			}
		}
		transform_mode = 0;
	}

	calculate_rotate_point();
	if ((rotate_point.x - x)*(rotate_point.x - x) + (rotate_point.y - y)*(rotate_point.y - y) < 81)
	{
		transform_mode = 2;
		return;
	}
	transform_mode = 0;
}

void canvas::Capture_cursor_polygon(int x, int y)
{
	current_p.calculate_area();
	if (x > current_p.Min.x - 11 && x<current_p.Max.x + 11 && y>current_p.Min.y - 11 && y < current_p.Max.y + 11)
	{
		int minlen = 16384;
		vector<Point>::iterator it = current_p.vec.begin();
		for (int t = 0; t < current_p.confirm_points; t++)
		{
			int c_len = ((it + t)->x - x)*((it + t)->x - x) + ((it + t)->y - y)*((it + t)->y - y);
			if (c_len < minlen)
			{
				minlen = c_len;
				captured_point = t;
			}
		}//找到最近的控制点

		int transform_point = 0;//临时标记变换点
		int transform_point_len = 0;
		if (2 * x > current_p.Max.x + current_p.Min.x)
		{
			if (2 * y > current_p.Max.y + current_p.Min.y)
			{
				transform_point = -3;
				transform_point_len = (x - current_p.Max.x)*(x - current_p.Max.x) + (y - current_p.Max.y)*(y - current_p.Max.y);
			}
			else
			{
				transform_point = -4;
				transform_point_len = (x - current_p.Max.x)*(x - current_p.Max.x) + (y - current_p.Min.y)*(y - current_p.Min.y);
			}
		}
		else
		{
			if (2 * y > current_p.Max.y + current_p.Min.y)
			{
				transform_point = -2;
				transform_point_len = (x - current_p.Min.x)*(x - current_p.Min.x) + (y - current_p.Max.y)*(y - current_p.Max.y);
			}
			else
			{
				transform_point = -1;
				transform_point_len = (x - current_p.Min.x)*(x - current_p.Min.x) + (y - current_p.Min.y)*(y - current_p.Min.y);
			}
		}//找到最近变换点

		 //比较最近控制点与最近变换点
		if (minlen < 81)
		{
			if (transform_point_len < 121)
			{
				if (transform_point_len < minlen)
				{
					captured_point = transform_point;
					transform_mode = 3;
					return;
				}
			}
			transform_mode = 4;
			return;
		}
		else
		{
			if (transform_point_len < 121)
			{
				captured_point = transform_point;
				transform_mode = 3;
				return;
			}

		}

		if (x > current_p.Min.x && x<current_p.Max.x && y>current_p.Min.y&& y < current_p.Max.y)//判断可平移位置
		{
			transform_mode = 1;
			return;
		}
		//transform_mode = 0;
	}
	calculate_rotate_point();
	if ((rotate_point.x - x)*(rotate_point.x - x) + (rotate_point.y - y)*(rotate_point.y - y) < 81)
	{
		transform_mode = 2;
		return;
	}
	transform_mode = 0;
}

void canvas::Capture_cursor_eclipse(int x, int y)
{
	current_p.calculate_area();
	if (x > current_p.Min.x - 11 && x<current_p.Max.x + 11 && y>current_p.Min.y - 11 && y < current_p.Max.y + 11)
	{

		int transform_point = 0;//临时标记变换点
		int transform_point_len = 0;
		if (2 * x > current_p.Max.x + current_p.Min.x)
		{
			if (2 * y > current_p.Max.y + current_p.Min.y)
			{
				transform_point = -3;
				transform_point_len = (x - current_p.Max.x)*(x - current_p.Max.x) + (y - current_p.Max.y)*(y - current_p.Max.y);
			}
			else
			{
				transform_point = -4;
				transform_point_len = (x - current_p.Max.x)*(x - current_p.Max.x) + (y - current_p.Min.y)*(y - current_p.Min.y);
			}
		}
		else
		{
			if (2 * y > current_p.Max.y + current_p.Min.y)
			{
				transform_point = -2;
				transform_point_len = (x - current_p.Min.x)*(x - current_p.Min.x) + (y - current_p.Max.y)*(y - current_p.Max.y);
			}
			else
			{
				transform_point = -1;
				transform_point_len = (x - current_p.Min.x)*(x - current_p.Min.x) + (y - current_p.Min.y)*(y - current_p.Min.y);
			}
		}
		if (transform_point_len < 121)
		{
			captured_point = transform_point;
			transform_mode = 3;
			return;
		}

		if (x > current_p.Min.x && x<current_p.Max.x && y>current_p.Min.y&& y < current_p.Max.y)//判断可平移位置
		{
			transform_mode = 1;
			return;
		}
	}
	transform_mode = 0;
}

void canvas::Capture_cursor_Bezier(int x, int y)
{
	Capture_cursor_polygon(x, y);
}

void canvas::Capture_cursor_Bspline(int x, int y)
{
	Capture_cursor_polygon(x, y);
}


void canvas::Transform_line(int x, int y)
{
	current_p.calculate_area();
	if (transform_mode == 1)
	{
		int dx = x - reference_point.x;
		int dy = y - reference_point.y;
		vector<Point>::iterator it = current_p.vec.begin();
		it->x += dx;
		it->y += dy;
		(it + 1)->x += dx;
		(it + 1)->y += dy;
		reference_point.x = x;
		reference_point.y = y;
	}
	else if (transform_mode == 2)
	{
		vector<Point>::iterator it = current_p.vec.begin();
		vector<Point>::iterator it1 = copy_p.vec.begin();
		int center_x = int((copy_p.Min.x + copy_p.Max.x) / 2.0 + 0.5);
		int center_y = int((copy_p.Min.y + copy_p.Max.y) / 2.0 + 0.5);
		double L1 = center_y - rotate_point.y;
		double L2_2 = (center_x - x)*(center_x - x) + (center_y - y)*(center_y - y);
		double L2 = sqrt(L2_2);
		double L3_2 = (rotate_point.x - x)*(rotate_point.x - x) + (rotate_point.y - y)*(rotate_point.y - y);
		double cos_a = 1;//旋转角的余弦值
		double sin_a = 0;//正弦值
		if (x != center_x||y != center_y)
		{
			cos_a = (L1*L1 + L2_2 - L3_2) / (2 * L1*L2);
			sin_a = sqrt(1 - cos_a*cos_a);
			if (x < center_x)
				sin_a = -sin_a;
		}
		it->x = int(center_x + (it1->x - center_x)*cos_a - (it1->y - center_y)*sin_a+0.5);
		it->y = int(center_y + (it1->x - center_x)*sin_a + (it1->y - center_y)*cos_a+0.5);
		(it+1)->x = int(center_x + ((it1 + 1)->x - center_x)*cos_a - ((it1+1)->y - center_y)*sin_a+0.5);
		(it+1)->y = int(center_y + ((it1 + 1)->x - center_x)*sin_a + ((it1+1)->y - center_y)*cos_a+0.5);
	}
	else if (transform_mode == 4)
	{
		vector<Point>::iterator it = current_p.vec.begin();
		(it + captured_point)->x = x;
		(it + captured_point)->y = y;
	}
}

void canvas::Transform_polygon(int x, int y)
{
	current_p.calculate_area();
	if (transform_mode == 1)
	{
		int dx = x - reference_point.x;
		int dy = y - reference_point.y;
		vector<Point>::iterator it = current_p.vec.begin();
		for (int t = 0; t < current_p.confirm_points; t++)
		{
			(it + t)->x += dx;
			(it + t)->y += dy;
		}
		reference_point.x = x;
		reference_point.y = y;
	}
	else if (transform_mode == 2)
	{
		vector<Point>::iterator it = current_p.vec.begin();
		vector<Point>::iterator it1 = copy_p.vec.begin();
		int center_x = int((copy_p.Min.x + copy_p.Max.x) / 2.0 + 0.5);
		int center_y = int((copy_p.Min.y + copy_p.Max.y) / 2.0 + 0.5);
		double L1 = center_y - rotate_point.y;
		double L2_2 = (center_x - x)*(center_x - x) + (center_y - y)*(center_y - y);
		double L2 = sqrt(L2_2);
		double L3_2 = (rotate_point.x - x)*(rotate_point.x - x) + (rotate_point.y - y)*(rotate_point.y - y);
		double cos_a = 1;//旋转角的余弦值
		double sin_a = 0;//正弦值
		if (x != center_x || y != center_y)
		{
			cos_a = (L1*L1 + L2_2 - L3_2) / (2 * L1*L2);
			sin_a = sqrt(1 - cos_a*cos_a);
			if (x < center_x)
				sin_a = -sin_a;
		}
		for (int t = 0; t < current_p.confirm_points; t++)
		{
			(it + t)->x = int(center_x + ((it1 + t)->x - center_x)*cos_a - ((it1 + t)->y - center_y)*sin_a + 0.5);
			(it + t)->y = int(center_y + ((it1 + t)->x - center_x)*sin_a + ((it1 + t)->y - center_y)*cos_a + 0.5);
		}
	}
	else if (transform_mode == 3)
	{
		int point_x = 0;
		int point_y = 0;//缩放中心点
		double u_x = 1;//缩放比例
		double u_y = 1;
		vector<Point>::iterator it = current_p.vec.begin();
		vector<Point>::iterator it1 = copy_p.vec.begin();
		if (captured_point == -1)
		{
			point_x = copy_p.Max.x;
			point_y = copy_p.Max.y;
			if (x < copy_p.Max.x&&y < copy_p.Max.y)
			{
				double dx = copy_p.Max.x - x;
				double dy = copy_p.Max.y - y;
				u_x = dx / (copy_p.Max.x - copy_p.Min.x);
				u_y = dy / (copy_p.Max.y - copy_p.Min.y);
			}
		}
		else if (captured_point == -2)
		{
			point_x = copy_p.Max.x;
			point_y = copy_p.Min.y;
			if (x < copy_p.Max.x&&y > copy_p.Min.y)
			{
				double dx = copy_p.Max.x - x;
				double dy = y - copy_p.Min.y;
				u_x = dx / (copy_p.Max.x - copy_p.Min.x);
				u_y = dy / (copy_p.Max.y - copy_p.Min.y);
			}
		}
		else if (captured_point == -3)
		{
			point_x = copy_p.Min.x;
			point_y = copy_p.Min.y;
			if (x > copy_p.Min.x&&y > copy_p.Min.y)
			{
				double dx = x - copy_p.Min.x;
				double dy = y - copy_p.Min.y;
				u_x = dx / (copy_p.Max.x - copy_p.Min.x);
				u_y = dy / (copy_p.Max.y - copy_p.Min.y);
			}
		}
		else if (captured_point == -4)
		{
			point_x = copy_p.Min.x;
			point_y = copy_p.Max.y;
			if (x > copy_p.Min.x&&y < copy_p.Max.y)
			{
				double dx = x-copy_p.Min.x;
				double dy = copy_p.Max.y - y;
				u_x = dx / (copy_p.Max.x - copy_p.Min.x);
				u_y = dy / (copy_p.Max.y - copy_p.Min.y);
			}
		}
		for (int t = 0; t < current_p.confirm_points; t++)
		{
			(it + t)->x = (it1 + t)->x*u_x + point_x*(1 - u_x);
			(it + t)->y = (it1 + t)->y*u_y + point_y*(1 - u_y);
		}
	}
	else if (transform_mode == 4)
	{
		vector<Point>::iterator it = current_p.vec.begin();
		(it + captured_point)->x = x;
		(it + captured_point)->y = y;
	}
}

void canvas::Transform_eclipse(int x, int y)
{
	current_p.calculate_area();
	if (transform_mode == 1)
	{
		int dx = x - reference_point.x;
		int dy = y - reference_point.y;
		vector<Point>::iterator it = current_p.vec.begin();
		for (int t = 0; t < current_p.confirm_points; t++)
		{
			(it + t)->x += dx;
			(it + t)->y += dy;
		}
		reference_point.x = x;
		reference_point.y = y;
	}
	else if (transform_mode == 3)
	{
		int point_x = 0;
		int point_y = 0;//缩放中心点
		double u_x = 1;//缩放比例
		double u_y = 1;
		vector<Point>::iterator it = current_p.vec.begin();
		vector<Point>::iterator it1 = copy_p.vec.begin();
		if (captured_point == -1)
		{
			point_x = copy_p.Max.x;
			point_y = copy_p.Max.y;
			if (x < copy_p.Max.x&&y < copy_p.Max.y)
			{
				double dx = copy_p.Max.x - x;
				double dy = copy_p.Max.y - y;
				u_x = dx / (copy_p.Max.x - copy_p.Min.x);
				u_y = dy / (copy_p.Max.y - copy_p.Min.y);
			}
		}
		else if (captured_point == -2)
		{
			point_x = copy_p.Max.x;
			point_y = copy_p.Min.y;
			if (x < copy_p.Max.x&&y > copy_p.Min.y)
			{
				double dx = copy_p.Max.x - x;
				double dy = y - copy_p.Min.y;
				u_x = dx / (copy_p.Max.x - copy_p.Min.x);
				u_y = dy / (copy_p.Max.y - copy_p.Min.y);
			}
		}
		else if (captured_point == -3)
		{
			point_x = copy_p.Min.x;
			point_y = copy_p.Min.y;
			if (x > copy_p.Min.x&&y > copy_p.Min.y)
			{
				double dx = x - copy_p.Min.x;
				double dy = y - copy_p.Min.y;
				u_x = dx / (copy_p.Max.x - copy_p.Min.x);
				u_y = dy / (copy_p.Max.y - copy_p.Min.y);
			}
		}
		else if (captured_point == -4)
		{
			point_x = copy_p.Min.x;
			point_y = copy_p.Max.y;
			if (x > copy_p.Min.x&&y < copy_p.Max.y)
			{
				double dx = x - copy_p.Min.x;
				double dy = copy_p.Max.y - y;
				u_x = dx / (copy_p.Max.x - copy_p.Min.x);
				u_y = dy / (copy_p.Max.y - copy_p.Min.y);
			}
		}
		for (int t = 0; t < current_p.confirm_points; t++)
		{
			(it + t)->x = (it1 + t)->x*u_x + point_x*(1 - u_x);
			(it + t)->y = (it1 + t)->y*u_y + point_y*(1 - u_y);
		}
	}
}

void canvas::Transform_Bezier(int x, int y)
{
	Transform_polygon(x, y);
}

void canvas::Transform_Bspline(int x,int y)
{
	Transform_polygon(x, y);
}


void canvas::To_drawline()
{
	current_p.status = -1;
	current_p.refresh();
	update();
}

void canvas::To_drawpolygon()
{
	current_p.status = -2;
	current_p.refresh();
	update();
}

void canvas::To_draweclipse()
{
	current_p.status = -3;
	current_p.refresh();
	update();
}

void canvas::To_drawcurve_Bezier()
{
	current_p.status = -4;
	current_p.refresh();
	update();
}

void canvas::To_drawcurve_Bspline()
{
	current_p.status = -5;
	current_p.refresh();
	update();
}

void canvas::Revoke_current()
{
	switch (current_p.status)
	{
	case 1:
	case 2:
	case 30:
	case 31:
	case 40:
	case 41:current_p.status = -1; break;
	case 4:
	case 5:
	case 6:
	case 7:
	case 32:
	case 33:current_p.status = -2; break;
	case 9:
	case 10:
	case 34:
	case 35:current_p.status = -3; break;
	case 12:
	case 13:
	case 14:
	case 36:
	case 37:current_p.status = -4; break;
	case 16:
	case 17:
	case 18:
	case 38:
	case 39:current_p.status = -5; break;
	}
	current_p.refresh();
	update();
}


void canvas::Draw_background()
{
	QPainter p1(this);
	p1.setPen(Qt::NoPen);
	p1.setBrush(Qt::white);
	p1.drawRect(rect());//背景处理,函数处理
}

void canvas::Draw_past_primitives()//已绘制的图元
{
	vector<PaintStatus>::iterator i = vec.begin();
	for (; i != vec.end(); i++)
	{
		if (i->status == 3)//线段
		{
			pen.setColor(i->color);
			Paint p(this);
			p.setPen(pen);
			vector<Point>::iterator it = i->vec.begin();
			p.drawline_DDA(it->x, it->y, (it + 1)->x, (it + 1)->y);
		}
		else if (i->status == 8)//多边形
		{
			pen.setColor(i->color);
			Paint p(this);
			p.setPen(pen);
			vector<Point>::iterator it = i->vec.begin();
			int *x = new int[i->confirm_points];
			int *y = new int[i->confirm_points];
			for (int t = 0; t < i->confirm_points; t++)
			{
				x[t] = (it + t)->x;
				y[t] = (it + t)->y;
			}
			p.drawpolygon_DDA(i->confirm_points, x, y);
			delete[] x;
			delete[] y;
		}
		else if (i->status == 11)//圆
		{
			pen.setColor(i->color);
			Paint p(this);
			p.setPen(pen);
			vector<Point>::iterator it = i->vec.begin();
			double x1 = it->x;
			double y1 = it->y;
			double x2 = (it + 1)->x;
			double y2 = (it + 1)->y;
			int x = int((x1 + x2) / 2 + 0.5);
			int y = int((y1 + y2) / 2 + 0.5);//中心点
			int rx = int(abs(x1 - x2) / 2 + 0.5);
			int ry = int(abs(y1 - y2) / 2 + 0.5);
			p.drawellipse(x, y, rx, ry);
		}
		else if (i->status == 15)//贝塞尔曲线
		{
			pen.setColor(i->color);
			Paint p(this);
			p.setPen(pen);
			vector<Point>::iterator it = i->vec.begin();
			int *x = new int[i->confirm_points];
			int *y = new int[i->confirm_points];
			for (int t = 0; t < i->confirm_points; t++)
			{
				x[t] = (it + t)->x;
				y[t] = (it + t)->y;
			}
			p.drawcurse_Bezier(i->confirm_points, x, y);
			delete[] x;
			delete[] y;
		}
		else if (i->status == 19)
		{
			pen.setColor(i->color);
			Paint p(this);
			p.setPen(pen);
			vector<Point>::iterator it = i->vec.begin();
			int *x = new int[i->confirm_points];
			int *y = new int[i->confirm_points];
			for (int t = 0; t < i->confirm_points; t++)
			{
				x[t] = (it + t)->x;
				y[t] = (it + t)->y;
			}
			p.drawcurse_B_spline(i->confirm_points, x, y);
			delete[] x;
			delete[] y;
		}
	}
}

void canvas::Draw_control_lucos()
{
	vector<Point>::iterator it = current_p.vec.begin();
	QPen pen1;       //画外围控制线
	pen1.setColor(QColor(255, 0, 0));
	pen1.setWidth(5);
	QPainter p1(this);
	p1.setPen(pen1);
	for (int t = 0; t < current_p.confirm_points; t++)
	{
		p1.drawPoint((it + t)->x, (it + t)->y);
	}
	pen1.setStyle(Qt::DashLine);
	pen1.setWidth(0);
	pen1.setColor(QColor(0, 255, 0));
	p1.setPen(pen1);
	for (int t = 0; t < current_p.confirm_points - 1; t++)
	{
		p1.drawLine((it + t)->x, (it + t)->y, (it + t + 1)->x, (it + t + 1)->y);
	}
}

void canvas::Draw_control_points()
{
	vector<Point>::iterator it = current_p.vec.begin();
	QPainter p1(this);
	QPen pen1;
	pen1.setColor(QColor(255, 0, 0));
	pen1.setWidth(5);
	p1.setPen(pen1);
	p1.drawPoint(it->x, it->y);
	p1.drawPoint((it + 1)->x, (it + 1)->y);
	for (int i = 0; i < current_p.confirm_points; i++)
	{
		p1.drawPoint((it+i)->x, (it+i)->y);
	}
}

void canvas::Draw_control_rect()
{
	QPainter p1(this);
	QPen pen1;
	pen1.setColor(QColor(255, 201, 14));
	pen1.setWidth(5);
	p1.setPen(pen1);
	current_p.calculate_area();
	p1.drawPoint(current_p.Min.x, current_p.Min.y);
	p1.drawPoint(current_p.Min.x, current_p.Max.y);
	p1.drawPoint(current_p.Max.x, current_p.Min.y);
	p1.drawPoint(current_p.Max.x, current_p.Max.y);

	pen1.setColor(QColor(0, 0, 255));
	pen1.setStyle(Qt::DashLine);
	pen1.setWidth(0);
	p1.setPen(pen1);
	p1.drawLine(current_p.Min.x, current_p.Min.y, current_p.Min.x, current_p.Max.y);
	p1.drawLine(current_p.Min.x, current_p.Max.y, current_p.Max.x, current_p.Max.y);
	p1.drawLine(current_p.Max.x, current_p.Max.y, current_p.Max.x, current_p.Min.y);
	p1.drawLine(current_p.Max.x, current_p.Min.y, current_p.Min.x, current_p.Min.y);
}

void canvas::Draw_control_area()
{
	if (current_p.status == 30||current_p.status==31)//或者
	{
		Draw_control_points();
	}
	else if (current_p.status == 32||current_p.status==33)
	{
		Draw_control_rect();
		Draw_control_points();
	}
	else if (current_p.status == 34||current_p.status==35)
	{
		Draw_control_rect();
		//Draw_control_points();
	}
	else if (current_p.status == 36||current_p.status==37)
	{
		Draw_control_rect();
		Draw_control_lucos();
	}
	else if (current_p.status == 38||current_p.status==39)
	{
		Draw_control_rect();
		Draw_control_lucos();
	}
}

void canvas::Draw_half_primitives()
{
	if (current_p.status == 2)
	{
		pen.setColor(current_p.color);
		Paint p(this);
		p.setPen(pen);
		vector<Point>::iterator it = current_p.vec.begin();
		p.drawline_DDA(it->x, it->y, (it + 1)->x, (it + 1)->y);
		//vec.push_back(current_p);
	}
	else if (current_p.status == 5)
	{
		pen.setColor(current_p.color);
		Paint p(this);
		p.setPen(pen);
		vector<Point>::iterator it = current_p.vec.begin();
		p.drawline_DDA(it->x, it->y, (it + 1)->x, (it + 1)->y);
	}
	else if (current_p.status == 6)
	{
		pen.setColor(current_p.color);
		Paint p(this);
		p.setPen(pen);
		vector<Point>::iterator it = current_p.vec.begin();
		for (int i = 0; i < current_p.num - 1; i++)
			p.drawline_DDA((it + i)->x, (it + i)->y, (it + i + 1)->x, (it + i + 1)->y);
	}
	else if (current_p.status == 10)
	{
		pen.setColor(current_p.color);
		Paint p(this);
		p.setPen(pen);
		vector<Point>::iterator it = current_p.vec.begin();
		double x1 = it->x;
		double y1 = it->y;
		double x2 = (it + 1)->x;
		double y2 = (it + 1)->y;
		int x = int((x1 + x2) / 2 + 0.5);
		int y = int((y1 + y2) / 2 + 0.5);//中心点
		int rx = int(abs(x1 - x2) / 2 + 0.5);
		int ry = int(abs(y1 - y2) / 2 + 0.5);
		p.drawellipse(x, y, rx, ry);
	}
	else if (current_p.status == 13)//贝塞尔曲线
	{
		pen.setColor(current_p.color);
		Paint p(this);
		p.setPen(pen);
		vector<Point>::iterator it = current_p.vec.begin();
		int *x = new int[current_p.confirm_points];
		int *y = new int[current_p.confirm_points];
		for (int t = 0; t < current_p.confirm_points; t++)
		{
			x[t] = (it + t)->x;
			y[t] = (it + t)->y;
		}
		p.drawcurse_Bezier(current_p.confirm_points, x, y);
		delete[] x;
		delete[] y;

		Draw_control_lucos();
	}
	else if (current_p.status == 17)//三次B样条曲线
	{
		vector<Point>::iterator it = current_p.vec.begin();
		if (current_p.confirm_points > 3)//至少四个点
		{
			pen.setColor(current_p.color);
			Paint p(this);
			p.setPen(pen);
			int *x = new int[current_p.confirm_points];
			int *y = new int[current_p.confirm_points];
			for (int t = 0; t < current_p.confirm_points; t++)
			{
				x[t] = (it + t)->x;
				y[t] = (it + t)->y;
			}
			p.drawcurse_B_spline(current_p.confirm_points, x, y);
			delete[] x;
			delete[] y;
		}
		Draw_control_lucos();
	}
}

void canvas::Draw_unconfirmed_primitives()
{
	Draw_control_area();

	if (current_p.status == 30||current_p.status==31)
	{
		pen.setColor(current_p.color);
		Paint p(this);
		p.setPen(pen);
		vector<Point>::iterator it = current_p.vec.begin();
		p.drawline_DDA(it->x, it->y, (it + 1)->x, (it + 1)->y);
	}
	else if (current_p.status == 32||current_p.status==33)
	{
		pen.setColor(current_p.color);
		Paint p(this);
		p.setPen(pen);
		vector<Point>::iterator it = current_p.vec.begin();
		int *x = new int[current_p.confirm_points];
		int *y = new int[current_p.confirm_points];
		for (int t = 0; t < current_p.confirm_points; t++)
		{
			x[t] = (it + t)->x;
			y[t] = (it + t)->y;
		}
		p.drawpolygon_DDA(current_p.confirm_points, x, y);
		delete[] x;
		delete[] y;
	}
	else if (current_p.status == 34||current_p.status==35)
	{
		pen.setColor(current_p.color);
		Paint p(this);
		p.setPen(pen);
		vector<Point>::iterator it = current_p.vec.begin();
		double x1 = it->x;
		double y1 = it->y;
		double x2 = (it + 1)->x;
		double y2 = (it + 1)->y;
		int x = int((x1 + x2) / 2 + 0.5);
		int y = int((y1 + y2) / 2 + 0.5);//中心点
		int rx = int(abs(x1 - x2) / 2 + 0.5);
		int ry = int(abs(y1 - y2) / 2 + 0.5);
		p.drawellipse(x, y, rx, ry);
	}
	else if (current_p.status == 36||current_p.status==37)
	{
		pen.setColor(current_p.color);
		Paint p(this);
		p.setPen(pen);
		vector<Point>::iterator it = current_p.vec.begin();
		int *x = new int[current_p.confirm_points];
		int *y = new int[current_p.confirm_points];
		for (int t = 0; t < current_p.confirm_points; t++)
		{
			x[t] = (it + t)->x;
			y[t] = (it + t)->y;
		}
		p.drawcurse_Bezier(current_p.confirm_points, x, y);
		delete[] x;
		delete[] y;
	}
	else if (current_p.status == 38||current_p.status==39)
	{
		pen.setColor(current_p.color);
		Paint p(this);
		p.setPen(pen);
		vector<Point>::iterator it = current_p.vec.begin();
		int *x = new int[current_p.confirm_points];
		int *y = new int[current_p.confirm_points];
		for (int t = 0; t < current_p.confirm_points; t++)
		{
			x[t] = (it + t)->x;
			y[t] = (it + t)->y;
		}
		p.drawcurse_B_spline(current_p.confirm_points, x, y);
		delete[] x;
		delete[] y;
	}
	else if (current_p.status == 41)
	{
		pen.setColor(current_p.color);
		Paint p(this);
		p.setPen(pen);
		vector<Point>::iterator it = current_p.vec.begin();
		p.drawline_DDA(it->x, it->y, (it + 1)->x, (it + 1)->y);
		QPen pen1;       //画外围控制线
		QPainter p1(this);
		pen1.setColor(QColor(255, 0, 128));
		pen1.setStyle(Qt::DashLine);
		pen1.setWidth(0);
		p1.setPen(pen1);
		p1.drawLine(clip_point1.x, clip_point1.y, clip_point1.x, clip_point2.y);
		p1.drawLine(clip_point1.x, clip_point2.y, clip_point2.x, clip_point2.y);
		p1.drawLine(clip_point2.x, clip_point2.y, clip_point2.x, clip_point1.y);
		p1.drawLine(clip_point2.x, clip_point1.y, clip_point1.x, clip_point1.y);

	}
}

void canvas::Color_change(QColor c)
{
	current_p.color = c;
	update();
}

void canvas::paintEvent(QPaintEvent *)
{
	Draw_background();//画背景

	Draw_past_primitives();//画已经绘制的图元

	if (current_p.status < 20)
		Draw_half_primitives();
	else
		Draw_unconfirmed_primitives();
}


void canvas::mousePressEvent(QMouseEvent *ev)
{
	if(ev->button()==Qt::LeftButton)
	{
		if (current_p.status == -1)//划线按下第一个点
		{
			current_p.add_point(ev->x(), ev->y());
			current_p.add_one_comfirm_point();
			current_p.status = 1;
		}
		else if (current_p.status == -2)//
		{
			current_p.add_point(ev->x(), ev->y());
			current_p.add_one_comfirm_point();
			current_p.status = 4;
		}
		else if (current_p.status == -3)
		{
			current_p.add_point(ev->x(), ev->y());
			current_p.add_one_comfirm_point();
			current_p.status = 9;
		}
		else if (current_p.status == -4)
		{
			current_p.status = 12;
		}
		else if (current_p.status == -5)
		{
			current_p.status = 16;
		}
		else if (current_p.status == 6)
		{
			current_p.status = 7;
		}
		else if (current_p.status == 13)
		{
			current_p.status = 14;
		}
		else if (current_p.status == 17)
		{
			current_p.status = 18;
		}
		else if (current_p.status == 30)
		{
			if (transform_mode == 2)
				setCursor(Qt::ClosedHandCursor);
			copy_status();
			reference_point.x = ev->x();
			reference_point.y = ev->y();
			current_p.status = 31;
		}
		else if (current_p.status == 32)
		{
			if (transform_mode == 2)
				setCursor(Qt::ClosedHandCursor);
			copy_status();
			reference_point.x = ev->x();
			reference_point.y = ev->y();
			current_p.status = 33;
		}
		else if (current_p.status == 34)
		{
			if (transform_mode == 2)
				setCursor(Qt::ClosedHandCursor);
			copy_status();
			reference_point.x = ev->x();
			reference_point.y = ev->y();
			current_p.status = 35;
		}
		else if (current_p.status == 36)
		{
			if (transform_mode == 2)
				setCursor(Qt::ClosedHandCursor);
			copy_status();
			reference_point.x = ev->x();
			reference_point.y = ev->y();
			current_p.status = 37;
		}
		else if (current_p.status == 38)
		{
			if (transform_mode == 2)
				setCursor(Qt::ClosedHandCursor);
			copy_status();
			reference_point.x = ev->x();
			reference_point.y = ev->y();
			current_p.status = 39;
		}
		else if (current_p.status == 40)
		{
			clip_point1.x = ev->x();
			clip_point1.y = ev->y();
			current_p.status = 41;
		}
	}
	else if(ev->button() == Qt::RightButton)//右键按下
	{
		if (current_p.status == 6 || current_p.status == 7)
		{
			current_p.status = 32;
			update();
		}
		else if (current_p.status == 13)
		{
			current_p.status = 36;
			update();
		}
		else if (current_p.status == 17)
		{
			if (current_p.confirm_points > 3)
			{
				current_p.status = 38;
				update();
			}
		}
		else if (current_p.status == 30)
		{
			setCursor(Qt::ArrowCursor);
			current_p.status = 3;
			vec.push_back(current_p);
			current_p.status = -1;
			current_p.refresh();
			update();
		}
		else if (current_p.status == 32)
		{
			setCursor(Qt::ArrowCursor);
			current_p.status = 8;
			vec.push_back(current_p);
			current_p.status = -2;
			current_p.refresh();
			update();
		}
		else if (current_p.status == 34)
		{
			setCursor(Qt::ArrowCursor);
			current_p.status = 11;
			vec.push_back(current_p);
			current_p.status = -3;
			current_p.refresh();
			update();
		}
		else if (current_p.status == 36)
		{
			setCursor(Qt::ArrowCursor);
			current_p.status = 15;
			vec.push_back(current_p);
			current_p.status = -4;
			current_p.refresh();
			update();
		}
		else if (current_p.status == 38)
		{
			setCursor(Qt::ArrowCursor);
			current_p.status = 19;
			vec.push_back(current_p);
			current_p.status = -5;
			current_p.refresh();
			update();
		}
	}
}


void canvas::mouseMoveEvent(QMouseEvent *ev)
{
	if (current_p.status == 1|| current_p.status == 2)//拖动状态
	{
		if (current_p.num == 1)
			current_p.add_point(ev->x(), ev->y());
		else
			current_p.update_point(1, ev->x(), ev->y());
		current_p.status = 2;
		update();
		//current_p.status = 1;
	}
	else if (current_p.status == 4|| current_p.status == 5)//拖动状态
	{
		if (current_p.num == current_p.confirm_points)
			current_p.add_point(ev->x(), ev->y());
		else
			current_p.update_point(current_p.confirm_points, ev->x(), ev->y());
		current_p.status = 5;
		update();
	}
	else if (current_p.status == 6)
	{
		if (current_p.num == current_p.confirm_points)
			current_p.add_point(ev->x(), ev->y());
		else
			current_p.update_point(current_p.confirm_points, ev->x(), ev->y());
		update();
	}
	else if (current_p.status == 7)
	{
		current_p.status = 6;
	}
	else if (current_p.status == 9 || current_p.status == 10)
	{
		if (current_p.num == 1)
			current_p.add_point(ev->x(), ev->y());
		else
			current_p.update_point(1, ev->x(), ev->y());
		current_p.status = 10;
		update();
	}
	else if (current_p.status == 12)
	{
		current_p.status = -4;
		current_p.refresh();
	}
	else if (current_p.status == 14)
	{
		current_p.status = 13;
	}
	else if (current_p.status == 16)
	{
		current_p.status = -5;
		current_p.refresh();
	}
	else if (current_p.status == 18)
	{
		current_p.status = 17;
	}
	else if (current_p.status == 30)//未确定状态下，改变鼠标形态
	{
		Capture_cursor_line(ev->x(),ev->y());
		if (transform_mode == 0)
			setCursor(Qt::ArrowCursor);
		else if (transform_mode == 1)
			setCursor(Qt::SizeAllCursor);
		else if (transform_mode == 2)
			setCursor(Qt::OpenHandCursor);
		else if (transform_mode == 4)
			setCursor(Qt::SizeVerCursor);
	}
	else if (current_p.status == 32)
	{
		Capture_cursor_polygon(ev->x(), ev->y());
		if (transform_mode == 0)
			setCursor(Qt::ArrowCursor);
		else if (transform_mode == 1)
			setCursor(Qt::SizeAllCursor);
		else if (transform_mode == 2)
			setCursor(Qt::OpenHandCursor);
		else if (transform_mode == 3)
		{
			if (captured_point == -1 || captured_point == -3)
				setCursor(Qt::SizeFDiagCursor);
			else if (captured_point == -2 || captured_point == -4)
				setCursor(Qt::SizeBDiagCursor);
		}
		else if (transform_mode == 4)
			setCursor(Qt::SizeVerCursor);
	}
	else if (current_p.status == 34)
	{
		Capture_cursor_eclipse(ev->x(), ev->y());
		if (transform_mode == 0)
			setCursor(Qt::ArrowCursor);
		else if (transform_mode == 1)
			setCursor(Qt::SizeAllCursor);
		else if (transform_mode == 3)
		{
			if (captured_point == -1 || captured_point == -3)
				setCursor(Qt::SizeFDiagCursor);
			else if (captured_point == -2 || captured_point == -4)
				setCursor(Qt::SizeBDiagCursor);
		}
	}
	else if (current_p.status == 36)
	{
		Capture_cursor_Bezier(ev->x(), ev->y());
		if (transform_mode == 0)
			setCursor(Qt::ArrowCursor);
		else if (transform_mode == 1)
			setCursor(Qt::SizeAllCursor);
		else if (transform_mode == 2)
			setCursor(Qt::OpenHandCursor);
		else if (transform_mode == 3)
		{
			if (captured_point == -1 || captured_point == -3)
				setCursor(Qt::SizeFDiagCursor);
			else if (captured_point == -2 || captured_point == -4)
				setCursor(Qt::SizeBDiagCursor);
		}
		else if (transform_mode == 4)
			setCursor(Qt::SizeVerCursor);
	}
	else if (current_p.status == 38)
	{
		Capture_cursor_Bspline(ev->x(), ev->y());
		if (transform_mode == 0)
			setCursor(Qt::ArrowCursor);
		else if (transform_mode == 1)
			setCursor(Qt::SizeAllCursor);
		else if (transform_mode == 2)
			setCursor(Qt::OpenHandCursor);
		else if (transform_mode == 3)
		{
			if (captured_point == -1 || captured_point == -3)
				setCursor(Qt::SizeFDiagCursor);
			else if (captured_point == -2 || captured_point == -4)
				setCursor(Qt::SizeBDiagCursor);
		}
		else if (transform_mode == 4)
			setCursor(Qt::SizeVerCursor);
	}
	else if (current_p.status == 31)//变换状态按下之后
	{
		Transform_line(ev->x(), ev->y());
		update();
	}
	else if (current_p.status == 33)
	{
		Transform_polygon(ev->x(), ev->y());
		update();
	}
	else if (current_p.status == 35)
	{
		Transform_eclipse(ev->x(), ev->y());
		update();
	}
	else if (current_p.status == 37)
	{
		Transform_Bezier(ev->x(), ev->y());
		update();
	}
	else if (current_p.status == 39)
	{
		Transform_Bspline(ev->x(), ev->y());
		update();
	}
	else if (current_p.status == 41)
	{
		clip_point2.x = ev->x();
		clip_point2.y = ev->y();
		update();
	}
}


void canvas::mouseReleaseEvent(QMouseEvent *ev)
{
	if (ev->button() == Qt::LeftButton)
	{
		if (current_p.status == 1)//回归初始状态
		{
			current_p.status = -1;
			current_p.refresh();
		}
		else if (current_p.status == 2)
		{
			current_p.status = 30;
			current_p.add_one_comfirm_point();
			update();
		}
		else if (current_p.status == 4)//回归初始状态
		{
			current_p.status = -2;
			current_p.refresh();
		}
		else if (current_p.status == 5)
		{
			current_p.add_one_comfirm_point();
			current_p.status = 6;
		}
		else if (current_p.status == 7)
		{
			current_p.add_one_comfirm_point();
			current_p.status = 6;
		}
		else if (current_p.status == 9)//回归初始状态
		{
			current_p.status = -3;
			current_p.refresh();
		}
		else if (current_p.status == 10)
		{
			current_p.status = 34;
			current_p.add_one_comfirm_point();
			update();
		}
		else if (current_p.status == 12)
		{
			current_p.add_point(ev->x(), ev->y());
			current_p.add_one_comfirm_point();
			current_p.status = 13;
			update();
		}
		else if (current_p.status == 14)
		{
			current_p.add_point(ev->x(), ev->y());
			current_p.add_one_comfirm_point();
			current_p.status = 13;
			update();
		}
		else if (current_p.status == 16)
		{
			current_p.add_point(ev->x(), ev->y());
			current_p.add_one_comfirm_point();
			current_p.status = 17;
			update();
		}
		else if (current_p.status == 18)
		{
			current_p.add_point(ev->x(), ev->y());
			current_p.add_one_comfirm_point();
			current_p.status = 17;
			update();
		}
		else if (current_p.status == 31)
		{
			current_p.status = 30;
		}
		else if (current_p.status == 33)
		{
			current_p.status = 32;
		}
		else if (current_p.status == 35)
		{
			current_p.status = 34;
		}
		else if (current_p.status == 37)
		{
			current_p.status = 36;
		}
		else if (current_p.status == 39)
		{
			current_p.status = 38;
		}
		else if (current_p.status == 41)
		{
			Do_the_clip();
		}
	}
	else if (ev->button() == Qt::RightButton)
	{

	}
}