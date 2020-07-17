package net.janrupf.ultralight;

import net.janrupf.ultralight.os.Architecture;
import net.janrupf.ultralight.os.OperatingSystem;

import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

public class UltralightJava {
    /**
     * Loads the native library from the java native library path.
     *
     * @throws UnsatisfiedLinkError If the library fails to load
     */
    public static void loadNative() throws UnsatisfiedLinkError {
        System.loadLibrary("ultralight-java");
    }

    /**
     * Extracts the native JNI library to the given directory.
     *
     * @param nativesDir The directory to extract the JNI natives to
     * @throws UltralightLoadException If extraction fails
     */
    public static void extractNativeLibrary(Path nativesDir) throws UltralightLoadException {
        OperatingSystem operatingSystem = OperatingSystem.get();
        Architecture architecture = Architecture.get();

        // Construct a library name containing the architecture
        String nameWithArchitecture = operatingSystem.mapLibraryName("ultralight-java-" + architecture.getBits());

        try {
            // Try to extract the library with the architecture included in the name
            // This will be the case in production environments
            if(extractResource(
                    "/native-binaries/" + nameWithArchitecture,
                    nativesDir.resolve(nameWithArchitecture)
            )) {
                // Success, the library has been extracted
                return;
            }
        } catch (IOException e) {
            throw new UltralightLoadException("Failed to extract native library", e);
        }

        // Construct a library name without the architecture
        String nameWithoutArchitecture = operatingSystem.mapLibraryName("ultralight-java");

        try {
            // Try to to extract the library without the architecture in the name
            // This will be the case in development environments
            if(extractResource(
                    "/native-binaries/" + nameWithoutArchitecture,
                    nativesDir.resolve(nameWithoutArchitecture)
            )) {
                // Success, the library has been extracted
                return;
            }
        } catch (IOException e) {
            throw new UltralightLoadException("Failed to extract native library", e);
        }

        // Unsupported platform
        throw new UltralightLoadException("No library found for the requested architecture and operating system");
    }

    /**
     * Extracts the given resource path to the given file. Parent directories are created as required.
     *
     * @param resourcePath The resource path to extract
     * @param targetFile The path to the file to extract the resource to
     * @return {@code true} if the resource has been extracted, {@code false} otherwise
     * @throws IOException If an I/O error occurs during extraction
     */
    private static boolean extractResource(String resourcePath, Path targetFile) throws IOException {
        try(InputStream stream = UltralightJava.class.getResourceAsStream(resourcePath)) {
            if(stream == null) {
                return false;
            }

            // Retrieve the target directory
            Path targetDir = targetFile.getParent();
            if(!Files.isDirectory(targetDir)) {
                // Create the target directory
                Files.createDirectories(targetDir);
            }

            // Copy the resource stream
            Files.copy(stream, targetFile, StandardCopyOption.REPLACE_EXISTING);
        }

        return true;
    }

    /**
     * Loads the ultralight libraries from the given directory.
     *
     * @param nativesDir The directory to extract the natives to
     * @throws UltralightLoadException If loading ultralight fails
     */
    public static void load(Path nativesDir) throws UltralightLoadException {
        OperatingSystem operatingSystem = OperatingSystem.get();
        Architecture architecture = Architecture.get();

        Path ultralightLibrary =
                determineLibraryPath(nativesDir, "ultralight-java", operatingSystem, architecture);

        try {
            System.load(ultralightLibrary.toAbsolutePath().toString());
        } catch (UnsatisfiedLinkError e) {
            throw new UltralightLoadException("Failed to load native ultralight-java library", e);
        }
    }

    /**
     * Tries to find the given library in the given directory.
     *
     * @param nativesDir      The directory to search the library in
     * @param libraryName     The name of the library to find
     * @param operatingSystem The operating system to find the library for
     * @param architecture    The architecture to find the library for
     * @return A path to the found library
     * @throws UltralightLoadException If the library could not be found
     */
    private static Path determineLibraryPath(
            Path nativesDir,
            String libraryName,
            OperatingSystem operatingSystem,
            Architecture architecture
    ) throws UltralightLoadException {
        // First try to find the library from ${nativesDir}/${prefix}${name}-${bits}${suffix}
        Path pathWithArchitecture = nativesDir.resolve(
                operatingSystem.mapLibraryName(libraryName + "-" + architecture.getBits()));
        if (Files.isRegularFile(pathWithArchitecture)) {
            // Found it
            return pathWithArchitecture;
        }

        // Then try to find the library from ${nativesDir}/${prefix}${name}${suffix}
        Path pathWithoutArchitecture = nativesDir.resolve(operatingSystem.mapLibraryName(libraryName));
        if (Files.isRegularFile(pathWithoutArchitecture)) {
            return pathWithoutArchitecture;
        }

        throw new UltralightLoadException("Failed to find library " + libraryName);
    }
}
