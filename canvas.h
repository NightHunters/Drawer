#pragma once

#include <QWidget>
#include <Qpen>
#include <Qpixmap>
#include <QMouseEvent>
#include"Paint.h"
#include"PaintStatus.h"

class canvas : public QWidget
{
	Q_OBJECT
private:
	vector<PaintStatus> vec;
	PaintStatus copy_p;//用来保存临时状态
	PaintStatus current_p;
	QPen pen;
	int captured_point;		//标记捕获光标的关键点
	int transform_mode;		//图元变换模式，1：平移，2：旋转，3：缩放，4：控制点
	Point reference_point;	//辅助基准点
	Point rotate_point;		//旋转控制点
protected:
	void paintEvent(QPaintEvent *ev);		//画图事件
	void mousePressEvent(QMouseEvent *ev);	//鼠标事件
	void mouseMoveEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
public:
	canvas(QWidget *parent = nullptr);
	~canvas();
	void copy_status();
	void calculate_rotate_point();
	bool is_able_change();//判断当前画布状态是否能被改变


	void Transform_line(int x, int y);
	void Transform_polygon(int x, int y);
	void Transform_eclipse(int x, int y);
	void Transform_Bezier(int x, int y);
	void Transform_Bspline(int x, int y);

	//捕获鼠标
	void Capture_cursor_line(int x, int y);
	void Capture_cursor_polygon(int x, int y);
	void Capture_cursor_eclipse(int x, int y);
	void Capture_cursor_Bezier(int x, int y);
	void Capture_cursor_Bspline(int x, int y);

	void Draw_background();		//绘制白色背景
	void Draw_past_primitives();//绘制已画好的图元
	void Draw_control_lucos();	//绘制控制相关的点线
	void Draw_control_points();
	void Draw_control_rect();
	void Draw_control_area();
	void Draw_half_primitives();
	void Draw_unconfirmed_primitives();

	//绘制图元
	void To_drawline();			//划线
	void To_drawpolygon();		//画多边形
	void To_draweclipse();
	void To_drawcurve_Bezier();
	void To_drawcurve_Bspline();
	void Revoke_current();

	//颜色控制
	void Color_bule();			//蓝色
};
