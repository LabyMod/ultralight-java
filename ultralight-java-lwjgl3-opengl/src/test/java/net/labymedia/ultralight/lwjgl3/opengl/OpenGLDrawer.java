/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
