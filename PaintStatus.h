#pragma once
#include<QColor>
#include<iostream>
#include<vector>
using namespace std;
class Point
{
public:
	int x;
	int y;
	Point()
	{
		x = 0;
		y = 0;
	}
	Point(int a, int b)
	{
		x = a;
		y = b;
	}
};

class PaintStatus
{
public:
	int status;//��ʾ��ͼ������
	vector<Point> vec;//���¹ؼ���
	int num;//�ؼ������Ŀ
	int confirm_points;//ȷ�ϵ���Ŀ
	QColor color;
	PaintStatus();
	void add_point(int x, int y);
	void update_point(int n, int x, int y);
	void refresh();
	void add_one_comfirm_point();
};