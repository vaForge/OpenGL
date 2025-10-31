#include <windows.h>
#include <GL/glut.h>

float angle = 0.0;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2, 2, -2, 2, -10, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Rotate model
    glRotatef(angle, 0.0, 0.0, 1.0);

    glColor3f(0.0, 1.0, 0.0);
    glutWireTeapot(1.0);

    glutSwapBuffers();
}

void idle() {
    angle += 0.2f;
    if (angle > 360) angle -= 360;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("GL_MODELVIEW Example");
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
