package net.janrupf.ultralight.lwjgl3.opengl;

import net.janrupf.ultralight.UltralightJava;
import net.janrupf.ultralight.UltralightLoadException;

import java.io.File;
import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * Entry pointer and controller for the test application.
 */
public class TestRunner {
    public static void main(String[] args) throws UltralightLoadException {
        // Get a directory to put natives into
        Path nativesDir = Paths.get(".");

        // Get the existing native library path
        String libraryPath = System.getProperty("java.library.path");
        if(libraryPath != null) {
            // There is a path set already, append our natives dir
            libraryPath += File.pathSeparator + nativesDir.toAbsolutePath().toString();
        } else {
            // There is no path set, make our natives dir the current path
            libraryPath = nativesDir.toAbsolutePath().toString();
        }

        // Set the path back
        System.setProperty("java.library.path", libraryPath);

        // Extract and load the natives
        UltralightJava.extractNativeLibrary(nativesDir);
        UltralightJava.load(nativesDir);

        // Create and run a simple test application
        TestApplication application = new TestApplication();
        application.centerWindow();
        application.run();

        // The user has requested the application to stop
        application.stop();
    }
}
