#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Drawer.h"
#include <QGraphicsDropShadowEffect>
#include <QFile>
#include <QFileDialog>
#include <Qpixmap>
#include <Qpen>
#include"Paint.h"
#define CANVAS_LIMIT_MIN 100
#define CANVAS_LIMIT_MAX 600

class Drawer : public QMainWindow
{
	Q_OBJECT

public:
	Drawer(QWidget *parent = Q_NULLPTR);
protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *ev);	//Êó±êÊÂ¼þ
	void mouseMoveEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
private:
	Ui::DrawerClass *ui;
	//bool Change_canvas_size;
	int To_change_canvas;
	Point Origin_point_canvas;
	int Max_canvas_x;
	int Max_canvas_y;
};
