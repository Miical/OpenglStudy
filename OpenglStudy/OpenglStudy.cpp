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

// 基本操作定义

struct Point {
	int x, y;
	Point(int x = 0, int y = 0) : x(x), y(y) {}
	bool operator == (const Point& oth) const {
		return x == oth.x && y == oth.y;
	}
	bool operator < (const Point& oth) const {
		return x == oth.x ? y < oth.y : x < oth.x;
	}
};
int Round(float x) { return int(x + 0.5); }

typedef Point Vector;

Vector operator-(const Point& a, const Point& b) {
	return Vector(a.x - b.x, a.y - b.y);
}
Vector operator+(const Vector& a, const Vector& b) {
	return Vector(a.x + b.x, a.y + b.y);
}
Vector operator*(const Vector& a, float p) {
	return Vector(a.x * p, a.y * p);
}
Vector operator/(const Vector& a, float p) {
	return Vector(a.x / p, a.y / p);
}

int dot(Vector a, Vector b) {
	return a.x * b.x + a.y * b.y;
}
int cross(Vector a, Vector b) {
	return a.x * b.y - a.y * b.x;
}
int length(Vector a) {
	return sqrt(dot(a, a));
}

// 直线绘制算法

void lineMidPoint(Point p1, Point p2, float thickness) {
	if (p1.x > p2.x) std::swap(p1, p2);
	int a = p1.y - p2.y, b = p2.x - p1.x, c = p1.x * p2.y - p2.x * p1.y;

	glPointSize(thickness);
	glBegin(GL_POINTS);
	if (abs(b) > abs(a)) {
		for (Point p = p1; p.x <= p2.x; p.x++) {
			glVertex2i(p.x, p.y);
			bool f = 2 * a * (p.x + 1) + b * (2 * p.y + 1) + 2 * c < 0; // f: 中点是否在直线下方
			if (a < 0) p.y += f;
			else p.y -= !f;
		}
	}
	else {
		int dy = (p2.y - p1.y) < 0 ? -1 : 1;
		for (Point p = p1; p.y != p2.y + dy; p.y += dy) {
			glVertex2i(p.x, p.y);
			// a < 0: f表示中点是否在直线左侧, a > 0: f表示中点是否在直线右侧
			bool f = 2 * a * (p.x + 1) + b * (2 * p.y + 1) + 2 * c > 0;
			if (a < 0) p.x += f;
			else p.x += !f;
		}
	}
	glEnd();
}

void lineDDA(Point p1, Point p2, float thickness) {
	int difX = p2.x - p1.x, difY = p2.y - p1.y;
	int steps = std::max(abs(difX), abs(difY));
	float xIncrement = 1.0f * difX / steps, yIncrement = 1.0f * difY / steps;

	glPointSize(thickness);

	glBegin(GL_POINTS);
	float x = p1.x, y = p1.y;
	glVertex2i(round(x), round(y));
	for (int i = 0; i < steps; i++) {
		x += xIncrement, y += yIncrement;
		glVertex2i(Round(x), Round(y));
	}
	glEnd();
}

// 圆绘制算法

void circleMidPoint(Point o, int radius, float thickness) {
	glPointSize(thickness);
	glBegin(GL_POINTS);

	Point p = Point(0, radius);
	float d = 1.25 - radius;
	while (p.x <= p.y) {
		if (d >= 0.0) {
			p.x++, p.y--;
			d = d + 2 * (p.x - p.y) + 5;
		}
		else {
			p.x++;
			d = d + 2 * p.x + 3;
		}

		for (int i = -1; i <= 1; i += 2) {
			for (int j = -1; j <= 1; j += 2) {
				glVertex2i(i * p.x + o.x, j * p.y + o.y);
				glVertex2i(i * p.y + o.x, j * p.x + o.y);
			}
		}
	}
	glEnd();
}


// 多边形填充

typedef std::vector<Point> Ploy;

struct Line {
	Point a, b;
	Line(Point a_, Point b_) : a(a_), b(b_) {}
	bool operator==(const Line& line) const {
		return a == line.a && b == line.b;
	}
	bool operator!=(const Line& line) const {
		return !(*this == line);
	}
	Point lowerPoint() const {
		if (a.y != b.y) 
			return a.y < b.y ? a : b; 
		else 
			return a.x < b.x ? a : b;
	}
	Point higherPoint() const {
		return a == lowerPoint() ? b : a;
	}
};

struct EdgeNode {
	int yMax;
	float x, deltaX;
	EdgeNode(int yMax_, float x_, float deltaX_) : 
		yMax(yMax_), x(x_), deltaX(deltaX_) {}
};
typedef std::vector<EdgeNode> edgeList;
typedef edgeList AET;
typedef std::vector<edgeList> NET;

void ployFill(Ploy ploy) {
	std::vector<Line> lines;
	for (int i = 0; i < ploy.size(); i++)
		lines.push_back(Line(ploy[i], ploy[(i + 1) % ploy.size()]));
	glPointSize(1);
	glBegin(GL_POINTS);

	int minY = INF, maxY = -INF;
	for (auto& line : lines) {
		minY = std::min(minY, line.lowerPoint().y);
		maxY = std::max(maxY, line.higherPoint().y);
	}
	
	// 建立NET
	NET net(maxY - minY + 1);
	for (auto& line : lines) {
		edgeList& ls = net[line.lowerPoint().y - minY];
		ls.push_back(EdgeNode(line.higherPoint().y, line.lowerPoint().x,
			1.0 * (line.higherPoint().x - line.lowerPoint().x)
			/ (std::max(1e-10, 1.0 * line.higherPoint().y - line.lowerPoint().y))));
	}
	
	AET aet;
	for (int y = minY; y <= maxY; y++) {
		// NET中对应节点插入AET中
		int netId = y - minY;
		for (auto& edgeNode : net[netId]) {
			for (auto it = aet.begin();; it++) {
				if (it == aet.end() || it->x > edgeNode.x) {
					aet.insert(it, edgeNode);
					break;
				}
			}
		}

		// 点配对，画出对应像素
		for (int i = 0; i + 1 < aet.size(); i += 2) {
			if (round(aet[i].x) == round(aet[i + 1].x)) {
				if ((y == aet[i].yMax) + (y == aet[i + 1].yMax) == 1) i--;
				continue;
			}
			for (int x = aet[i].x; x <= aet[i + 1].x; x++)
				glVertex2i(x, y);
		}
		
		// 更新AET
		for (auto it = aet.begin(); it != aet.end();) {
			if (it->yMax == y) {
				it = aet.erase(it);
			}
			else {
				it->x += it->deltaX;
				++it;
			}
		}

		// 允许边自相交，排序
		sort(aet.begin(), aet.end(), [](const EdgeNode& e1, const EdgeNode& e2) {
			return e1.x < e2.x;
		});
	}
	glEnd();
}


// Ploy Clipping

void emptyPloy(Ploy ploy, float thickness) {
	for (int i = 0; i < ploy.size() - 1; i++) 
		lineDDA(ploy[i], ploy[i + 1], thickness);
	lineDDA(ploy.back(), ploy.front(), thickness);
}

bool onLineRight(Line line, Point p) {
	return cross(line.b - line.a, p - line.a) < 0;
}
Point getLineIntersection(Line l1, Line l2) {
	Point p = l1.a, q = l2.a;
	Vector v = l1.b - l1.a, w = l2.b - l2.a;
	Vector u = p - q;
	float t = 1.0 * cross(w, u) / cross(v, w);
	return p + v * t;
}

Ploy ployClippingSH(Ploy area, Ploy ploy) {
	for (int i = 0; i < area.size(); i++) {
		Ploy ans;
		Line line(area[i], area[(i + 1) % area.size()]);
		if (onLineRight(line, ploy[0]))
			ans.push_back(ploy[0]);
		for (int j = 0; j < ploy.size(); j++) {
			Point& nextPoint = ploy[(j + 1) % ploy.size()];
			if (onLineRight(line, ploy[j])) {
				if (onLineRight(line, nextPoint))
					ans.push_back(nextPoint);
				else
					ans.push_back(getLineIntersection(line, 
						Line(ploy[j], nextPoint)));
			}
			else if (onLineRight(line, nextPoint)) {
					ans.push_back(getLineIntersection(line, 
						Line(ploy[j], nextPoint)));
					ans.push_back(nextPoint);
			}
		}
		ploy = ans;
	}
	return ploy;
}

// 直线段裁剪
typedef Point RectArea[2];

int encode(Point p, RectArea area) {
	return ((p.y > area[1].y) << 3) + ((p.y < area[0].y) << 2) 
		 + ((p.x > area[1].x) << 1) + ((p.x < area[0].x) << 0);
}

Point getLineIntersection(Line line, RectArea area) {
	Point midPoint((line.a.x + line.b.x) / 2, (line.a.y + line.b.y) / 2);
	if (length(line.a - midPoint) < 1.5) 
		return midPoint;
	int codeA = encode(line.a, area), codeB = encode(line.b, area),
		codeMid = encode(midPoint, area);
	if (codeA == 0) {
		if (codeMid == 0) 
			return getLineIntersection(Line(midPoint, line.b), area);
		else
			return getLineIntersection(Line(line.a, midPoint), area);
	}
	else {
		if (codeMid == 0) 
			return getLineIntersection(Line(midPoint, line.a), area);
		else
			return getLineIntersection(Line(line.b, midPoint), area);

	}
	return midPoint;
}

const Line EMPTY_LINE(Point(INF, INF), Point(INF, INF));
Line midLineClip(Line line, RectArea area) {
	int codeA = encode(line.a, area), codeB = encode(line.b, area);
	if ((codeA & codeB) != 0) {
		return EMPTY_LINE;
	} 
	else if (codeA == 0 && codeB == 0) {
		return line;
	}
	else {
		if (codeA == 0) {
			return Line(line.a, getLineIntersection(line, area));
		}
		else if (codeB == 0) {
			return Line(getLineIntersection(line, area), line.b);
		}
		else {
			Point midPoint((line.a.x + line.b.x) / 2, (line.a.y + line.b.y) / 2);
			Line l1 = midLineClip(Line(line.a, midPoint), area);
			Line l2 = midLineClip(Line(midPoint, line.b), area);
			std::vector<Point> v;
			if (l1 != EMPTY_LINE) v.push_back(l1.a), v.push_back(l1.b);
			if (l2 != EMPTY_LINE) v.push_back(l2.a), v.push_back(l2.b);
			if (v.empty()) return EMPTY_LINE;
			std::sort(v.begin(), v.end());
			return Line(v.front(), v.back());
		}
	}
}

// 画图函数

void drawLine() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	srand(2000);
	for (int i = 1; i <= 10; i++) {
		lineMidPoint(Point(rand() % MAX_X, rand() % MAX_Y), Point(rand() % MAX_X, rand() % MAX_Y), 4.0);
		// lineDDA(Point(rand()%MAX_X, rand()%MAX_Y), Point(rand()%MAX_X, rand()%MAX_Y), 4.0);
	}

	glFlush();
}

void drawCircle() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	circleMidPoint(Point(MAX_X / 2, MAX_Y / 2), 200, 3.0);

	glFlush();
}

void drawClock() {
	const int R = 150;
	const Point O = Point(MAX_X / 2, MAX_Y / 2);
	glClear(GL_COLOR_BUFFER_BIT);

	// 圆轮廓
	glColor3f(0.0, 0.0, 0.0);
	circleMidPoint(O, 1.1 * R, 12.5);
	circleMidPoint(O, 1.16 * R, 5.0);

	// 长刻度线
	glColor3f(0.0, 0.0, 0.0);
	const int LONG_LINE_LENGTH = 8.0;
	for (int i = 0; i < 12; i++) {
		float rad = 2.0 * PI / 12 * i;
		Point startPoint = Point(R * sin(rad) + O.x, R * cos(rad) + O.y);
		Point endPoint = Point((R - LONG_LINE_LENGTH) * sin(rad) + O.x,
			(R - LONG_LINE_LENGTH) * cos(rad) + O.y);
		lineDDA(startPoint, endPoint, 4.0);
	}

	// 短刻度线
	glColor3f(0.0, 0.0, 0.0);
	const int SHORT_LINE_LENGTH = 5.0;
	for (int i = 0; i < 60; i++) {
		float rad = 2.0 * PI / 60 * i;
		Point startPoint = Point(R * sin(rad) + O.x, R * cos(rad) + O.y);
		Point endPoint = Point((R - SHORT_LINE_LENGTH) * sin(rad) + O.x,
			(R - SHORT_LINE_LENGTH) * cos(rad) + O.y);
		lineDDA(startPoint, endPoint, 1.5);
	}

	// 中心点
	glColor3f(0.0, 0.0, 0.0);
	circleMidPoint(O, 6, 4.5);

	// 时针	
	glColor3f(0.0, 0.0, 0.0);
	const int HOUR_HAND_LENGTH = 0.5 * R;
	const float HOUR_HAND_DEGREE = 0.6 * PI;
	Point hourHandEndPoint =
		Point(HOUR_HAND_LENGTH * sin(HOUR_HAND_DEGREE) + O.x,
			  HOUR_HAND_LENGTH * cos(HOUR_HAND_DEGREE) + O.y);
	lineDDA(O, hourHandEndPoint, 6.0);

	// 分针	
	glColor3f(0.0, 0.0, 0.0);
	const int MINUTE_HAND_LENGTH = 0.75 * R;
	const float MINUTE_HAND_DEGREE = 1.2 * PI;
	Point minuteHandEndPoint =
		Point(MINUTE_HAND_LENGTH * sin(MINUTE_HAND_DEGREE) + O.x,
			  MINUTE_HAND_LENGTH * cos(MINUTE_HAND_DEGREE) + O.y);
	lineDDA(O, minuteHandEndPoint, 3.0);

	// 秒针	
	glColor3f(1.0, 0.0, 0.0);
	const int SECOND_HAND_LENGTH = 0.85 * R;
	const float SECOND_HAND_DEGREE = 1.8 * PI;
	Point secondHandEndPoint =
		Point(SECOND_HAND_LENGTH * sin(SECOND_HAND_DEGREE) + O.x,
			  SECOND_HAND_LENGTH * cos(SECOND_HAND_DEGREE) + O.y);
	lineDDA(O, secondHandEndPoint, 1.5);
	secondHandEndPoint =
		Point((R - SECOND_HAND_LENGTH) * sin(SECOND_HAND_DEGREE + PI) + O.x,
			  (R - SECOND_HAND_LENGTH) * cos(SECOND_HAND_DEGREE + PI) + O.y);
	lineDDA(O, secondHandEndPoint, 1.5);
	circleMidPoint(O, 2, 3);

	glFlush();
}

void drawFilledPloy() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0.95, 0.1);

	Ploy ploy{ Point(368, 31), Point(435, 241), Point(655, 242), Point(485, 373), Point(555, 587), Point(371, 468), Point(182, 589), Point(250, 377), Point(80, 248), Point(302, 243)};
	ployFill(ploy);

	glFlush();
}

void testSH() {
	glClear(GL_COLOR_BUFFER_BIT);

	Ploy area{ Point(80, 80), Point(50, 400), Point(500, 599), Point(630, 580), Point(400, 100), Point(210, 50) };
	Ploy ploy{ Point(308, 131), Point(615, 242), Point(631, 533), Point(555, 587), Point(182, 589), Point(80, 248), Point(122, 200)};

	glColor3f(0.2, 0.2, 0.2);
	emptyPloy(area, 3.0);

	glColor3f(0.0, 0.9, 0.2);
	emptyPloy(ploy, 3.0);

	ploy = ployClippingSH(area, ploy);
	ployFill(ploy);


	glFlush();
}

void testMidLineClip() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.2, 0.2, 0.2);

	RectArea area{ Point(200, 100), Point(600, 500) };
	Ploy edges = { Point(200, 100), Point(200, 500), Point(600, 500), Point(600, 100) };
	emptyPloy(edges, 3.0);
	
	const int LENGTH = 250;
	for (float i = 0; i <= 2 * PI; i += 2.0 * PI / 90.0) {
		Point p = Point(MAX_X / 2 + LENGTH * cos(i), MAX_Y / 2 + LENGTH * sin(i));
		Line l = Line(Point(MAX_X / 2, MAX_Y / 2), p);

		glColor3f(0.5, 0.5, 0.5);
		lineDDA(l.a, l.b, 1.0);

		l = midLineClip(l, area);
		glColor3f(0.1, 0.1, 0.1);
		if (l != EMPTY_LINE) 
			lineDDA(l.a, l.b, 2.0);
	}

	glFlush();
}

void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, MAX_X, 0.0, MAX_Y);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Shape");

	init();
	glutDisplayFunc(testMidLineClip);
	glutMainLoop();

	return 0;
}
