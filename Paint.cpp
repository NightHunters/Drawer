#include "Paint.h"

Paint::Paint(QPaintDevice* d) :QPainter(d)
{

}

void Paint::drawline_DDA(int x1, int y1, int x2, int y2)//线生成的DDA算法
{
	double dx, dy;
	int steps, i;
	if (abs(x2 - x1) > abs(y2 - y1))
		steps = abs(x2 - x1);
	else
		steps = abs(y2 - y1);
	dx = (double)(x2 - x1) / steps;
	dy = (double)(y2 - y1) / steps;
	double x = (double)x1;
	double y = (double)y1;
	for (i = 0; i < steps; i++)
	{
		drawPoint(int(x + 0.5), int(y + 0.5)); //在(x，y)处画点,注意double类型取整四舍五入要+0.5
		x += dx;
		y += dy;
	}
}

void Paint::drawline_Bresenham(int x1, int y1, int x2, int y2)//线段生成的Bresenham算法
{
	int x0 = x1, y0 = y1, xe = x2, ye = y2;
	int steps;
	if (abs(x2 - x1) > abs(y2 - y1))
	{
		if (x1 > x2)
		{
			x0 = x2;
			y0 = y2;
			xe = x1;
			ye = y1;
		}
		int deltax = xe - x0;
		int deltay = ye - y0;
		int p1x = 2 * deltay - 2 * deltax;
		int p2x = -2 * deltay - 2 * deltax;
		int px = 2 * deltay - deltax;
		int x = x0;
		int y = y0;
		drawPoint(x, y);
		steps = xe - x0;
		if (ye > y0)
		{
			for (int i = 0; i < steps; i++)
			{
				x += 1;
				if (px < 0)
				{
					drawPoint(x, y);
					px += 2 * deltay;
				}
				else
				{
					y++;
					drawPoint(x, y);
					px += p1x;
				}
			}
		}
		else
		{
			px = -2 * deltay - deltax;
			for (int i = 0; i < steps; i++)
			{
				x += 1;
				if (px < 0)
				{
					drawPoint(x, y);
					px -= 2 * deltay;
				}
				else
				{
					y--;
					drawPoint(x, y);
					px += p2x;
				}
			}
		}
	}
	else
	{
		if (y1 > y2)
		{
			x0 = x2;
			y0 = y2;
			xe = x1;
			ye = y1;
		}
		int deltax = xe - x0;
		int deltay = ye - y0;
		int p1y = 2 * deltax - 2 * deltay;
		int p2y = -2 * deltax - 2 * deltay;
		int py = 2 * deltax - deltay;
		int x = x0;
		int y = y0;
		steps = ye - y0;
		if (xe > x0)
		{
			for (int i = 0; i < steps; i++)
			{
				y += 1;
				if (py < 0)
				{
					drawPoint(x, y);
					py += 2 * deltax;
				}
				else
				{
					x++;
					drawPoint(x, y);
					py += p1y;
				}
			}
		}
		else
		{
			py = -2 * deltax - deltay;
			for (int i = 0; i < steps; i++)
			{
				y += 1;
				if (py < 0)
				{
					drawPoint(x, y);
					py -= 2 * deltax;
				}
				else
				{
					x--;
					drawPoint(x, y);
					py += p2y;
				}
			}
		}
	}
}

void Paint::drawpolygon_DDA(int n, int x[], int y[])
{
	for (int i = 0; i < n; i++)
		drawline_DDA(x[i], y[i], x[(i + 1) % n], y[(i + 1) % n]);
}

void Paint::drawpolygon_Bresenham(int n, int x[], int y[])
{
	for (int i = 0; i < n; i++)
		drawline_Bresenham(x[i], y[i], x[(i + 1) % n], y[(i + 1) % n]);
}

void Paint::drawellipse(int x, int y, int rx, int ry)
{
	double x1 = x;
	double y1 = y;
	bool flag = false;//表示是否rx与ry转换；
	if (ry > rx)
	{
		int t = ry;
		ry = rx;
		rx = t;
		flag = true;
	}
	double rx_sq = rx*rx;
	double ry_sq = ry*ry;
	double rxry_sq = rx_sq*ry_sq;
	double nx = 0;
	double ny = ry;
	double p1 = ry_sq - rx_sq*ry + rx_sq / 4;
	int mx, my;//用来保存上一次的nx，ny值
	while (ry_sq*nx < rx_sq*ny)
	{
		mx = nx;
		my = ny;
		if (flag)
		{
			drawPoint(int(x + ny + 0.5), int(y + nx + 0.5));
			drawPoint(int(x - ny + 0.5), int(y + nx + 0.5));
			drawPoint(int(x + ny + 0.5), int(y - nx + 0.5));
			drawPoint(int(x - ny + 0.5), int(y - nx + 0.5));
		}
		else
		{
			drawPoint(int(x + nx + 0.5), int(y + ny + 0.5));
			drawPoint(int(x - nx + 0.5), int(y + ny + 0.5));
			drawPoint(int(x + nx + 0.5), int(y - ny + 0.5));
			drawPoint(int(x - nx + 0.5), int(y - ny + 0.5));
		}
		if (p1 < 0)
			p1 = p1 + 2 * ry_sq*nx + 3 * ry_sq;
		else
		{
			p1 = p1 + 2 * ry_sq*nx + 3 * ry_sq - 2 * rx_sq*ny + 2 * rx_sq;
			ny--;
		}
		nx++;
	}
	nx = mx;
	ny = my;
	double p2 = ry_sq*(nx + 0.5)*(nx + 0.5) + rx_sq*(ny - 1)*(ny - 1) - rxry_sq;
	while (ny >= 0)
	{
		if (flag)
		{
			drawPoint(int(x + ny + 0.5), int(y + nx + 0.5));
			drawPoint(int(x - ny + 0.5), int(y + nx + 0.5));
			drawPoint(int(x + ny + 0.5), int(y - nx + 0.5));
			drawPoint(int(x - ny + 0.5), int(y - nx + 0.5));
		}
		else
		{
			drawPoint(int(x + nx + 0.5), int(y + ny + 0.5));
			drawPoint(int(x - nx + 0.5), int(y + ny + 0.5));
			drawPoint(int(x + nx + 0.5), int(y - ny + 0.5));
			drawPoint(int(x - nx + 0.5), int(y - ny + 0.5));
		}
		if (p2 <= 0)
		{
			p2 = p2 + 2 * ry_sq*nx + 2 * ry_sq - 2 * rx_sq*ny + 3 * rx_sq;
			nx++;
		}
		else
			p2 = p2 - 2 * rx_sq*ny + 3 * rx_sq;
		ny--;
	}
}

double Paint::BEZ(int k, int n, double u)
{
	if (k == n)
		return pow(u, n);
	else if (k == 0)
		return pow(1 - u, n);
	else
		return (1 - u)*BEZ(k, n - 1, u) + u*BEZ(k - 1, n - 1, u);
}
void Paint::drawcurse_Bezier(int n, int x[], int y[])
{
	for (double u = 0; u <= 1; u += 0.005)
	{
		double nx = 0;
		double ny = 0;
		for (int i = 0; i < n; i++)
		{
			double bez = BEZ(i, n - 1, u);
			nx += x[i] * bez;
			ny += y[i] * bez;
		}
		drawPoint(int(nx + 0.5), int(ny + 0.5));
	}
}

double Paint::F_0_3(double t)
{
	return 1.0 / 6 * (1 - t)*(1 - t)*(1 - t);
}
double Paint::F_1_3(double t)
{
	return 1.0 / 6 * (3 * t*t*t - 6 * t*t + 4);
}
double Paint::F_2_3(double t)
{
	return 1.0 / 6 * (-3 * t*t*t + 3 * t*t + 3 * t + 1);
}
double Paint::F_3_3(double t)
{
	return 1.0 / 6 * t*t*t;
}
void Paint::drawcurse_B_spline(int n, int x[], int y[])
{
	for (int i = 0; i < n - 3; i++)
	{
		double nx = 0;
		double ny = 0;
		for (double t = 0; t <= 1; t += 0.01)
		{
			nx = F_0_3(t)*x[i] + F_1_3(t)*x[i + 1] + F_2_3(t)*x[i + 2] + F_3_3(t)*x[i + 3];
			ny = F_0_3(t)*y[i] + F_1_3(t)*y[i + 1] + F_2_3(t)*y[i + 2] + F_3_3(t)*y[i + 3];
			drawPoint(int(nx + 0.5), int(ny + 0.5));
		}
	}
}