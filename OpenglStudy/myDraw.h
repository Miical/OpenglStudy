#pragma once
#include <GL/glut.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int MAX_X = 800;
const int MAX_Y = 600;
const float PI = 3.14159265358979323846;
const int INF = 0x7fffffff;

/***** 基本元素定义 *****/

// 点
struct Point {
	int x, y;
	Point(int x = 0, int y = 0) : x(x), y(y) {}
};
bool operator==(const Point& a, const Point& b);
bool operator<(const Point& a, const Point& b);

// 向量
typedef Point Vector;

Vector operator-(const Point& a, const Point& b);
Vector operator+(const Vector& a, const Vector& b);
Vector operator*(const Vector& a, float p);
Vector operator/(const Vector& a, float p);
int dot(Vector a, Vector b);
int cross(Vector a, Vector b);
int length(Vector a);
Vector Rotate(Vector a, float rad);

// 直线段
struct Line {
	Point a, b;
	Line(Point a_, Point b_) : a(a_), b(b_) {}
	bool operator==(const Line& line) const;
	bool operator!=(const Line& line) const;
	Point lowerPoint() const;
	Point higherPoint() const;
};
const Line EMPTY_LINE(Point(INF, INF), Point(INF, INF));

// 圆
struct Circle {
	Point o;
	int radius;
	Circle(Point o_, int radius_) : o(o_), radius(radius_) {}
};

// 多边形
typedef std::vector<Point> Ploy;

// 矩形区域
typedef Point RectArea[2];


/***** 实用函数 *****/

int Round(float x);


/***** 绘图函数 *****/

// 直线段绘制
void lineMidPoint(Line line, float thickness);
void lineDDA(Line line, float thickness);

// 直线段裁剪
Line lineClipping(Line line, RectArea area);


// 圆绘制
void circleMidPoint(Circle c, float thickness);

// 多边形
void filledPloy(Ploy ploy);
void emptyPloy(Ploy ploy, float thickness);

// 多边形切割
Ploy ployClipping(Ploy area, Ploy ploy);

/***** 二维几何变换 *****/

// 多边形平移
Ploy ployTranslation(Ploy ploy, Vector shift);
// 多边形旋转
Ploy ployRotate(Ploy ploy, Point rotation_point, float theta);
// 多边形缩放 
Ploy ployScaling(Ploy ploy, Point fixed_point, float sx, float sy);
// 错切
Ploy ployShearX(Ploy ploy, float sh_x, float ref_y);
Ploy ployShearY(Ploy ploy, float sh_y, float ref_x);
