#include "Color_label.h"

Color_label::Color_label(QWidget *parent) :QWidget(parent),color(0,0,0)
{

}

Color_label::~Color_label()
{

}
void Color_label::paint_background()
{
	QPainter p1(this);
	p1.setPen(Qt::NoPen);
	p1.setBrush(color);
	p1.drawRect(rect());
}

void Color_label::update_color(QColor col)
{
	color = col;
	update();
}
void Color_label::paintEvent(QPaintEvent *ev)
{
	paint_background();
}