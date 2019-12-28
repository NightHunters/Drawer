#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Drawer.h"
#include <QGraphicsDropShadowEffect>
#include <QFile>
#include <QFileDialog>
#include <Qpixmap>
#include <Qpen>
#include"Paint.h"
#define CANVAS_WIDTH_MIN 100
#define CANVAS_WIDTH_MAX 980
#define CANVAS_HEIGHT_MIN 100
#define CANVAS_HEIGHT_MAX 600
#define WINDOWS_WIDTH 1000
#define WINDOWS_HEIGHT 800

class Drawer : public QMainWindow
{
	Q_OBJECT

public:
	Drawer(QWidget *parent = Q_NULLPTR);
	//void show_pen_color();
	void save_file();
	void reset();
	void Color_red();
	void Color_green();
	void Color_blue();
	void Color_orange();
	void Color_purple();
	void Color_yellow();
	void Color_random(int value);
protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *ev);	//Êó±êÊÂ¼þ
	void mouseMoveEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
private:
	Ui::DrawerClass *ui;
	//bool Change_canvas_size;
	//QColor canvas_color;
	int To_change_canvas;
	Point Origin_point_canvas;
	int Max_canvas_x;
	int Max_canvas_y;
};
