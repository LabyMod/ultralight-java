package net.janrupf.ultralight.os;

import java.util.function.Function;

/**
 * Enum representing know operating systems.
 */
public enum OperatingSystem {
    /**
     * Some kind of Linux or Unix platform.
     */
    LINUX(s -> "lib" + s + ".so"),

    /**
     * Some kind of Windows platform.
     */
    WINDOWS(s -> s + ".dll"),

    /**
     * Some kind of MacOS platform
     */
    MAC_OS(s -> "lib" + s + ".dylib"),

    /**
     * OS which name could not be identified. Mapping library names on it will
     * map them like on linux, since this has the best chance of succeeding.
     */
    UNKNOWN(LINUX::mapLibraryName);

    private final Function<String, String> libraryNameMapper;

    /**
     * Constructs a new {@link OperatingSystem} with the given library name mapper.
     *
     * @param libraryNameMapper Function used to map library names to the platform name
     */
    OperatingSystem(Function<String, String> libraryNameMapper) {
        this.libraryNameMapper = libraryNameMapper;
    }

    /**
     * Maps the given library name to the platform library name.
     *
     * @param libraryName The library name to map
     * @return The mapped name
     */
    public String mapLibraryName(String libraryName) {
        return libraryNameMapper.apply(libraryName);
    }

    /**
     * Guesses the platform this application is running on.
     *
     * @return A best guess of the platform this application is running on
     */
    public static OperatingSystem get() {
        String systemName = System.getProperty("os.name", "unknown").toLowerCase();

        if(systemName.contains("nux") || systemName.contains("nix")) {
            return LINUX;
        } else if(systemName.contains("mac")) {
            return MAC_OS;
        } else if(systemName.contains("windows")) {
            return WINDOWS;
        } else {
            return UNKNOWN;
        }
    }
}
