
```cpp
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0, 10, 0, 10);
```

are **the key to why your triangle becomes visible**.

---

### 1. `glMatrixMode(GL_PROJECTION)`

Think of OpenGL as having **two main matrices** that transform your scene:

* **ModelView Matrix** → handles moving/rotating/scaling objects in the scene.
* **Projection Matrix** → defines the "camera lens" — how the scene is projected onto the screen (orthographic, perspective, zoom level, etc.).

By calling `glMatrixMode(GL_PROJECTION);`, you're telling OpenGL:
➡️ "Hey, I want to change the camera setup, not move the objects themselves."

---

### 2. `glLoadIdentity()`

When you call this, you're saying:
➡️ "Reset the current matrix to a neutral state (identity matrix), with no scaling, rotation, or translation."

Without this, if there was any old transformation left over, it would mess up your coordinate system.
So this is like **starting with a blank sheet before setting up your view**.

---

### 3. `gluOrtho2D(0, 10, 0, 10)`

This is the **real magic** for 2D rendering.
It sets up an **orthographic projection** (no perspective distortion) where:

* The left edge of the window corresponds to **x = 0**
* The right edge corresponds to **x = 10**
* The bottom edge corresponds to **y = 0**
* The top edge corresponds to **y = 10**

So now your triangle with vertices `(2,2), (5,5), (8,2)` is placed nicely inside this range and will be visible.

---

### 📊 Mental Model:

Imagine your OpenGL window as a **graph paper**.
By default, OpenGL's "graph paper" only goes from **-1 to +1** in both x and y directions — super tiny area.
`gluOrtho2D(0, 10, 0, 10)` says:
➡️ "Replace that tiny piece of graph paper with a bigger one that goes from 0 to 10."

