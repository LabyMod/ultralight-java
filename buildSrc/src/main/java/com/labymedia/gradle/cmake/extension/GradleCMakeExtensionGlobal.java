package com.labymedia.gradle.cmake.extension;


import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import javax.annotation.Nonnull;
import org.gradle.api.Project;
import org.gradle.api.Task;

public class GradleCMakeExtensionGlobal {
  public static final String EXTENSION_NAME = "cmake";
  private final Project project;
  private final Task task;
  private String executable;
  private Map<String, Optional<String>> variables;
  private List<String> arguments;
  private List<String> buildArguments;

  public GradleCMakeExtensionGlobal(Project project) {
    this.project = project;
    this.task = null;
    this.executable = "cmake";
    this.variables = new HashMap();
    this.arguments = new ArrayList();
    this.buildArguments = new ArrayList();
  }

  public GradleCMakeExtensionGlobal(Task task) {
    this.project = null;
    this.task = task;
    this.buildArguments = new ArrayList(this.getGlobalExentsion().buildArguments);
    this.arguments = new ArrayList(this.getGlobalExentsion().arguments);
    this.variables = new HashMap(this.getGlobalExentsion().variables);
  }

  public void setExecutable(String executable) {
    this.executable = executable;
  }

  public String getExecutable() {
    return this.executable != null ? this.executable : this.getGlobalExentsion().executable;
  }

  public void setVariables(Map<String, Optional<String>> variables) {
    this.variables = variables;
  }

  public void addVariable(@Nonnull String key) {
    this.variables.put(key, Optional.empty());
  }

  public void addVariable(@Nonnull String key, @Nonnull String value) {
    this.variables.put(key, Optional.of(value));
  }

  public void variable(@Nonnull String key) {
    this.variables.put(key, Optional.empty());
  }

  public void variable(@Nonnull String key, @Nonnull String value) {
    this.variables.put(key, Optional.of(value));
  }

  public Map<String, Optional<String>> getVariables() {
    return this.variables;
  }

  public void setArguments(List<String> arguments) {
    this.arguments = arguments;
  }

  public List<String> getArguments() {
    return this.arguments;
  }

  public void arguments(@Nonnull String... args) {
    this.arguments.addAll(Arrays.asList(args));
  }

  public void argument(@Nonnull String arg) {
    this.arguments.add(arg);
  }

  public void setBuildArguments(@Nonnull List<String> buildArguments) {
    this.buildArguments = buildArguments;
  }

  public List<String> getBuildArguments() {
    return this.buildArguments;
  }

  public void buildArguments(String... args) {
    this.buildArguments.addAll(Arrays.asList(args));
  }

  public void buildArgument(@Nonnull String arg) {
    this.buildArguments.add(arg);
  }

  private GradleCMakeExtensionGlobal getGlobalExentsion() {
    if (this.task == null && this.project != null) {
      return (GradleCMakeExtensionGlobal)this.project.getExtensions().getByType(GradleCMakeExtensionGlobal.class);
    } else if (this.project == null && this.task != null) {
      return (GradleCMakeExtensionGlobal)this.task.getProject().getExtensions().getByType(GradleCMakeExtensionGlobal.class);
    } else {
      throw new IllegalStateException("Couldn't determine where to get extension from, not belonging to anything");
    }
  }
}
