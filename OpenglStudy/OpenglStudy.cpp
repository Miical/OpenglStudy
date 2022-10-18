#include "myDraw.h"

void testTranslation() {
	glClear(GL_COLOR_BUFFER_BIT);

	Ploy ploy{Point(50, 50), Point(300, 50), Point(300, 200), Point(50, 200)};
	glColor3f(1, 0.95, 0.1);
	filledPloy(ploy);

	Ploy ploy2 = ployTranslation(ploy, Vector(200, 250));
	glColor3f(0, 0.95, 1);
	filledPloy(ploy2);

	glFlush();
}

void testRotate() {
	glClear(GL_COLOR_BUFFER_BIT);

	Ploy ploy{Point(50, 50), Point(300, 50), Point(300, 200), Point(50, 200)};
	glColor3f(1, 0.95, 0.1);
	filledPloy(ploy);

	Ploy ploy2 = ployRotate(ploy, Point(300, 200), PI / 4 * 3);
	glColor3f(0, 0.95, 1);
	filledPloy(ploy2);

	glFlush();
}

void testScaling() {
	glClear(GL_COLOR_BUFFER_BIT);
	Ploy ploy{Point(300, 250), Point(550, 250), Point(550, 400), Point(300, 400)};

	Ploy ploy2 = ployScaling(ploy, Point(425, 325), 1.5, 2.5);
	glColor3f(0, 0.95, 1);
	filledPloy(ploy2);

	glColor3f(1, 0.95, 0.1);
	filledPloy(ploy);

	glFlush();
}

void testShearX() {
	glClear(GL_COLOR_BUFFER_BIT);
	Ploy ploy{Point(300, 0), Point(550, 0), Point(550, 250), Point(300, 250)};
	glColor3f(1, 0.95, 0.1);
	emptyPloy(ploy, 3.0);

	Ploy ploy2 = ployShearX(ploy, 0.5, 250);
	glColor3f(0, 0.95, 1);
	emptyPloy(ploy2, 3.0);
	
	glFlush();
}

void testShearY() {
	glClear(GL_COLOR_BUFFER_BIT);
	Ploy ploy{Point(0, 300), Point(0, 550), Point(250, 550), Point(250, 300)};
	glColor3f(1, 0.95, 0.1);
	emptyPloy(ploy, 3.0);

	Ploy ploy2 = ployShearY(ploy, 0.5, 250);
	glColor3f(0, 0.95, 1);
	emptyPloy(ploy2, 3.0);
	
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
	glutDisplayFunc(testShearY);
	glutMainLoop();

	return 0;
}
