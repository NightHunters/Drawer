#include "canvas.h"

canvas::canvas(QWidget *parent) :QWidget(parent)
{
	setMouseTracking(true);
}

canvas::~canvas()
{
	
}

void canvas::Draw_background()
{
	QPainter p1(this);
	p1.setPen(Qt::NoPen);
	p1.setBrush(Qt::white);
	p1.drawRect(rect());//背景处理,函数处理
}

void canvas::Draw_past_primitives()
{
	vector<PaintStatus>::iterator i = vec.begin();//将来用函数实现
	for (; i != vec.end(); i++)
	{
		if (i->status == 3)
		{
			pen.setColor(i->color);
			Paint p(this);
			p.setPen(pen);
			vector<Point>::iterator it = i->vec.begin();
			p.drawline_DDA(it->x, it->y, (it + 1)->x, (it + 1)->y);
		}
		else if (i->status == 8)
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
			p.drawpolygon_DDA(i->confirm_points,x,y);
			delete[] x;
			delete[] y;
		}
	}
}

void canvas::To_drawline()
{
	current_p.status = -1;
	current_p.refresh();
}

void canvas::To_drawpolygon()
{
	current_p.status = -2;
	current_p.refresh();
}

void canvas::Color_bule()
{
	current_p.color.setRgb(0, 0, 255);
}

void canvas::paintEvent(QPaintEvent *)
{
	Draw_background();

	Draw_past_primitives();

	if (current_p.status==2)
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
		else if (current_p.status == -2)
		{
			if (current_p.num == 0)
				current_p.add_point(ev->x(), ev->y());
			current_p.add_one_comfirm_point();
			current_p.status = 4;
		}
		else if (current_p.status == 6)
		{
			current_p.status = 7;
		}
	}
	else if(ev->button() == Qt::RightButton)//右键按下
	{
		if (current_p.status == 6 || current_p.status == 7)
		{
			current_p.status = 8;
			//current_p.add_one_comfirm_point();
			vec.push_back(current_p);
			current_p.status = -2;
			current_p.refresh();
			update();
		}
	}
}


void canvas::mouseMoveEvent(QMouseEvent *ev)
{
	if (current_p.status == 1|| current_p.status == 2)
	{
		if (current_p.num == 1)
			current_p.add_point(ev->x(), ev->y());
		else
			current_p.update_point(1, ev->x(), ev->y());
		current_p.status = 2;
		update();
		//current_p.status = 1;
	}
	else if (current_p.status == 4|| current_p.status == 5)
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
}


void canvas::mouseReleaseEvent(QMouseEvent *ev)
{
	if (ev->button() == Qt::LeftButton)
	{
		if (current_p.status == 1)
		{
			current_p.status = -1;
			current_p.refresh();
		}
		else if (current_p.status == 2)
		{
			current_p.status = 3;
			current_p.add_one_comfirm_point();
			vec.push_back(current_p);
			current_p.status = -1;
			current_p.refresh();
		}
		else if (current_p.status == 4)
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
	}
	else if (ev->button() == Qt::RightButton)
	{

	}
}