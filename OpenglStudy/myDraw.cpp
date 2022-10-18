#include "myDraw.h"

bool operator==(const Point& a, const Point& b) {
	return a.x == b.x && a.y == b.y;
}
bool operator<(const Point& a, const Point& b) {
	return a.x == b.x ? a.y < b.y : a.x < b.x;
}


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


bool Line::operator==(const Line& line) const {
	return a == line.a && b == line.b;
}
bool Line::operator!=(const Line& line) const {
	return !(*this == line);
}
Point Line::lowerPoint() const {
	if (a.y != b.y) 
		return a.y < b.y ? a : b; 
	else 
		return a.x < b.x ? a : b;
}
Point Line::higherPoint() const {
	return a == lowerPoint() ? b : a;
}

int Round(float x) { return int(x + 0.5); }


void lineMidPoint(Line line, float thickness) {
	Point p1 = line.a, p2 = line.b;
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

void lineDDA(Line line, float thickness) {
	Point p1 = line.a, p2 = line.b;
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


void circleMidPoint(Circle c, float thickness) {
	Point o = c.o;
	int radius = c.radius;
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


struct EdgeNode {
	int yMax;
	float x, deltaX;
	EdgeNode(int yMax_, float x_, float deltaX_) : 
		yMax(yMax_), x(x_), deltaX(deltaX_) {}
};
typedef std::vector<EdgeNode> edgeList;
typedef edgeList AET;
typedef std::vector<edgeList> NET;

void filledPloy(Ploy ploy) {
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

void emptyPloy(Ploy ploy, float thickness) {
	for (int i = 0; i < ploy.size() - 1; i++) 
		lineDDA(Line(ploy[i], ploy[i + 1]), thickness);
	lineDDA(Line(ploy.back(), ploy.front()), thickness);
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

Ploy ployClipping(Ploy area, Ploy ploy) {
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

Line lineClipping(Line line, RectArea area) {
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
			Line l1 = lineClipping(Line(line.a, midPoint), area);
			Line l2 = lineClipping(Line(midPoint, line.b), area);
			std::vector<Point> v;
			if (l1 != EMPTY_LINE) v.push_back(l1.a), v.push_back(l1.b);
			if (l2 != EMPTY_LINE) v.push_back(l2.a), v.push_back(l2.b);
			if (v.empty()) return EMPTY_LINE;
			std::sort(v.begin(), v.end());
			return Line(v.front(), v.back());
		}
	}
}
