#include<windows.h>
#include <GL/glut.h>
#include <iostream>
#include <cmath>
using namespace std;
float radius;
void drawTriangle();
void drawCircle();
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    //drawCircle();
    //drawTriangle();
    glFlush();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    cout << "Enter the Radius of Circle: ";
    cin >> radius;
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Circle and Triangle");
    glClearColor(0.6, 0.8, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-10, 10, -10, 10);
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
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
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 200; i++) {
        float theta = 2.0f * 3.14159f * i / 200;
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();
}
