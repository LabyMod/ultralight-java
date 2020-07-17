package net.janrupf.ultralight.lwjgl3.opengl;

import java.io.*;

import static org.lwjgl.opengl.GL20.*;

/**
 * Utility class for loading OpenGL shaders.
 */
public class OpenGLShaderLoader {
    /**
     * Creates a new OpenGLShaderLoader instance.
     */
    public OpenGLShaderLoader() {
    }

    /**
     * Reads a shader from an {@link InputStream} and compiles it to the given type.
     *
     * @param stream The stream to read the shader from
     * @param type The type of the shader
     * @return The compiled shader
     * @throws ShaderLoadException If reading from the stream or compilation of the shader fails
     */
    public int compileShader(InputStream stream, int type) throws ShaderLoadException {
        // Copy the stream into the array
        try(ByteArrayOutputStream out = new ByteArrayOutputStream()) {
            // Buffer for transfer
            byte[] buffer = new byte[1024];
            int read;

            // Copy the stream using the buffer
            while ((read = stream.read(buffer)) != - 1) {
                out.write(buffer, 0, read);
            }

            // Convert the stream to a string and compile it using the other method below
            return compileShader(out.toString("UTF-8"), type);
        } catch (IOException e) {
            throw new ShaderLoadException("Failed to read resource", e);
        }
    }

    /**
     * Compiles a shader from the given source code as the given type.
     *
     * @param source The source code of the shader
     * @param type   The type of the shader
     * @return The compiled shader
     * @throws ShaderLoadException If compilation of the shader fails
     */
    public int compileShader(String source, int type) throws ShaderLoadException {
        int shader = glCreateShader(type);
        if (shader == 0) {
            throw new ShaderLoadException("Failed to create shader");
        }

        // Set the shader source and compile the shader
        glShaderSource(shader, source);
        glCompileShader(shader);

        // Check the compile status
        int compileStatus = glGetShaderi(shader, GL_COMPILE_STATUS);
        if (compileStatus != GL_TRUE) {
            String log = glGetShaderInfoLog(shader);
            glDeleteShader(shader);
            throw new ShaderLoadException("Failed to compile shader: " + log);
        }

        return shader;
    }

    /**
     * Links shaders to a program.
     *
     * @param shaders The shaders to link
     * @return The linked program
     *
     * @throws ShaderLoadException If the program fails to link
     */
    public int linkProgram(int... shaders) throws ShaderLoadException {
        int program = glCreateProgram();
        if(program == 0) {
            throw new ShaderLoadException("Failed to create program");
        }

        // Attach all shaders
        for(int shader : shaders) {
            glAttachShader(program, shader);
        }

        // Link the program
        glLinkProgram(program);

        // Check the link status
        int linkStatus = glGetProgrami(program, GL_LINK_STATUS);
        if(linkStatus != GL_TRUE) {
            String log = glGetProgramInfoLog(program);
            glDeleteProgram(program);
            throw new ShaderLoadException("Failed to link program: " + log);
        }

        // Detach all shaders, they are no longer required to be attached
        for(int shader : shaders) {
            glDetachShader(program, shader);
        }

        return program;
    }
}
