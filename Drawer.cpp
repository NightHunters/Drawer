#include "Drawer.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	resize(QSize(800, 600));
}

MainWindow::~MainWindow()
{
	
}
void MainWindow::paintEvent(QPaintEvent *)
{
	QPainter painter(this);  //QPainter����
	painter.drawLine(QPointF(0, 0), QPointF(100, 100)); //ע�⣺��������������������Ǵ��ڵ����
}
