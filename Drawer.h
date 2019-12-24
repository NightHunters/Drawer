#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Drawer.h"
#include <QGraphicsDropShadowEffect>
#include <QFile>
#include <QFileDialog>
#include <Qpixmap>
#include <Qpen>
#include"Paint.h"

class Drawer : public QMainWindow
{
	Q_OBJECT

public:
	Drawer(QWidget *parent = Q_NULLPTR);
protected:
	void paintEvent(QPaintEvent *);
private:
	Ui::DrawerClass *ui;
};
