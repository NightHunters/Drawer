#include "Drawer.h"
#include "Paint.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{

	resize(QSize(800, 600));//设置窗口大小
	/*QPixmap pixmap(108 * 2, 192 * 2 - 100);
	//pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);  //QPainter对象
	painter.drawLine(QPointF(0, 0), QPointF(100, 100)); //注意：不包括工具栏，坐标就是窗口的起点
	pixmap.save("2.bmp");*/
}

MainWindow::~MainWindow()
{
	
}
void MainWindow::paintEvent(QPaintEvent *)
{
	QPixmap pixmap(400, 200);
	pixmap.fill(QColor(238, 63, 77, 255));//R,G,B值，最后一位255表示不透明，127半透明，0透明
	QPainter painter(&pixmap);  //QPainter对象
	QPen pen;
	Paint p(&painter);
	p.drawline_DDA(20, 20, 100, 100);
	pixmap.save("2.bmp");
}
