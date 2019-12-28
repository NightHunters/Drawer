#pragma once

#include <QWidget>
#include <Qpen>
#include <QPainter>
#include <QMouseEvent>

class Color_label : public QWidget
{
	Q_OBJECT
private:
	QColor color;
protected:
	void paintEvent(QPaintEvent *ev);
public:
	Color_label(QWidget *parent = nullptr);
	~Color_label();
	void paint_background();
	void update_color(QColor col);
};
