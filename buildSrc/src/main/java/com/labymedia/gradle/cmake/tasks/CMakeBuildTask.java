package com.labymedia.gradle.cmake.tasks;


import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import com.labymedia.gradle.cmake.GradleLogOutputStream;
import com.labymedia.gradle.cmake.extension.GradleCMakeExtensionTask;
import org.gradle.api.DefaultTask;
import org.gradle.api.GradleException;
import org.gradle.api.Task;
import org.gradle.api.logging.LogLevel;
import org.gradle.api.tasks.InputDirectory;
import org.gradle.api.tasks.OutputDirectory;
import org.gradle.api.tasks.TaskAction;

public class CMakeBuildTask extends DefaultTask {
  private File sourceDirectory;
  private File generationDirectory;
  private GradleCMakeExtensionTask extension = this.getExtensions().create("cmake", GradleCMakeExtensionTask.class, this);

  public CMakeBuildTask() {
    this.getOutputs().upToDateWhen(this::upToDate);
  }

  @InputDirectory
  public File getSourceDirectory() {
    return this.sourceDirectory;
  }

  @OutputDirectory
  public File getGenerationDirectory() {
    return this.generationDirectory;
  }

  public void setSourceDirectory(File sourceDirectory) {
    this.sourceDirectory = sourceDirectory;
  }

  public void setGenerationDirectory(File generationDirectory) {
    this.generationDirectory = generationDirectory;
  }

  @TaskAction
  private void build() throws IOException, InterruptedException {
    this.generate();
    this.compile();
  }

  private void generate() throws IOException, InterruptedException {
    List<String> commandLine = new ArrayList<>();
    commandLine.add(this.extension.getExecutable());
    commandLine.add(this.sourceDirectory.getAbsolutePath());
    if (this.extension.getGenerator() != null) {
      commandLine.add("-G" + this.extension.getGenerator());
    }

    commandLine.addAll(this.extension.getArguments());
    this.extension.getVariables().entrySet().stream().map((entry) -> entry.getValue().isPresent() ? "-D" + entry.getKey() + "=" + ((Optional)entry.getValue()).get() : "-D" + entry.getKey()).forEach(commandLine::add);
    Process cmakeProcess = (new ProcessBuilder()).command(commandLine).directory(this.generationDirectory).start();
    Thread outRedirectThread = this.redirectStream(cmakeProcess.getInputStream(), new GradleLogOutputStream("CMake | Generating | Info", this.getLogger(), LogLevel.LIFECYCLE));
    Thread errRedirectThread = this.redirectStream(cmakeProcess.getErrorStream(), new GradleLogOutputStream("CMake | Generating | Erro", this.getLogger(), LogLevel.ERROR));
    if (cmakeProcess.waitFor() != 0) {
      throw new GradleException("CMake exited with non 0 exit code while generating build system");
    } else {
      outRedirectThread.join();
      errRedirectThread.join();
    }
  }

  private void compile() throws IOException, InterruptedException {
    List<String> commandLine = new ArrayList();
    commandLine.add(this.extension.getExecutable());
    commandLine.add("--build");
    commandLine.add(".");
    if (this.extension.getTarget() != null) {
      commandLine.add("--target");
      commandLine.add(this.extension.getTarget());
    }

    if (this.extension.getBuildArguments().size() > 0) {
      commandLine.add("--");
      commandLine.addAll(this.extension.getBuildArguments());
    }

    Process cmakeProcess = (new ProcessBuilder()).command(commandLine).directory(this.generationDirectory).start();
    Thread outRedirectThread = this.redirectStream(cmakeProcess.getInputStream(), new GradleLogOutputStream("CMake | Compiling | Info", this.getLogger(), LogLevel.LIFECYCLE));
    Thread errRedirectThread = this.redirectStream(cmakeProcess.getErrorStream(), new GradleLogOutputStream("CMake | Compiling | Erro", this.getLogger(), LogLevel.ERROR));
    if (cmakeProcess.waitFor() != 0) {
      throw new GradleException("CMake exited with non 0 exit code while compiling");
    } else {
      outRedirectThread.stop();
      errRedirectThread.stop();
    }
  }

  private <T extends Task> boolean upToDate(T spec) {
    return false;
  }

  private Thread redirectStream(InputStream in, OutputStream out) {
    Thread t = new Thread(() -> {
      while(true) {
        try {
          int read = in.read();
          if (read != -1) {
            out.write(read);
            continue;
          }
        } catch (IOException var4) {
          var4.printStackTrace();
        }

        return;
      }
    });
    t.start();
    return t;
  }
}
