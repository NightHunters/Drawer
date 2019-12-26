#include "Drawer.h"

Drawer::Drawer(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::DrawerClass)
{
	ui->setupUi(this);
	setFixedSize(1000,800);//固定大小
	/*QPalette palette(ui->myCanvas->palette());//改变背景色
	palette.setColor(QPalette::Background, Qt::white);
	ui->myCanvas->setPalette(palette);*/
	To_change_canvas = 0;
	Origin_point_canvas.x = 10;
	Origin_point_canvas.y = 150;
	Max_canvas_x = 400;
	Max_canvas_y = 400;
	ui->Canvas->setFixedSize(Max_canvas_x, Max_canvas_y);

	QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();//添加
	shadow->setColor(Qt::black);
	shadow->setBlurRadius(8);
	shadow->setOffset(0);
	ui->Canvas->setGraphicsEffect(shadow);

	/*QPixmap pix = QPixmap::grabWidget(ui->myCanvas);//保存图片
	QString str = "1.bmp";
	QString fileName = QFileDialog::getSaveFileName(this, "保存图片", str, "PNG (*.png);;BMP (*.bmp);;JPEG (*.jpg *.jpeg)");
	if (!fileName.isNull())
	{
		pix.save(fileName);
	}*/
	//this->setStyleSheet("background-color: rgb(255, 255, 255);");
	connect(ui->Line_Button, &QToolButton::clicked, ui->Canvas, &canvas::To_drawline);//信号与槽
	connect(ui->Polygon_Button, &QToolButton::clicked, ui->Canvas, &canvas::To_drawpolygon);
	connect(ui->Eclipse_Button, &QToolButton::clicked, ui->Canvas, &canvas::To_draweclipse);
	connect(ui->Bezier_Button, &QToolButton::clicked, ui->Canvas, &canvas::To_drawcurve_Bezier);
	connect(ui->Bspline_Button, &QToolButton::clicked, ui->Canvas, &canvas::To_drawcurve_Bspline);
	connect(ui->Revoke_Button, &QToolButton::clicked, ui->Canvas, &canvas::Revoke_current);

	connect(ui->Bule_Color, &QToolButton::clicked, ui->Canvas, &canvas::Color_bule);
	ui->centralWidget->setMouseTracking(true);
	setMouseTracking(true);
}
void Drawer::paintEvent(QPaintEvent *)
{

}

void Drawer::mousePressEvent(QMouseEvent *ev)
{
	if (ev->button() == Qt::LeftButton)
	{
		if (To_change_canvas == 1)
		{
			To_change_canvas = 2;
		}
	}
}

void Drawer::mouseMoveEvent(QMouseEvent *ev)
{
	if (To_change_canvas == 0||To_change_canvas==1)
	{
		if (ui->Canvas->is_able_change())
		{
			int dx = ev->x() - Origin_point_canvas.x - Max_canvas_x;
			int dy = ev->y() - Origin_point_canvas.y - Max_canvas_y;
			if (!(dx<0 && dy<0)&&abs(dx)<6&&abs(dy)<6)
			{
				setCursor(Qt::SizeFDiagCursor);
				To_change_canvas = 1;
			}
			else
			{
				setCursor(Qt::ArrowCursor);
				To_change_canvas = 0;
			}
		}
		else
		{
			setCursor(Qt::ArrowCursor);
			To_change_canvas = 0;
		}
	}
	else if (To_change_canvas == 2 || To_change_canvas == 3)
	{
		int dx = ev->x() - Origin_point_canvas.x;
		int dy = ev->y() - Origin_point_canvas.y;
		if (dx < CANVAS_LIMIT_MIN)
			dx = CANVAS_LIMIT_MIN;
		else if (dx > CANVAS_LIMIT_MAX)
			dx = CANVAS_LIMIT_MAX;
		if (dy < CANVAS_LIMIT_MIN)
			dy = CANVAS_LIMIT_MIN;
		else if (dy > CANVAS_LIMIT_MAX)
			dy = CANVAS_LIMIT_MAX;
		To_change_canvas = 3;
		ui->Canvas->setFixedSize(dx, dy);
		Max_canvas_x = dx;
		Max_canvas_y = dy;
	}
}
void Drawer::mouseReleaseEvent(QMouseEvent *ev) 
{
	if (ev->button() == Qt::LeftButton)
	{
		if (To_change_canvas == 2)
			To_change_canvas = 0;
		else if (To_change_canvas == 3)
			To_change_canvas = 0;
	}
}
