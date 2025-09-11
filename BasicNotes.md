Nice — I’ll walk through **all the essentials** you need so your GLUT + legacy OpenGL program actually shows what you expect. I’ll explain *why* each item matters and give two small, ready-to-run examples:

* a 2D **orthographic** setup (your original approach: `gluOrtho2D(0,6,0,6)`) — recommended if you want world coordinates like `0..6`, and
* a **normalized**-coordinates example that uses the default clip-space `[-1,1]` (so you don’t need to set a projection).

# Essentials (what to do and why)

## 1. `glutInit()` / `glutInitDisplayMode()` / window size & position

* `glutInit(&argc, argv);` must be first.
* **Call `glutInitDisplayMode()` before creating the window** to specify single/double buffering, color/depth buffers:

  * `GLUT_RGB` or `GLUT_DOUBLE | GLUT_RGB`
  * If you do animation, prefer `GLUT_DOUBLE` and `glutSwapBuffers()` in display.
* `glutInitWindowSize()` and `glutInitWindowPosition()` set the initial window. Then `glutCreateWindow()`.

Why: these decide buffer formats and how swapping/clearing works.

## 2. Clear color and clearing buffers

* Set a clear color once in init: `glClearColor(r,g,b,a);` (floats 0..1).
* Always call `glClear(GL_COLOR_BUFFER_BIT)` (and `GL_DEPTH_BUFFER_BIT` if using depth).

Why: without `glClearColor` you get whatever background color the GL implementation left, and you must clear buffers before drawing each frame.

## 3. Projection vs ModelView matrices (and `glMatrixMode`)

* OpenGL has separate matrix stacks:

  * `GL_PROJECTION` — controls the camera’s *projection* (orthographic or perspective).
  * `GL_MODELVIEW` — controls model transforms (translate/rotate/scale) and the view transform.
* Typical pattern:

  ```c
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  /* set projection: gluOrtho2D(...) or gluPerspective(...) */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  ```

Why: if you set the projection while still in `GL_MODELVIEW` or vice-versa you’ll get wrong transforms/visibility.

## 4. Coordinate systems & clipping (the crucial bit)

* There’s a pipeline: object coords → eye coords → clip coords → NDC (Normalized Device Coordinates) → window coords.
* NDC range is **-1..+1** for X,Y (and Z after perspective divide). Anything outside that is clipped/discarded.
* If you want to use world coords like `0..6`, you must create a projection that maps those into NDC, e.g. `gluOrtho2D(0,6,0,6)`.
* Alternatively, use vertex coordinates already in `[-1,1]` (normalized coordinates) and no projection is required.

Why: your original `(0,0),(3,3),(6,0)` is outside `[-1,1]` so it’s invisible unless you set an appropriate projection.

## 5. Viewport & window resize (`glViewport` + reshape callback)

* `glViewport(0, 0, width, height)` maps NDC to window pixels — call it in a `glutReshapeFunc` handler.
* To avoid distortion when resizing, adjust projection using the aspect ratio:

  ```c
  float aspect = (float)w / (float)h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (aspect >= 1.0f)
      gluOrtho2D(0, 6 * aspect, 0, 6);
  else
      gluOrtho2D(0, 6, 0, 6 / aspect);
  glMatrixMode(GL_MODELVIEW);
  ```

Why: otherwise shapes stretch when window aspect ratio changes.

## 6. Colors — use the right function and types

* Use `glColor3f(r,g,b)` with floats in `0.0 .. 1.0`. Example: `glColor3f(1.0f, 0.0f, 0.0f)` = red.
* Alternatively, `glColor3ub(255,0,0)` uses 0..255 unsigned bytes.
* `glColor3i` exists but it’s non-intuitive — stick with `glColor3f` or `glColor3ub`.

Why: you want predictable color ranges.

## 7. Vertex functions / types

* `glVertex2i`, `glVertex2f`, etc. are fine — integers are converted to floats. But be aware of the coordinate range that projection expects.
* Use `glBegin(...)` / `glEnd()` only in legacy immediate mode. They are deprecated in modern OpenGL, but OK for simple demos.

## 8. Buffering: `glFlush()` vs `glutSwapBuffers()`

* For single-buffered (`GLUT_SINGLE`) use `glFlush()` to ensure drawing commands are executed.
* For double-buffered (`GLUT_DOUBLE`) use `glutSwapBuffers()` to present the back buffer to screen.
* `glFinish()` blocks until all commands are complete (rarely needed).

Why: swapping is required to avoid flicker in animations.

## 9. Depth test and 3D

* For 3D, request `GLUT_DEPTH` in `glutInitDisplayMode`, clear depth with `glClear(GL_DEPTH_BUFFER_BIT)` and `glEnable(GL_DEPTH_TEST)`.

## 10. Callbacks you’ll want to know

* `glutDisplayFunc(display);` — required
* `glutReshapeFunc(reshape);` — recommended to handle resize and set viewport/projection
* `glutIdleFunc(idle);` — for continuous updates/animation
* `glutKeyboardFunc(...)`, `glutMouseFunc(...)` — for input

## 11. Linking / compile flags

* On Linux: `gcc program.c -o prog -lGL -lGLU -lglut`
* On macOS use the appropriate frameworks (or FreeGLUT builds).

## 12. Modern OpenGL note (brief)

* Immediate-mode (`glBegin/glEnd`, fixed-function matrices/colors) is deprecated. For production/learning modern OpenGL, use shaders, VBOs, VAOs and set projection/model matrices in shaders. But for small learning demos, the legacy approach is fine.

---

# Two minimal working examples

### A — Orthographic projection (`gluOrtho2D`) with reshape handler (recommended for world coords 0..6)

```c
#include <GL/glut.h>

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 0.0f, 0.0f);  // red
    glBegin(GL_TRIANGLES);
        glVertex2i(0, 0);
        glVertex2i(3, 3);
        glVertex2i(6, 0);
    glEnd();

    glutSwapBuffers();  // use glutSwapBuffers if GLUT_DOUBLE
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    float aspect = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Keep world extent ~0..6 while preserving aspect
    if (aspect >= 1.0f)
        gluOrtho2D(0.0, 6.0 * aspect, 0.0, 6.0);
    else
        gluOrtho2D(0.0, 6.0, 0.0, 6.0 / aspect);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);   // double buffer, RGB
    glutInitWindowSize(600, 400);
    glutCreateWindow("Ortho triangle");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // white background

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
```

### B — Normalized coordinates (no custom projection needed)

This uses default clip-space `[-1,1]` so vertices must be in that range.

```c
#include <GL/glut.h>

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.8f, -0.6f);
        glVertex2f( 0.0f,  0.8f);
        glVertex2f( 0.8f, -0.6f);
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Normalized coords triangle");

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
```

# Quick checklist you can run through when something is invisible

1. Did you set `glutInitDisplayMode()` (double vs single)?
2. Is the triangle’s coordinate range inside the projection/NDC? (If not, set `gluOrtho2D` or change vertex coords.)
3. Did you call `glClear` and `glClearColor`?
4. Did you call `glutSwapBuffers()` for `GLUT_DOUBLE`, or `glFlush()` for `GLUT_SINGLE`?
5. Is the viewport set correctly (especially after a resize)?
6. Are you in the right matrix mode when setting projection/modelview?

---

If you want, I can:

* convert your example to a **double-buffered** version with `glutIdleFunc` for animation, or
* show the **modern OpenGL** equivalent (VBO + shader), which is the right long-term approach.

Which would you like next?
