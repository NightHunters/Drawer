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
	void paintEvent(QPaintEvent *); //�ػ��¼�������������: ���еĻ��Ʋ�����Ҫ���������������ɡ�
};

#endif // MAINWINDOW_H