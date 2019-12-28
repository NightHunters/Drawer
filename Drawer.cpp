#include "Drawer.h"

Drawer::Drawer(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::DrawerClass)
{
	ui->setupUi(this);
	setFixedSize(WINDOWS_WIDTH,WINDOWS_HEIGHT);//固定大小
	/*QPalette palette(ui->myCanvas->palette());//改变背景色
	palette.setColor(QPalette::Background, Qt::white);
	ui->myCanvas->setPalette(palette);*/
	To_change_canvas = 0;
	Origin_point_canvas.x = 10;
	Origin_point_canvas.y = 150;
	Max_canvas_x = 400;
	Max_canvas_y = 400;
	ui->Canvas->move(Origin_point_canvas.x, Origin_point_canvas.y);
	ui->Canvas->setFixedSize(Max_canvas_x, Max_canvas_y);

	ui->R_Box->setValue(0);
	ui->G_Box->setValue(0);
	ui->B_Box->setValue(0);

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

	connect(ui->Save_Button, &QToolButton::clicked, this, &Drawer::save_file);
	connect(ui->Reset_Button, &QToolButton::clicked, this,&Drawer::reset);

	connect(ui->R_Box, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->R_Color, &QSlider::setValue);
	connect(ui->R_Color, &QSlider::valueChanged, ui->R_Box, &QSpinBox::setValue);
	connect(ui->G_Box, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->G_Color, &QSlider::setValue);
	connect(ui->G_Color, &QSlider::valueChanged, ui->G_Box, &QSpinBox::setValue);
	connect(ui->B_Box, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->B_Color, &QSlider::setValue);
	connect(ui->B_Color, &QSlider::valueChanged, ui->B_Box, &QSpinBox::setValue);//关联滑块和数值

	connect(ui->Red_Color, &QToolButton::clicked, this, &Drawer::Color_red);
	connect(ui->Green_Color, &QToolButton::clicked, this, &Drawer::Color_green);
	connect(ui->Blue_Color, &QToolButton::clicked, this, &Drawer::Color_blue);
	connect(ui->Orange_Color, &QToolButton::clicked, this, &Drawer::Color_orange);
	connect(ui->Purple_Color, &QToolButton::clicked, this, &Drawer::Color_purple);
	connect(ui->Yellow_Color, &QToolButton::clicked, this, &Drawer::Color_yellow);

	connect(ui->R_Box, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Drawer::Color_random);
	connect(ui->G_Box, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Drawer::Color_random);
	connect(ui->B_Box, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Drawer::Color_random);

	ui->centralWidget->setMouseTracking(true);
	setMouseTracking(true);
}

void Drawer::paintEvent(QPaintEvent *)
{

}

void Drawer::save_file()
{
	QPixmap pix = QPixmap::grabWidget(ui->Canvas);//保存图片
	QString str = "output.bmp";
	QString fileName = QFileDialog::getSaveFileName(this, "保存图片", str, "BMP (*.bmp);;PNG (*.png);;JPEG (*.jpg *.jpeg)");
	if (!fileName.isNull())
	{
		pix.save(fileName);
	}
}

void Drawer::reset()
{
	ui->Canvas->reset();
	Max_canvas_x = 400;
	Max_canvas_y = 400;
	ui->Canvas->setFixedSize(Max_canvas_x, Max_canvas_y);
}

void Drawer::Color_red()
{
	QColor c(255, 0, 0);
	ui->Current_Color->update_color(c);
	ui->Canvas->Color_change(c);
	ui->R_Box->setValue(255);
	ui->G_Box->setValue(0);
	ui->B_Box->setValue(0);
}

void Drawer::Color_green()
{
	QColor c(0, 255, 0);
	ui->Current_Color->update_color(c);
	ui->Canvas->Color_change(c);
	ui->R_Box->setValue(0);
	ui->G_Box->setValue(255);
	ui->B_Box->setValue(0);
}

void Drawer::Color_blue()
{
	QColor c(0, 0, 255);
	ui->Current_Color->update_color(c);
	ui->Canvas->Color_change(c);
	ui->R_Box->setValue(0);
	ui->G_Box->setValue(0);
	ui->B_Box->setValue(255);
}

void Drawer::Color_orange()
{
	QColor c(255, 127, 39);
	ui->Current_Color->update_color(c);
	ui->Canvas->Color_change(c);
	ui->R_Box->setValue(255);
	ui->G_Box->setValue(127);
	ui->B_Box->setValue(39);
}

void Drawer::Color_purple()
{
	QColor c(163, 73, 164);
	ui->Current_Color->update_color(c);
	ui->Canvas->Color_change(c);
	ui->R_Box->setValue(163);
	ui->G_Box->setValue(73);
	ui->B_Box->setValue(164);
}

void Drawer::Color_yellow()
{
	QColor c(255, 255, 0);
	ui->Current_Color->update_color(c);
	ui->Canvas->Color_change(c);
	ui->R_Box->setValue(255);
	ui->G_Box->setValue(255);
	ui->B_Box->setValue(0);
}

void Drawer::Color_random(int value)
{
	QColor c(ui->R_Box->value(), ui->G_Box->value(), ui->B_Box->value());
	ui->Current_Color->update_color(c);
	ui->Canvas->Color_change(c);
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
			if (!(dx<1 && dy<1)&&abs(dx)<6&&abs(dy)<6)//
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
		if (dx < CANVAS_WIDTH_MIN)
			dx = CANVAS_WIDTH_MIN;
		else if (dx > CANVAS_WIDTH_MAX)
			dx = CANVAS_WIDTH_MAX;
		if (dy < CANVAS_HEIGHT_MIN)
			dy = CANVAS_HEIGHT_MIN;
		else if (dy > CANVAS_HEIGHT_MAX)
			dy = CANVAS_HEIGHT_MAX;
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
