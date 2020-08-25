package net.labymedia.ultralight.lwjgl3.opengl;

import static org.lwjgl.opengl.GL21.*;

/**
 * Class used for drawing something on the OpenGL context for testing.
 */
public class OpenGLDrawer {
    private float rotation;

    /**
     * Draws something on the context for testing.
     */
    public void draw() {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_ALPHA_TEST);

        glPushMatrix();
        glRotatef(rotation++, 0, 0, 1);
        glBegin(GL_TRIANGLES);


        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-1.0f, -1.0f);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(1.0f, -1.0f);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.0f, 1.0f);

        glEnd();
        glPopMatrix();

    }
}
