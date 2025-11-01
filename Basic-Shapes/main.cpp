#include<windows.h>
#include <GL/glut.h>
#include <iostream>
#include <cmath>
using namespace std;
float radius;
static float angle = 0.0;
GLfloat controlPoints[4][2] = { //me
    {-8,-8} ,{-5,8}, {5,8} ,{8,-8}

};
GLfloat tetrahedron[4][3] = {  //AI
    {  0.0f,  1.0f,  0.0f },    // top vertex (peak)
    { -0.9428f, -0.3333f,  0.0f }, // base vertex 1
    {  0.4714f, -0.3333f,  0.8165f }, // base vertex 2
    {  0.4714f, -0.3333f, -0.8165f }  // base vertex 3
};
GLfloat colors[4][3] = {
    {1.0f, 0.0f, 0.0f},  // red
    {0.0f, 1.0f, 0.0f},  // green
    {0.0f, 0.0f, 1.0f},  // blue
    {1.0f, 1.0f, 0.0f}   // yellow
};
void drawTriangle();
void drawCircle();
void getBeizerPoints(float t,float& x,float& y);
void drawBeizer();
//void tetSetup();
void drawTetrahedron();
void inputCircle();
void display() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //drawCircle();
    //drawTriangle();
    //drawBeizer();
    drawTetrahedron();
    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    //inputCircle();
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Shiny tetrahedron");
    glClearColor(0.1,0.1,0.1,1.0);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.0, 0.1, 100.0);
    //gluOrtho2D(-10, 10, -10, 10); use for 2D visuals else looks flat and lighting goes wrong
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

void inputCircle(){
    cout << "Enter the Radius of Circle: ";
    cin >> radius;
}
void drawTriangle() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
        glVertex2f(5.0, 5.0);
        glVertex2f(10.0, 10.0);
        glVertex2f(15.0, 5.0);
    glEnd();
}

void drawCircle() {
    glColor3f(0.0, 0.0, 1.0);
    glLineWidth(2.0f);
    glBegin(GL_TRIANGLE_FAN);  //use glBegin(GL_TRIANGLE_FAN) for filling out color
    for (int i = 0; i <= 200; i++) {
        float theta = 2.0f * (3.14159f/2) * i / 200;
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);  //use glBegin(GL_TRIANGLE_FAN) for filling out color
    for (int i = 0; i <= 200; i++) {
        float theta = 2.0f * 3.14159f * i / 200;
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

void getBeizerPoints(float t,float& x,float& y){
    float one_minus_t = 1 - t;
    float p0 = one_minus_t * one_minus_t * one_minus_t;
    float p1 = 3.0f* t * one_minus_t * one_minus_t;
    float p2 = 3.0f * t * t * one_minus_t;
    float p3 = t*t*t;
    x = controlPoints[0][0] * p0+controlPoints[1][0] * p1 + controlPoints[2][0]*p2 + controlPoints[3][0]*p3;
    y = controlPoints[0][1] * p0+controlPoints[1][1] * p1 + controlPoints[2][1]*p2 + controlPoints[3][1]*p3;
}

void drawBeizer(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //draw control points and control polygon
    glColor3f(1,0,0);
    glLineWidth(5.0f);
    glBegin(GL_POINTS);
        for(int i=0;i<4;i++){
            glVertex2fv(controlPoints[i]);
        }
    glEnd();
    glColor3f(0,0,0);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
        for(int i=0;i<4;i++){
            glVertex2fv(controlPoints[i]);
        }
    glEnd();
    int seg = 100;
    glBegin(GL_LINE_STRIP);
        for(int i=0;i<=seg;i++){
            float t = (float)i/seg;
            float x,y;
            getBeizerPoints(t,x,y);
            glVertex2f(x,y);
        }
    glEnd();
}

void drawTetrahedron() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 4.0,   // camera position
              0.0, 0.0, 0.0,   // look at origin
              0.0, 1.0, 0.0);  // up direction

    // Rotate continuously
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    angle += 0.25f;
    GLuint faces[4][3] = {
        {0, 1, 2},
        {0, 2, 3},
        {0, 3, 1},
        {1, 3, 2}
    };
    for (int i = 0; i < 4; i++) {
        glColor3fv(colors[i]);   // assign a color per face
        glBegin(GL_TRIANGLES);
            glVertex3fv(tetrahedron[faces[i][0]]);
            glVertex3fv(tetrahedron[faces[i][1]]);
            glVertex3fv(tetrahedron[faces[i][2]]);
        glEnd();
    }
}
