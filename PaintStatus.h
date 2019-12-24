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
	int status;//表示画图的类型
	vector<Point> vec;//存下关键点
	int num;//关键点的数目
	int confirm_points;//确认点数目
	QColor color;
	PaintStatus();
	void add_point(int x, int y);
	void update_point(int n, int x, int y);
	void refresh();
	void add_one_comfirm_point();
};