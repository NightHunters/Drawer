#pragma once

#include <QWidget>
#include <Qpen>
#include <Qpixmap>
#include <QMouseEvent>
#include"Paint.h"
#include"PaintStatus.h"

class canvas : public QWidget
{
	Q_OBJECT
private:
	vector<PaintStatus> vec;
	PaintStatus current_p;
	//QColor color;
	QPen pen;
	//bool L_button;
protected:
	void paintEvent(QPaintEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
public:
	canvas(QWidget *parent = nullptr);
	~canvas();
	void Draw_background();
	void Draw_past_primitives();
	void To_drawline();//划线
	void To_drawpolygon();//画多边形
	void Color_bule();//蓝色
};
