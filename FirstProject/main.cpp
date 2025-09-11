#include <GL/glut.h>  // Include the GLUT library for OpenGL

void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen with the background color
    glColor3f(1.0, 0.0, 0.0);      // Set the drawing color to red (R=1, G=0, B=0)

    // Start drawing a triangle
    glBegin(GL_TRIANGLES);
        glVertex2i(2, 2);  // First vertex of the triangle
        glVertex2i(5, 5);  // Second vertex of the triangle
        glVertex2i(8, 2);  // Third vertex of the triangle
    glEnd();  // End of triangle drawing

    glFlush();  // Force execution of OpenGL commands
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);  // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  // Use single buffering and RGB color mode
    glutInitWindowSize(400, 400);  // Set the window size to 400x400 pixels
    glutCreateWindow("Hello OpenGL");  // Create a window with this title

    // Set the background color to white (R=1, G=1, B=1, Alpha=1)
    glClearColor(1.0, 1.0, 1.0, 1.0);

     // Setup the coordinate system so we can use (0,0) to (10,10) for drawing
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 10, 0, 10);  // Left=0, Right=10, Bottom=0, Top=10

    glutDisplayFunc(display);  // Register the display callback function
    glutMainLoop();  // Enter the main event-processing loop (keeps the window open)

    return 0;  // End of program
}
