#include "myDraw.h"

void drawLine() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	srand(2000);
	for (int i = 1; i <= 10; i++) {
		// lineMidPoint(Line(Point(rand() % MAX_X, rand() % MAX_Y), Point(rand() % MAX_X, rand() % MAX_Y)), 4.0);
		lineDDA(Line(Point(rand()%MAX_X, rand()%MAX_Y), Point(rand()%MAX_X, rand()%MAX_Y)), 4.0);
	}

	glFlush();
}

void drawCircle() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);

	circleMidPoint(Circle(Point(MAX_X / 2, MAX_Y / 2), 200), 3.0);

	glFlush();
}

void drawClock() {
	const int R = 150;
	const Point O = Point(MAX_X / 2, MAX_Y / 2);
	glClear(GL_COLOR_BUFFER_BIT);

	// 圆轮廓
	glColor3f(0.0, 0.0, 0.0);
	circleMidPoint(Circle(O, 1.1 * R), 12.5);
	circleMidPoint(Circle(O, 1.16 * R), 5.0);

	// 长刻度线
	glColor3f(0.0, 0.0, 0.0);
	const int LONG_LINE_LENGTH = 8.0;
	for (int i = 0; i < 12; i++) {
		float rad = 2.0 * PI / 12 * i;
		Point startPoint = Point(R * sin(rad) + O.x, R * cos(rad) + O.y);
		Point endPoint = Point((R - LONG_LINE_LENGTH) * sin(rad) + O.x,
			(R - LONG_LINE_LENGTH) * cos(rad) + O.y);
		lineDDA(Line(startPoint, endPoint), 4.0);
	}

	// 短刻度线
	glColor3f(0.0, 0.0, 0.0);
	const int SHORT_LINE_LENGTH = 5.0;
	for (int i = 0; i < 60; i++) {
		float rad = 2.0 * PI / 60 * i;
		Point startPoint = Point(R * sin(rad) + O.x, R * cos(rad) + O.y);
		Point endPoint = Point((R - SHORT_LINE_LENGTH) * sin(rad) + O.x,
			(R - SHORT_LINE_LENGTH) * cos(rad) + O.y);
		lineDDA(Line(startPoint, endPoint), 1.5);
	}

	// 中心点
	glColor3f(0.0, 0.0, 0.0);
	circleMidPoint(Circle(O, 6), 4.5);

	// 时针	
	glColor3f(0.0, 0.0, 0.0);
	const int HOUR_HAND_LENGTH = 0.5 * R;
	const float HOUR_HAND_DEGREE = 0.6 * PI;
	Point hourHandEndPoint =
		Point(HOUR_HAND_LENGTH * sin(HOUR_HAND_DEGREE) + O.x,
			  HOUR_HAND_LENGTH * cos(HOUR_HAND_DEGREE) + O.y);
	lineDDA(Line(O, hourHandEndPoint), 6.0);

	// 分针	
	glColor3f(0.0, 0.0, 0.0);
	const int MINUTE_HAND_LENGTH = 0.75 * R;
	const float MINUTE_HAND_DEGREE = 1.2 * PI;
	Point minuteHandEndPoint =
		Point(MINUTE_HAND_LENGTH * sin(MINUTE_HAND_DEGREE) + O.x,
			  MINUTE_HAND_LENGTH * cos(MINUTE_HAND_DEGREE) + O.y);
	lineDDA(Line(O, minuteHandEndPoint), 3.0);

	// 秒针	
	glColor3f(1.0, 0.0, 0.0);
	const int SECOND_HAND_LENGTH = 0.85 * R;
	const float SECOND_HAND_DEGREE = 1.8 * PI;
	Point secondHandEndPoint =
		Point(SECOND_HAND_LENGTH * sin(SECOND_HAND_DEGREE) + O.x,
			  SECOND_HAND_LENGTH * cos(SECOND_HAND_DEGREE) + O.y);
	lineDDA(Line(O, secondHandEndPoint), 1.5);
	secondHandEndPoint =
		Point((R - SECOND_HAND_LENGTH) * sin(SECOND_HAND_DEGREE + PI) + O.x,
			  (R - SECOND_HAND_LENGTH) * cos(SECOND_HAND_DEGREE + PI) + O.y);
	lineDDA(Line(O, secondHandEndPoint), 1.5);
	circleMidPoint(Circle(O, 2), 3);

	glFlush();
}

void drawFilledPloy() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0.95, 0.1);

	Ploy ploy{ Point(368, 31), Point(435, 241), Point(655, 242), Point(485, 373), Point(555, 587), Point(371, 468), Point(182, 589), Point(250, 377), Point(80, 248), Point(302, 243)};
	filledPloy(ploy);

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

	ploy = ployClipping(area, ploy);
	filledPloy(ploy);


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
		lineDDA(l, 1.0);

		l = lineClipping(l, area);
		glColor3f(0.1, 0.1, 0.1);
		if (l != EMPTY_LINE) 
			lineDDA(l, 2.0);
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
