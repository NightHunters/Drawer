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
	PaintStatus copy_p;//����������ʱ״̬
	PaintStatus current_p;
	QPen pen;
	int captured_point;		//��ǲ�����Ĺؼ���
	int transform_mode;		//ͼԪ�任ģʽ��1��ƽ�ƣ�2����ת��3�����ţ�4�����Ƶ�
	Point reference_point;	//������׼��
	Point rotate_point;		//��ת���Ƶ�
protected:
	void paintEvent(QPaintEvent *ev);		//��ͼ�¼�
	void mousePressEvent(QMouseEvent *ev);	//����¼�
	void mouseMoveEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
public:
	canvas(QWidget *parent = nullptr);
	~canvas();
	void copy_status();
	void calculate_rotate_point();
	bool is_able_change();//�жϵ�ǰ����״̬�Ƿ��ܱ��ı�
	void reset();
	QColor get_color();


	void Transform_line(int x, int y);
	void Transform_polygon(int x, int y);
	void Transform_eclipse(int x, int y);
	void Transform_Bezier(int x, int y);
	void Transform_Bspline(int x, int y);

	//�������
	void Capture_cursor_line(int x, int y);
	void Capture_cursor_polygon(int x, int y);
	void Capture_cursor_eclipse(int x, int y);
	void Capture_cursor_Bezier(int x, int y);
	void Capture_cursor_Bspline(int x, int y);

	void Draw_background();		//���ư�ɫ����
	void Draw_past_primitives();//�����ѻ��õ�ͼԪ
	void Draw_control_lucos();	//���ƿ�����صĵ���
	void Draw_control_points();
	void Draw_control_rect();
	void Draw_control_area();
	void Draw_half_primitives();
	void Draw_unconfirmed_primitives();

	//����ͼԪ
	void To_drawline();			//����
	void To_drawpolygon();		//�������
	void To_draweclipse();
	void To_drawcurve_Bezier();
	void To_drawcurve_Bspline();
	void Revoke_current();

	//��ɫ����
	//void Color_bule();			//��ɫ
	//void Color_green();
	//void Color_red();
	void Color_change(QColor c);
};
