#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <Qpixmap>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
protected:
	void paintEvent(QPaintEvent *); //重绘事件处理函数的声明: 所有的绘制操作都要在这个函数里面完成。
};

#endif // MAINWINDOW_H