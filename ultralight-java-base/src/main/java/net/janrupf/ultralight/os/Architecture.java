package net.janrupf.ultralight.os;

/**
 * Enum representing known computer architectures.
 */
public enum Architecture {
    /**
     * amd64/x86_64 architecture, most common one for home PC's
     */
    AMD64(64),

    /**
     * i386/x86 architecture, most common one for older home PC's
     */
    I386(32),

    /**
     * Unknown architecture
     */
    UNKNOWN(-1);

    private final int bits;

    /**
     * Constructs a new {@link Architecture} with the given amount of bits per pointer.
     *
     * @param bits The amount of bits of a pointer on this architecture
     */
    Architecture(int bits) {
        this.bits = bits;
    }

    /**
     * Retrieves the amount of bits a pointer has on this platform.
     *
     * @return The amount of bits of a pointer, or {@code -1} if unknown
     */
    public int getBits() {
        return bits;
    }

    /**
     * Guesses the architecture this application is running on.
     *
     * @return A best guess of the architecture
     */
    public static Architecture get() {
        String arch = System.getProperty("os.arch", "unknown").toLowerCase();
        String dataModel = System.getProperty("sun.arch.data.model", "unknown").toLowerCase();

        if(arch.equals("unknown") && dataModel.equals("unknown")) {
            return UNKNOWN;
        } else if(arch.contains("64") || dataModel.contains("64")) {
            // TODO: Technically incorrect, for now we just assume its some kind of amd64 PC
            return AMD64;
        } else {
            // TODO: Technically incorrect, for now just assume its some kind of i386 PC
            return I386;
        }
    }
}
