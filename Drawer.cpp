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
	QPainter painter(this);  //QPainter对象
	painter.drawLine(QPointF(0, 0), QPointF(100, 100)); //注意：不包括工具栏，坐标就是窗口的起点
}
