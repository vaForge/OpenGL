#include <windows.h>
#include <GL/glut.h>
#include <string>
#include <cmath>
#include <vector>

// --- Constants ---
#define PI 3.1415926535

// --- Global Variables for Simulation State ---
int screenWidth = 1000;
int screenHeight = 600;

bool isConvex = true;
float focalLength = 150.0f;
float objectDistance = 300.0f; // Start at 2F
const float OBJECT_HEIGHT = 50.0f;

// --- Helper function to draw text on screen ---
void drawText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
}

void drawTextLarge(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

// --- Visual Enhancements ---
void drawGrid() {
    glColor4f(0.3f, 0.3f, 0.4f, 0.2f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int x = -screenWidth / 2; x <= screenWidth / 2; x += 50) {
        glVertex2f(x, -screenHeight / 2);
        glVertex2f(x, screenHeight / 2);
    }
    for (int y = -screenHeight / 2; y <= screenHeight / 2; y += 50) {
        glVertex2f(-screenWidth / 2, y);
        glVertex2f(screenWidth / 2, y);
    }
    glEnd();
}

// --- Drawing Functions ---

void drawOpticalAxis() {
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    glColor3f(0.6f, 0.6f, 0.7f);
    glVertex2f(-screenWidth / 2.0f, 0.0f);
    glColor3f(0.8f, 0.8f, 0.9f);
    glVertex2f(screenWidth / 2.0f, 0.0f);
    glEnd();
}

void drawFocalPoints() {
    glLineWidth(2.0f);

    // Right side (F, 2F)
    glColor3f(1.0f, 0.4f, 0.4f);
    glBegin(GL_LINES);
    glVertex2f(focalLength, -8.0f); glVertex2f(focalLength, 8.0f);
    glVertex2f(2 * focalLength, -5.0f); glVertex2f(2 * focalLength, 5.0f);
    glEnd();
    drawText(focalLength - 8.0f, -25.0f, "F");
    drawText(2 * focalLength - 12.0f, -25.0f, "2F");

    // Left side (F', 2F')
    glColor3f(0.5f, 0.7f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(-focalLength, -8.0f); glVertex2f(-focalLength, 8.0f);
    glVertex2f(-2 * focalLength, -5.0f); glVertex2f(-2 * focalLength, 5.0f);
    glEnd();
    drawText(-focalLength - 14.0f, -25.0f, "F'");
    drawText(-2 * focalLength - 18.0f, -25.0f, "2F'");
}

void drawLens() {
    const float lens_half_height = 120.0f;
    const float arc_radius = 250.0f;
    const int num_segments = 40;

    std::vector<float> x_coords, y_coords;
    x_coords.reserve(num_segments + 1);
    y_coords.reserve(num_segments + 1);

    for (int i = 0; i <= num_segments; ++i) {
        float y = -lens_half_height + (2.0f * lens_half_height * i / num_segments);
        float x_bulge = sqrt(arc_radius * arc_radius - y * y) -
                        sqrt(arc_radius * arc_radius - lens_half_height * lens_half_height);
        x_coords.push_back(x_bulge);
        y_coords.push_back(y);
    }

    if (isConvex) {
        // Translucent fill (convex is a simple, symmetric shape)
        glColor4f(0.6f, 0.8f, 1.0f, 0.25f);
        glBegin(GL_POLYGON);
        for (size_t i = 0; i < x_coords.size(); ++i)
            glVertex2f(x_coords[i], y_coords[i]);
        for (int i = (int)x_coords.size() - 1; i >= 0; --i)
            glVertex2f(-x_coords[i], y_coords[i]);
        glEnd();

        // Outline
        glColor3f(0.8f, 0.9f, 1.0f);
        glLineWidth(2.5f);
        glBegin(GL_LINE_LOOP);
        for (size_t i = 0; i < x_coords.size(); ++i)
            glVertex2f(x_coords[i], y_coords[i]);
        for (int i = (int)x_coords.size() - 1; i >= 0; --i)
            glVertex2f(-x_coords[i], y_coords[i]);
        glEnd();
    } else {
        // --- FIXED: use triangle strip for concave fill to avoid polygon filling artifacts ---
        const float edge_thickness = 40.0f;
        glColor4f(0.6f, 0.8f, 1.0f, 0.25f);

        // Build a triangle strip between the right and left inner curves
        glBegin(GL_TRIANGLE_STRIP);
        for (size_t i = 0; i < x_coords.size(); ++i) {
            float xr = edge_thickness - x_coords[i];
            float xl = -(edge_thickness - x_coords[i]);
            float y = y_coords[i];
            glVertex2f(xr, y); // right side vertex
            glVertex2f(xl, y); // left side vertex
        }
        glEnd();

        // Outline the right inner curve
        glColor3f(0.8f, 0.9f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < x_coords.size(); ++i) {
            glVertex2f(edge_thickness - x_coords[i], y_coords[i]);
        }
        glEnd();

        // Outline the left inner curve
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < x_coords.size(); ++i) {
            glVertex2f(-(edge_thickness - x_coords[i]), y_coords[i]);
        }
        glEnd();
    }

    glLineWidth(1.0f);
}

void drawObject() {
    // Shadow
    glColor4f(0.0f, 0.0f, 0.0f, 0.3f);
    glBegin(GL_LINES);
    glVertex2f(-objectDistance, 0);
    glVertex2f(-objectDistance, OBJECT_HEIGHT - 3);
    glEnd();

    // Arrow
    glColor3f(0.0f, 1.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-objectDistance, 0);
    glVertex2f(-objectDistance, OBJECT_HEIGHT);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-objectDistance, OBJECT_HEIGHT);
    glVertex2f(-objectDistance - 5, OBJECT_HEIGHT - 10);
    glVertex2f(-objectDistance + 5, OBJECT_HEIGHT - 10);
    glEnd();

    glLineWidth(1.0f);
}

void drawImageAndRays() {
    float imageDistance, imageHeight;
    bool isVirtual = false;

    if (std::abs(objectDistance - focalLength) < 1.0f)
        imageDistance = 99999;
    else
        imageDistance = (objectDistance * focalLength) / (objectDistance - focalLength);

    imageHeight = OBJECT_HEIGHT * (-imageDistance / objectDistance);

    if (isConvex) {
        if (objectDistance < focalLength) isVirtual = true;
    } else {
        isVirtual = true;
        float f = -focalLength;
        imageDistance = (objectDistance * f) / (objectDistance - f);
        imageHeight = OBJECT_HEIGHT * (-imageDistance / objectDistance);
    }

    // Rays
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(-objectDistance, OBJECT_HEIGHT); glVertex2f(0, OBJECT_HEIGHT);
    if (isConvex)
        glVertex2f(0, OBJECT_HEIGHT), glVertex2f(imageDistance, imageHeight);
    else
        glVertex2f(0, OBJECT_HEIGHT), glVertex2f(screenWidth/2.0f, (OBJECT_HEIGHT/focalLength)*(screenWidth/2.0f) + OBJECT_HEIGHT);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-objectDistance, OBJECT_HEIGHT); glVertex2f(imageDistance, imageHeight);
    glEnd();

    if (isVirtual) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0xAAAA);
        glColor3f(1.0f, 0.5f, 0.0f);

        glBegin(GL_LINES);
        if (isConvex)
            glVertex2f(0, OBJECT_HEIGHT), glVertex2f(imageDistance, imageHeight);
        else
            glVertex2f(0, OBJECT_HEIGHT), glVertex2f(-focalLength, 0);
        glVertex2f(0, 0); glVertex2f(-objectDistance, OBJECT_HEIGHT);
        glEnd();
        glDisable(GL_LINE_STIPPLE);
    }

    // Image shadow
    glColor4f(0.0f, 0.0f, 0.0f, 0.3f);
    glBegin(GL_LINES);
    glVertex2f(imageDistance, 0);
    glVertex2f(imageDistance, imageHeight - 3);
    glEnd();

    // Image arrow
    if (isVirtual) {
        glColor3f(0.0f, 0.7f, 0.0f);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x0101);
    } else {
        glColor3f(0.0f, 1.0f, 0.0f);
    }

    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(imageDistance, 0);
    glVertex2f(imageDistance, imageHeight);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(imageDistance, imageHeight);
    glVertex2f(imageDistance - 5, imageHeight + (imageHeight > 0 ? -10 : 10));
    glVertex2f(imageDistance + 5, imageHeight + (imageHeight > 0 ? -10 : 10));
    glEnd();

    glDisable(GL_LINE_STIPPLE);
    glLineWidth(1.0f);
}

// --- GLUT Callback Functions ---

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawGrid();
    drawOpticalAxis();
    drawFocalPoints();
    drawObject();
    drawImageAndRays();
    drawLens();

    // UI
    glColor3f(1.0f, 1.0f, 1.0f);
    std::string lensType = isConvex ? "Convex Lens" : "Concave Lens";
    drawTextLarge(-screenWidth/2.0f + 20, screenHeight/2.0f - 30, lensType + "  [C/V to switch]");
    drawText(-screenWidth/2.0f + 20, screenHeight/2.0f - 50, "Focal Length: " + std::to_string((int)focalLength) + "  [+ / -]");
    drawText(-screenWidth/2.0f + 20, screenHeight/2.0f - 70, "Object Distance: " + std::to_string((int)objectDistance) + "  [A / D]");

    glutSwapBuffers();
}

void reshape(int w, int h) {
    screenWidth = w;
    screenHeight = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-w / 2.0, w / 2.0, -h / 2.0, h / 2.0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'c': case 'C': isConvex = false; break;
        case 'v': case 'V': isConvex = true; break;
        case '+': case '=': focalLength += 10.0f; break;
        case '-': case '_': if (focalLength > 20.0f) focalLength -= 10.0f; break;
        case 'd': case 'D': objectDistance += 10.0f; break;
        case 'a': case 'A': if (objectDistance > 10.0f) objectDistance -= 10.0f; break;
        case 27: case 'q': case 'Q': exit(0); break;
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glLineWidth(1.5f);
}

// --- Main Function ---
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ray Optics Simulation - Enhanced Visuals (Fixed Concave Fill)");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
