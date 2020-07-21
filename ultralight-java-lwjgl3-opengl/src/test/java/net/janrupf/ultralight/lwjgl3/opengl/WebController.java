package net.janrupf.ultralight.lwjgl3.opengl;

import net.janrupf.ultralight.UltralightPlatform;
import net.janrupf.ultralight.UltralightRenderer;
import net.janrupf.ultralight.UltralightView;
import net.janrupf.ultralight.bitmap.UltralightBitmap;
import net.janrupf.ultralight.bitmap.UltralightBitmapSurface;
import net.janrupf.ultralight.config.FontHinting;
import net.janrupf.ultralight.config.UltralightConfig;
import net.janrupf.ultralight.math.IntRect;

import java.nio.ByteBuffer;

import static org.lwjgl.opengl.GL20.*;

/**
 * Class used for controlling the WebGUI rendered on top of the OpenGL GUI.
 */
public class WebController {
    private final UltralightPlatform platform;
    private final UltralightRenderer renderer;
    private final UltralightView view;
    private final TestViewListener viewListener;
    private final TestLoadListener loadListener;
    private final TestInputAdapter inputAdapter;

    private int glTexture;

    /**
     * Constructs a new {@link WebController} and retrieves the platform.
     *
     * @param cursorManager Cursor manager for callbacks on cursor changes
     */
    public WebController(TestCursorManager cursorManager) {
        this.platform = UltralightPlatform.instance();

        this.platform.setConfig(
                new UltralightConfig()
                        .resourcePath("./resources/")
                        .fontHinting(FontHinting.NORMAL)
                        .deviceScale(1.0)
        );
        this.platform.usePlatformFontLoader();
        this.platform.setFileSystem(new TestFileSystem());
        this.platform.setLogger(new TestLogger());

        this.renderer = UltralightRenderer.create();
        this.renderer.logMemoryUsage();

        this.view = renderer.createView(300, 300, false);
        this.viewListener = new TestViewListener(cursorManager);
        this.view.setViewListener(viewListener);
        this.loadListener = new TestLoadListener();
        this.view.setLoadListener(loadListener);
        this.glTexture = -1;

        this.inputAdapter = new TestInputAdapter(view);
    }

    /**
     * Retrieves the input adapter of this web controller.
     *
     * @return The input adapter of this web controller
     */
    public TestInputAdapter getInputAdapter() {
        return inputAdapter;
    }

    /**
     * Loads the specified URL into this controller.
     *
     * @param url The URL to load
     */
    public void loadURL(String url) {
        this.view.loadURL(url);
    }

    /**
     * Updates and renders the renderer
     */
    public void update() {
        this.renderer.update();
        this.renderer.render();
    }

    /**
     * Resizes the web view.
     *
     * @param width  The new view width
     * @param height The new view height
     */
    public void resize(int width, int height) {
        this.view.resize(width, height);
    }

    /**
     * Render the current image using OpenGL
     */
    public void render() {
        if(glTexture == -1) {
            createGLTexture();
        }

        UltralightBitmapSurface surface = (UltralightBitmapSurface) this.view.surface();
        UltralightBitmap bitmap = surface.bitmap();

        int width = (int) view.width();
        int height = (int) view.height();

        // Prepare OpenGL for 2D textures and bind our texture
        glEnable(GL_TEXTURE_2D);

        IntRect dirtyBounds = surface.dirtyBounds();

        if(dirtyBounds.isValid()) {
            ByteBuffer imageData = bitmap.lockPixels();

            glBindTexture(GL_TEXTURE_2D, this.glTexture);

            if(dirtyBounds.width() == width && dirtyBounds.height() == height) {
                // Update full image
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, imageData);
            } else {
                // Update partial image
                int x = dirtyBounds.x();
                int y = dirtyBounds.y();
                int dirtyWidth = dirtyBounds.width();
                int dirtyHeight = dirtyBounds.height();
                int startOffset = (y * width * 4) + x * 4;

                glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
                glTexSubImage2D(
                        GL_TEXTURE_2D,
                        0,
                        x, y, dirtyWidth, dirtyHeight,
                        GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV,
                        (ByteBuffer) imageData.position(startOffset));
                glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            }

            bitmap.unlockPixels();
            surface.clearDirtyBounds();
        }

        // Set up the OpenGL state for rendering of a fullscreen quad
        glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, this.view.width(), this.view.height(), 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        // Disable lighting and scissoring, they could mess up th renderer
        glLoadIdentity();
        glDisable(GL_LIGHTING);
        glDisable(GL_SCISSOR_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Make sure we draw with a neutral color
        // (so we don't mess with the color channels of the image)
        glColor3f(1, 1, 1);

        glBegin(GL_QUADS);

        // Lower left corner, 0/0 on the screen space, and 0/0 of the image UV
        glTexCoord2i(0, 0);
        glVertex2f(0, 0);

        // Upper left corner
        glTexCoord2f(0, 1);
        glVertex2i(0, height);

        // Upper right corner
        glTexCoord2f(1, 1);
        glVertex2i(width, height);

        // Lower right corner
        glTexCoord2f(1, 0);
        glVertex2i(width, 0);

        glEnd();

        // Restore OpenGL state
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glDisable(GL_TEXTURE_2D);
        glPopAttrib();

    }

    /**
     * Sets up the OpenGL texture for rendering
     */
    private void createGLTexture() {
        glEnable(GL_TEXTURE_2D);
        this.glTexture = glGenTextures();
        glBindTexture(GL_TEXTURE_2D, this.glTexture);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}
