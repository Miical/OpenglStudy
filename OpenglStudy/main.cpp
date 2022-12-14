#include <GL/GLUT.h>
#include <math.h>
#include "imageloader.h"

GLint winWidth = 600, winHeight = 600;
typedef GLint vertex3i[3];
typedef GLfloat vertex3f[3];

vertex3f p0 = { 20.0, 20.0, 20.0 }; // 观察参考系原点
vertex3f p_ref = { 0.0, 0.0, 0.0 }; // 观察参考点
GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0;

GLfloat xwMin = -40.0, ywMin = -40.0, xwMax = 40.0, ywMax = 40.0;
GLfloat dnear = 20.0, dfar = 1000.0;

GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
        0,                            //0 for now
        GL_RGB,                       //Format OpenGL uses for image
        image->width, image->height,  //Width and height
        0,                            //The border of the image
        GL_RGB, //GL_RGB, because pixels are stored in RGB format
        GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
        //as unsigned numbers
        image->pixels);               //The actual pixel data
    return textureId; //Returns the id of the texture
}
GLuint _textureId; //The id of the texture

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    // 光源1
    glPushMatrix();
    ag += 0.1;
    glRotated(ag, 0.0, 0.0, 1.0);
    glTranslatef(200, 120, 100);
    GLfloat light0_position[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //光源的位置在世界坐标系圆心，齐次坐标形式
    GLfloat light0_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //RGBA模式的环境光
    GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //RGBA模式的漫反射光
    GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //RGBA模式下的镜面光
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    GLfloat mat0_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //定义材质的环境光颜色
    GLfloat mat0_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f }; //定义材质的漫反射光颜色
    GLfloat mat0_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f }; //定义材质的镜面反射光颜色，为0
    GLfloat mat0_emission[] = { 1.0f, 0.9f, 0.1f, 1.0f }; //定义材质的辐射光颜色，为白色
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat0_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat0_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat0_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat0_emission);
    glutSolidSphere(20.0, 20, 20);
    glPopMatrix();


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);


    // 物体
    /**/
    GLfloat object_mat_ambient[] = { 0.5f, 0.5f, 0.5, 1.0f }; //定义材质的环境光颜色
    GLfloat object_mat_diffuse[] = { 0.9f, 0.8f, 0.6f, 1.0f }; //定义材质的漫反射光颜色，红色
    GLfloat object_mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //定义材质的镜面反射光颜色，白色
    GLfloat object_mat_emission[] = { 0.3f, 0.3f, 0.3f, 1.0f }; //定义材质的辐射光颜色，为0
    GLfloat object_mat_shininess = 100.0f; //定义材质的光泽程度
    glMaterialfv(GL_FRONT, GL_AMBIENT, object_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, object_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, object_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, object_mat_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, object_mat_shininess);

    //绘制底部矩形
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);

    glNormal3f(0.0, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-8.0f, -8.0f, 8.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(8.0f, -8.0f, 8.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(8.0f, -8.0f, -8.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-8.0f, -8.0f, -8.0f);

    glEnd();


    // 一维纹理
    /*
    GLubyte texLine[16];
    for (int k = 0; k <= 2; k += 2) {
        texLine[4 * k] = 100;
        texLine[4 * k + 1] = 100;
        texLine[4 * k + 2] = 255;
        texLine[4 * k + 3] = 255;
    }
    for (int k = 1; k <= 3; k += 2) {
        texLine[4 * k] = 255;
        texLine[4 * k + 1] = 255;
        texLine[4 * k + 2] = 255;
        texLine[4 * k + 3] = 255;
    }
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, texLine);
    glEnable(GL_TEXTURE_1D);
    glLineWidth(5.0);
    glBegin(GL_LINES);
    glTexCoord1f(0);
    glVertex3f(0, 0, 0);
    glTexCoord1f(20);
    glVertex3f(200.0, 0, 0);
    glEnd();
    glDisable(GL_TEXTURE_1D);
    */


    glutSwapBuffers();
}



void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    Image* image = loadBMP("./img/imag.bmp3");
    _textureId = loadTexture(image);
    delete image;
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 1.0f, -6.0f);

    //设置光照
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat directedLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat directedLightPos[] = { -10.0f, 15.0f, 20.0f, 0.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
    glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    //绘制底部矩形
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);

    glNormal3f(0.0, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-16.0f, -16.0f, 16.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(16.0f, -16.0f, 16.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(16.0f, -16.0f, -16.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-16.0f, -16.0f, -16.0f);

    glEnd();

    /*
    //绘制后部back三角形
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);

    //设定平面法向量指向位置
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.5f, -2.5f, -2.5f);
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 2.5f, -2.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.5f, -2.5f, -2.5f);

    glEnd();

    //绘制左侧矩形
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);

    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.5f, -2.5f, 2.5f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.5f, -2.5f, -2.5f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2.5f, 2.5f, -2.5f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.5f, 2.5f, 2.5f);

    glEnd();

    */
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}


void reshapeFcn(GLint newWidth, GLint newHeight) {
    glViewport(0, 0, newWidth, newHeight);

    winWidth = newWidth;
    winHeight = newHeight;
}
void idleFcn(void) {
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
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(500, 100);
    glutCreateWindow("graphics");

    init();
    initRendering();
    glutDisplayFunc(displayFcn);
    glutReshapeFunc(reshapeFcn);
    glutIdleFunc(idleFcn);
    glutMainLoop();

    return 0;
} 