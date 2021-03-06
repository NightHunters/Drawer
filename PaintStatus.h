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
	int status;			//表示画图的类型
	int num;			//关键点的数目
	int confirm_points;	//确认点数目
	QColor color;
	Point Min;			//记录当前图元的范围
	Point Max;
	vector<Point> vec;	//存下关键点
	PaintStatus();
	void add_point(int x, int y);
	void update_point(int n, int x, int y);
	void refresh();
	void add_one_comfirm_point();
	void calculate_area();//计算图元的范围
};