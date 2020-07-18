package net.janrupf.ultralight.lwjgl3.opengl;

import static org.lwjgl.opengl.GL21.*;

/**
 * Class used for drawing something on the OpenGL context for testing.
 */
public class OpenGLDrawer {
    /**
     * Draws something on the context for testing.
     */
    public void draw() {
        glBegin(GL_TRIANGLES);

        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-1.0f, -1.0f);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(1.0f, -1.0f);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.0f, 1.0f);

        glEnd();
    }
}
