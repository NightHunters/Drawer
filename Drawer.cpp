#include "Drawer.h"
#include "Paint.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{

	resize(QSize(800, 600));//���ô��ڴ�С
	/*QPixmap pixmap(108 * 2, 192 * 2 - 100);
	//pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);  //QPainter����
	painter.drawLine(QPointF(0, 0), QPointF(100, 100)); //ע�⣺��������������������Ǵ��ڵ����
	pixmap.save("2.bmp");*/
}

MainWindow::~MainWindow()
{
	
}
void MainWindow::paintEvent(QPaintEvent *)
{
	QPixmap pixmap(400, 200);
	pixmap.fill(QColor(238, 63, 77, 255));//R,G,Bֵ�����һλ255��ʾ��͸����127��͸����0͸��
	QPainter painter(&pixmap);  //QPainter����
	QPen pen;
	Paint p(&painter);
	p.drawline_DDA(20, 20, 100, 100);
	pixmap.save("2.bmp");
}
