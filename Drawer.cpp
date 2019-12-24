#include "Drawer.h"

Drawer::Drawer(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::DrawerClass)
{
	ui->setupUi(this);
	setFixedSize(1000,800);//�̶���С
	/*QPalette palette(ui->myCanvas->palette());//�ı䱳��ɫ
	palette.setColor(QPalette::Background, Qt::white);
	ui->myCanvas->setPalette(palette);*/

	QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();//���
	shadow->setColor(Qt::black);
	shadow->setBlurRadius(8);
	shadow->setOffset(0);
	ui->Canvas->setGraphicsEffect(shadow);

	/*QPixmap pix = QPixmap::grabWidget(ui->myCanvas);//����ͼƬ
	QString str = "1.bmp";
	QString fileName = QFileDialog::getSaveFileName(this, "����ͼƬ", str, "PNG (*.png);;BMP (*.bmp);;JPEG (*.jpg *.jpeg)");
	if (!fileName.isNull())
	{
		pix.save(fileName);
	}*/
	//this->setStyleSheet("background-color: rgb(255, 255, 255);");
	connect(ui->Line_Button, &QToolButton::clicked, ui->Canvas, &canvas::To_drawline);//�ź�
	connect(ui->Polygon_Button, &QToolButton::clicked, ui->Canvas, &canvas::To_drawpolygon);
	connect(ui->Bule_Color, &QToolButton::clicked, ui->Canvas, &canvas::Color_bule);
}
void Drawer::paintEvent(QPaintEvent *)
{
}


