#include <GL/GLUT.h>
#include <windows.h>
#include <math.h>
#include <cstdio>
const float PI = 3.14159265358979323846;
GLint winWidth = 1200, winHeight = 700;
typedef GLint vertex3i[3];
typedef GLfloat vertex3f[3];
GLfloat object_mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //定义材质的环境光颜色
GLfloat object_mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //定义材质的漫反射光颜色
GLfloat object_mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //定义材质的镜面反射光颜色
GLfloat object_mat_emission[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //定义材质的辐射光颜色
GLfloat object_mat_shininess = 5.0f; //定义材质的光泽程度
const int MAX_TEXTURE = 100;
GLint TEXTURE[MAX_TEXTURE];

void useTexture(GLint id) {
	glBindTexture(GL_TEXTURE_2D, TEXTURE[id]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// 坐标系
void coordinateSystem() {
    glColor3f(0.4, 0.4, 0.4);
    glLineWidth(3.0);
    glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0); glVertex3f(0.0, 0.0, 10.0);
		glVertex3f(0.0, 0.0, 0.0); glVertex3f(0.0, 10.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0); glVertex3f(10.0, 0.0, 0.0);
    glEnd();
}

// 正方体
int cube_texture[6];
void cube() {
    const vertex3f cube[8] = {
        { -1.0, -1.0, -1.0 }, { -1.0, 1.0, -1.0},
        { -1.0, 1.0, 1.0 }, { -1.0, -1.0, 1.0},
        { 1.0, -1.0, -1.0 }, { 1.0, 1.0, -1.0},
        { 1.0, 1.0, 1.0 }, { 1.0, -1.0, 1.0},
    };
    const int vertexsOrder[6][4] = {
    { 4, 5, 6, 7 }, { 1, 0, 3, 2 },
    { 0, 4, 7, 3 }, { 5, 1, 2, 6 },
    { 6, 2, 3, 7 }, { 1, 0, 4, 5}
    };
    const GLfloat normals[6][3] = {
        { 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f }
    };
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    for (int i = 0; i < 6; i++) {
        useTexture(cube_texture[i]);
        glBegin(GL_QUADS);
        glNormal3f(normals[i][0], normals[i][1], normals[i][2]);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(cube[vertexsOrder[i][0]]);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(cube[vertexsOrder[i][1]]);
        glTexCoord2f(1.0f, 1.0f); glVertex3fv(cube[vertexsOrder[i][2]]);
        glTexCoord2f(0.0f, 1.0f); glVertex3fv(cube[vertexsOrder[i][3]]);
        glEnd();
    }
    glPopMatrix();
}

// 绘制天空
void sky() {
    glPushMatrix();
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glScalef(100, 100, 100);
    cube_texture[0] = 1; cube_texture[1] = 0;
    cube_texture[2] = 2; cube_texture[3] = 3;
    cube_texture[4] = 4; cube_texture[5] = 5;
    cube();
    glPopMatrix();
}



// 绘制地面
void ground() {
    glColor4f(1.0, 1.0, 1.0, 1.0);
    useTexture(5);
    glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-100, -100, -5);
		glTexCoord2f(25.0f, 0.0f); glVertex3f(100, -100, -5);
		glTexCoord2f(25.0f, 25.0f); glVertex3f(100, 100, -5);
		glTexCoord2f(0.0f, 25.0f); glVertex3f(-100, 100, -5);
    glEnd();
}

// 绘制河流
void river() {
    glColor4f(1.0, 1.0, 1.0, 1.0);
    useTexture(6);
    glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-100, -60, -4.99);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(100, -60, -4.99);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(100, 0, -4.99);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-100, 0, -4.99);
    glEnd();
}

// 绘制房子
void house() {
    // 房子下部分
    glColor4f(1.0, 1.0, 1.0, 1.0);
    cube_texture[0] = 7; cube_texture[1] = 7;
    cube_texture[2] = 7; cube_texture[3] = 7;
    cube_texture[4] = 7; cube_texture[5] = 7;

    const vertex3f cube[8] = {
        { -1.0, -1.0, -1.0 }, { -1.0, 1.0, -1.0},
        { -1.0, 1.0, 1.0 }, { -1.0, -1.0, 1.0},
        { 1.0, -1.0, -1.0 }, { 1.0, 1.0, -1.0},
        { 1.0, 1.0, 1.0 }, { 1.0, -1.0, 1.0},
    };
    const int vertexsOrder[6][4] = {
    { 4, 5, 6, 7 }, { 1, 0, 3, 2 },
    { 0, 4, 7, 3 }, { 5, 1, 2, 6 },
    { 6, 2, 3, 7 }, { 1, 0, 4, 5}
    };
    const GLfloat normals[6][3] = {
        { 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f }
    };
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslated(0, 20, 0);
    glScalef(10, 7, 5);
    for (int i = 0; i < 6; i++) {
        useTexture(cube_texture[i]);
        glBegin(GL_QUADS);
        glNormal3f(normals[i][0], normals[i][1], normals[i][2]);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(cube[vertexsOrder[i][0]]);
        glTexCoord2f(5.0f, 0.0f); glVertex3fv(cube[vertexsOrder[i][1]]);
        glTexCoord2f(5.0f, 5.0f); glVertex3fv(cube[vertexsOrder[i][2]]);
        glTexCoord2f(0.0f, 5.0f); glVertex3fv(cube[vertexsOrder[i][3]]);
        glEnd();
    }
    glPopMatrix();

    // 屋顶
    glPushMatrix();
    useTexture(8);
    glTranslated(0, 15, 3.2);
    glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-11, -5, 0);
		glTexCoord2f(3.0f, 0.0f); glVertex3f(11, -5, 0);
		glTexCoord2f(3.0f, 3.0f); glVertex3f(11, 5, 6);
		glTexCoord2f(0.0f, 3.0f); glVertex3f(-11, 5, 6);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    useTexture(8);
    glTranslated(0, 25, 3.2);
    glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-11, -5, 6);
		glTexCoord2f(3.0f, 0.0f); glVertex3f(11, -5, 6);
		glTexCoord2f(3.0f, 3.0f); glVertex3f(11, 5, 0);
		glTexCoord2f(0.0f, 3.0f); glVertex3f(-11, 5, 0);
    glEnd();
    glPopMatrix();
}

// 绘制群山 
void mountain() {
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.1, 0.3, 0.1);
    
    glPushMatrix();
    glTranslated(80, -80, -5);
    glutSolidCone(40.0, 35.0, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glTranslated(40, -80, -5);
    glutSolidCone(30.0, 25.0, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-60, -80, -5);
    glutSolidCone(50.0, 40.0, 100, 100);
    glPopMatrix();


    glPushMatrix();
    glTranslated(50, 80, -5);
    glutSolidCone(40.0, 35.0, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glTranslated(70, 40, -5);
    glutSolidCone(25.0, 25.0, 100, 100);
    glPopMatrix();

    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
}

// 绘制船
GLfloat shipSpeed = 0.05;
GLfloat shipOffset = 0;
void singleShip() {
    glColor4f(1.0, 1.0, 1.0, 1.0);
    cube_texture[0] = 9; cube_texture[1] = 9;
    cube_texture[2] = 9; cube_texture[3] = 9;
    cube_texture[4] = 9; cube_texture[5] = 9;

    const vertex3f cube[8] = {
        { -0.65, -0.65, 0.0 }, { -0.65, 0.65, 0.0},
        { -1.0, 1.0, 1.0 }, { -1.0, -1.0, 1.0},
        { 0.65, -0.65, 0.0 }, { 0.65, 0.65, 0.0},
        { 1.0, 1.0, 1.0 }, { 1.0, -1.0, 1.0},
    };
    const int vertexsOrder[6][4] = {
    { 4, 5, 6, 7 }, { 1, 0, 3, 2 },
    { 0, 4, 7, 3 }, { 5, 1, 2, 6 },
    { 6, 2, 3, 7 }, { 1, 0, 4, 5}
    };
    const GLfloat normals[6][3] = {
        { 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f }
    };
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    for (int i = 0; i < 6; i++) {
        useTexture(cube_texture[i]);
        glBegin(GL_QUADS);
        glNormal3f(normals[i][0], normals[i][1], normals[i][2]);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(cube[vertexsOrder[i][0]]);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(cube[vertexsOrder[i][1]]);
        glTexCoord2f(1.0f, 1.0f); glVertex3fv(cube[vertexsOrder[i][2]]);
        glTexCoord2f(0.0f, 1.0f); glVertex3fv(cube[vertexsOrder[i][3]]);
        glEnd();
    }
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glColor3f(0.5, 0.5, 0.4);
    glLineWidth(5.0);
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 3);
    glEnd();
    glEnable(GL_TEXTURE_2D);

    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glVertex3f(0, 0, 2);
		glVertex3f(0, 0, 4);
		glVertex3f(0.5, 0, 2);
		glVertex3f(0.1, 0, 2);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}
void ship() {
    shipOffset += shipSpeed;

    const int SHIP_NUM = 12;
    int pos[SHIP_NUM][2] = {
        { 0, -10 }, { 20, -20 }, { 20, -5 }, { 40, -15 }, { 60, -20 }, {80, -3}, 
        { 0, -10 }, { -20, -20 }, { -20, -5 }, { -40, -15 }, { -60, -20 }, {-80, -18}
    };
    for (int i = 0; i < SHIP_NUM; i++) {
		glPushMatrix();
		glTranslated((int((shipOffset + pos[i][0] + 100) * 10) % 2000 - 1000) / 10.0, pos[i][1], -5);
		glScaled(4, 2, 2);
		singleShip();
		glPopMatrix();
    }
}


// 绘制树
void singleTree() {
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.3, 0.2, 0.0);
    gluCylinder(gluNewQuadric(), 0.5, 0.5, 10, 50, 50);

    glColor3f(0.1, 1.0, 0.2);
    glPushMatrix();
    glTranslated(0, 0, 10);
    glutSolidSphere(3, 50, 50);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}
void tree() {
    glPushMatrix();
    glTranslated(10, -60, -5);
    singleTree();
    glPopMatrix();

    glPushMatrix();
    glTranslated(15, -60, -5);
    singleTree();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-5, -60, -5);
    singleTree();
    glPopMatrix();
}


// 主绘图函数

GLfloat ag;
void displayFcn(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);

    // 绘制光源
    glPushMatrix();
    ag += 0.2;
    glRotated(ag, 1.0, 0.0, 0.0);
    glTranslatef(0, 100, 0);
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

    glPopMatrix();


    // 绘制物体
    sky();
    ground();
    river();
    house();
    mountain();
    ship();
    tree();

    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

/********** 视角变换 **********/

const GLfloat MOVE_SPEED = 0.4f;
const GLfloat MOUSE_SPEED = 1.5f;

// 视野调节
const GLfloat VISION = 0.1f;
const GLfloat XW_MIN = -2.4f * VISION, YW_MIN = -1.4f * VISION,
XW_MAX = 2.4f * VISION, YW_MAX = 1.4f * VISION;
const GLfloat D_NEAR = 0.25f, D_FAR = 200.0f;

// 观察点、观察方向调节
vertex3f p0 = { 10.0, 0, 0.0 }; // 观察参考系原点
GLfloat alpha = PI, gama = PI / 2.0; // alpha - 视线在xoy平面上与x轴夹角，gama - 视线与y轴的夹角.
const GLfloat Vx = 0.0, Vy = 0.0, Vz = 1.0; 

float getRefPointX(){ return p0[0] + sinf(gama) * cosf(alpha); }
float getRefPointY(){ return p0[1] + sinf(gama) * sinf(alpha); }
float getRefPointZ(){ return p0[2] + cosf(gama); }
void move(int direct) {
    float angle = alpha + direct * PI / 2;
    p0[0] += MOVE_SPEED * cosf(angle);
    p0[1] += MOVE_SPEED * sinf(angle);
}

void keyboardFunc(unsigned char key, int x, int y) {
    if (key == 'w') move(0);
    else if (key == 'a') move(1);
    else if (key == 's') move(2);
    else if (key == 'd') move(3);
}

int lastPosX, lastPosY;
void mouseMotionFunc(int x, int y) {
    GLfloat deltaX = GLfloat(x - lastPosX), deltaY = GLfloat(y - lastPosY);
    alpha += MOUSE_SPEED * deltaX / 1000.0f;
    gama += MOUSE_SPEED * deltaY / 1000.0f;

    if (alpha < 0.0) alpha += 2 * PI;
    if (alpha > 2 * PI) alpha -= 2 * PI;
    gama = max(gama, 0.01 * PI);
    gama = min(gama, 0.99f * PI);

    lastPosX = x, lastPosY = y;
}

void mouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		lastPosX = x, lastPosY = y;
    }
}

void specialKeyFunc(int key, int x, int y) {
    if (key == GLUT_KEY_DOWN)
        p0[2] -= MOVE_SPEED;
    else if (key == GLUT_KEY_UP)
        p0[2] += MOVE_SPEED;
}


/********** 纹理加载 **********/

GLuint LoadBMP(const char* fileName) {
    unsigned char header[54], * data;
    unsigned int dataPos, size, width, height;
    FILE* file;
    if (fopen_s(&file, fileName, "rb"))
        return false;
    fread(header, 1, 54, file);
    dataPos = *(int*)&(header[0x0A]);
    size = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);
    if (size == 0)
        size = width * height * 3;
    if (dataPos == 0)
        dataPos = 54;
    data = new unsigned char[size];
    fread(data, 1, size, file);
    fclose(file);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
    return texture;
}

void initTexture() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, object_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, object_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, object_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, object_mat_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, object_mat_shininess);

    TEXTURE[0] = LoadBMP("./img/alt/alt_ft_bmp.bmp");
    TEXTURE[1] = LoadBMP("./img/alt/alt_bk_bmp.bmp");
    TEXTURE[2] = LoadBMP("./img/alt/alt_lf_bmp.bmp");
    TEXTURE[3] = LoadBMP("./img/alt/alt_rt_bmp.bmp");
    TEXTURE[4] = LoadBMP("./img/alt/alt_up_bmp.bmp");
    TEXTURE[5] = LoadBMP("./img/alt/alt_dn_bmp.bmp");
    TEXTURE[5] = LoadBMP("./img/ground_bmp.bmp");
    TEXTURE[6] = LoadBMP("./img/river_bmp.bmp");
    TEXTURE[7] = LoadBMP("./img/house_wall_bmp.bmp");
    TEXTURE[8] = LoadBMP("./img/house_top_bmp.bmp");
    TEXTURE[9] = LoadBMP("./img/ship_bmp.bmp");
    // TEXTURE[7] = LoadBMP("./img/wall2.bmp");
}

// GLUT相关回调函数

void idleFcn(void) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(p0[0], p0[1], p0[2], 
        getRefPointX(), getRefPointY(), getRefPointZ(), 
        Vx, Vy, Vz);

    glutPostRedisplay();
}

void reshapeFcn(GLint newWidth, GLint newHeight) {
    glViewport(0, 0, newWidth, newHeight);

    winWidth = newWidth;
    winHeight = newHeight;
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glMatrixMode(GL_MODELVIEW);
    gluLookAt(p0[0], p0[1], p0[2], 
        getRefPointX(), getRefPointY(), getRefPointZ(), 
        Vx, Vy, Vz);

    glMatrixMode(GL_PROJECTION);
    glFrustum(XW_MIN, XW_MAX, YW_MIN, YW_MAX, D_NEAR, D_FAR);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    initTexture();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(150, 80);
    glutCreateWindow("graphics");

    init();
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(mouseMotionFunc);
    glutSpecialFunc(specialKeyFunc);
    glutDisplayFunc(displayFcn);
    glutReshapeFunc(reshapeFcn);
    glutIdleFunc(idleFcn);

    glutMainLoop();

    return 0;
}
