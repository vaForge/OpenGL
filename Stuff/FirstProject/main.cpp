// solar_system.cpp
// Simple hierarchical solar system demonstration using GLUT (immediate mode).
// Build: g++ solar_system.cpp -o solar_system -lGL -lGLU -lglut
// (On macOS use: g++ solar_system.cpp -o solar_system -framework OpenGL -framework GLUT)

#include <GL/glut.h>
#include <cmath>
#include <cstdio>

// Animation control
static float day = 0.0f;         // rotation of planets around their own axis (degrees)
static float year = 0.0f;        // revolution around Sun (degrees)
static float speed = 1.0f;       // animation speed multiplier
static bool paused = false;

// Window size
int winWidth = 900, winHeight = 600;

// Light settings (simple directional-ish light)
GLfloat lightAmbient[]  = {0.15f, 0.15f, 0.2f, 1.0f};
GLfloat lightDiffuse[]  = {1.0f, 1.0f, 0.9f, 1.0f};
GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPos[]      = {0.0f, 0.0f, 0.0f, 1.0f}; // place at Sun (positional light)

// Utility: draw coordinate axes (for debugging/visualization)
void drawAxes(float length = 2.0f) {
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
      // X - red
      glColor3f(1,0,0);
      glVertex3f(0,0,0); glVertex3f(length,0,0);
      // Y - green
      glColor3f(0,1,0);
      glVertex3f(0,0,0); glVertex3f(0,length,0);
      // Z - blue
      glColor3f(0,0,1);
      glVertex3f(0,0,0); glVertex3f(0,0,length);
    glEnd();
    glEnable(GL_LIGHTING);
}

// Set material conveniently
void setMaterial(const GLfloat ambient[4], const GLfloat diffuse[4],
                 const GLfloat specular[4], GLfloat shininess) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void initLighting() {
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glShadeModel(GL_SMOOTH);
}

// Render the solar system
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Camera / View
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Place camera slightly back and above for a nice view
    // eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ
    gluLookAt( 0.0, 6.0, 18.0,   // camera position
               0.0, 0.0,  0.0,   // looking at origin
               0.0, 1.0,  0.0);  // up vector

    // Move light position with Sun (so Sun "emits" light from its center)
    GLfloat lp[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lp);

    // Optional axes
    // drawAxes(5.0f);

    // -------------------
    // Draw Sun
    // -------------------
    glPushMatrix();
      // Sun material - bright, slightly emissive
      GLfloat sunAmb[]  = {0.5f, 0.45f, 0.05f, 1.0f};
      GLfloat sunDiff[] = {0.9f, 0.7f, 0.1f, 1.0f};
      GLfloat sunSpec[] = {0.9f, 0.9f, 0.5f, 1.0f};
      setMaterial(sunAmb, sunDiff, sunSpec, 50.0f);

      // Make the Sun slightly emissive to look bright
      GLfloat sunEmit[] = {0.6f, 0.5f, 0.1f, 1.0f};
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sunEmit);

      glutSolidSphere(2.0, 40, 40); // radius 2.0

      // turn off emission for other objects
      GLfloat noEmit[] = {0,0,0,1};
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmit);

    // -------------------
    // Earth orbiting Sun
    // -------------------
      // Earth orbit angle = year
      glRotatef(year, 0.0f, 1.0f, 0.0f);    // revolution around Sun in the XZ plane
      glTranslatef(6.0f, 0.0f, 0.0f);       // move out along X to Earth's orbit radius

      // Draw Earth
      glPushMatrix();
        // Earth's own rotation
        glRotatef(day * 2.0f, 0.0f, 1.0f, 0.0f); // day controls axial rotation

        GLfloat eAmb[]  = {0.05f, 0.05f, 0.2f, 1.0f};
        GLfloat eDiff[] = {0.2f, 0.3f, 0.8f, 1.0f};
        GLfloat eSpec[] = {0.3f, 0.3f, 0.3f, 1.0f};
        setMaterial(eAmb, eDiff, eSpec, 25.0f);

        glutSolidSphere(1.0, 32, 32); // Earth radius 1.0

      // -------------------
      // Moon orbiting Earth
      // -------------------
        glPushMatrix();
          // Moon revolution around Earth: faster relative to year (just for demo)
          glRotatef(year * 12.0f + day * 0.5f, 0.0f, 1.0f, 0.0f);
          glTranslatef(2.0f, 0.0f, 0.0f); // moon distance from earth

          GLfloat mAmb[]  = {0.12f, 0.12f, 0.12f, 1.0f};
          GLfloat mDiff[] = {0.6f, 0.6f, 0.6f, 1.0f};
          GLfloat mSpec[] = {0.2f, 0.2f, 0.2f, 1.0f};
          setMaterial(mAmb, mDiff, mSpec, 5.0f);

          glutSolidSphere(0.28, 20, 20); // Moon radius 0.28
        glPopMatrix();

      glPopMatrix();

    // -------------------
    // Planet 2 (e.g., Mars-like)
    // -------------------
      glPushMatrix();
        // a different orbit speed and radius
        glRotatef(year * 0.6f + 45.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(-9.0f, 0.0f, 0.0f);

        GLfloat p2Amb[]  = {0.2f, 0.08f, 0.05f, 1.0f};
        GLfloat p2Diff[] = {0.9f, 0.4f, 0.2f, 1.0f};
        GLfloat p2Spec[] = {0.2f, 0.2f, 0.2f, 1.0f};
        setMaterial(p2Amb, p2Diff, p2Spec, 10.0f);

        // rotate and draw
        glRotatef(day * 0.6f, 0.0f, 1.0f, 0.0f);
        glutSolidSphere(0.9, 24, 24);
      glPopMatrix();

    // -------------------
    // Planet 3 (e.g., big gas giant)
    // -------------------
      glPushMatrix();
        glRotatef(year * 0.25f - 30.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(13.5f, 1.0f, 0.0f); // slight inclination in Y to show 3D
        GLfloat p3Amb[]  = {0.08f, 0.08f, 0.08f, 1.0f};
        GLfloat p3Diff[] = {0.7f, 0.6f, 0.4f, 1.0f};
        GLfloat p3Spec[] = {0.5f, 0.5f, 0.4f, 1.0f};
        setMaterial(p3Amb, p3Diff, p3Spec, 5.0f);

        glRotatef(day * 0.3f, 0.0f, 1.0f, 0.0f);
        glutSolidSphere(2.0, 36, 36);
      glPopMatrix();

    glPopMatrix(); // pop Sun matrix

    glutSwapBuffers();
}

// Idle/timer updates animation variables
void idle() {
    if (!paused) {
        // advance time (adjust factors to taste)
        day  += 0.15f * speed;   // planet axial rotations
        year += 0.06f * speed;   // planet revolutions
        if (day > 360.0f) day -= 360.0f;
        if (year > 360.0f) year -= 360.0f;
    }
    glutPostRedisplay();
}

// Handle window resize: keep aspect ratio and set perspective
void reshape(int w, int h) {
    if (h == 0) h = 1;
    winWidth = w; winHeight = h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // fovY, aspect, near, far
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard commands for simple interaction
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
      case 27:   // ESC
      case 'q':
      case 'Q':
        std::printf("Exiting...\n");
        exit(0);
        break;
      case ' ':
        paused = !paused; // toggle pause
        break;
      case '+':
        speed *= 1.25f; if (speed > 10.0f) speed = 10.0f;
        break;
      case '-':
        speed /= 1.25f; if (speed < 0.01f) speed = 0.01f;
        break;
      case 'r':
      case 'R':
        day = 0.0f; year = 0.0f; speed = 1.0f; paused = false;
        break;
      default:
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    // Double buffer for smooth animation, RGB color, depth buffer enabled
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Simple Solar System - GLUT");

    glEnable(GL_DEPTH_TEST);   // enable Z-buffering
    glEnable(GL_NORMALIZE);    // normalize normals after scaling
    initLighting();

    // Background color - deep space
    glClearColor(0.02f, 0.02f, 0.06f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle); // continuous animation

    glutMainLoop();
    return 0;
}
