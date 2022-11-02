#include <GL/GLUT.h>
#include <math.h>

GLint winWidth = 600, winHeight = 600;
typedef GLint vertex3i[3];
typedef GLfloat vertex3f[3];

vertex3f p0 = { 200.0, 300.0, 200.0 }; // 观察参考系原点
vertex3f p_ref = { 0.0, 0.0, 0.0 }; // 观察参考点
GLfloat Vx = 0.0, Vy = 0.0, Vz = 1.0;

GLfloat xwMin = -40.0, ywMin = -40.0, xwMax = 40.0, ywMax = 40.0;
GLfloat dnear = 20.0, dfar = 1000.0;


void coordinateSystem() {
    glColor3f(0.4, 0.4, 0.4);
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 300.0);
    glEnd();
    glLineWidth(1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 300.0, 0.0);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(300.0, 0.0, 0.0);
    glEnd();
}

GLfloat ag;
void displayFcn(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    coordinateSystem();

    ag += 0.03;
    glRotated(ag, 0.0, 0.0, 1.0);
    glTranslatef(75, 75, 75);
    glColor3f(0.0, 1.0, 0.0);
    glutWireCube(150.0);

    glPopMatrix();
    glutSwapBuffers();
}

void reshapeFcn(GLint newWidth, GLint newHeight) {
    glViewport(0, 0, newWidth, newHeight);

    winWidth = newWidth;
    winHeight = newHeight;
}

void idleFcn(void) {
    // 视角在此处变换
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(p0[0], p0[1], p0[2], p_ref[0], p_ref[1], p_ref[2], Vx, Vy, Vz);

    glutPostRedisplay();
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glMatrixMode(GL_MODELVIEW);
    gluLookAt(p0[0], p0[1], p0[2], p_ref[0], p_ref[1], p_ref[2], Vx, Vy, Vz);

    glMatrixMode(GL_PROJECTION);
    glFrustum(xwMin, xwMax, ywMin, ywMax, dnear, dfar);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(800, 300);
    glutCreateWindow("graphics");

    init();
    glutDisplayFunc(displayFcn);
    glutReshapeFunc(reshapeFcn);
    glutIdleFunc(idleFcn);
    glutMainLoop();

    return 0;
}
