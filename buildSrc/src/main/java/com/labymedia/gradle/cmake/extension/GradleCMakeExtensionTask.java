package com.labymedia.gradle.cmake.extension;


import org.gradle.api.Task;

public class GradleCMakeExtensionTask extends GradleCMakeExtensionGlobal {
  private String generator;
  private String target;

  public GradleCMakeExtensionTask(Task task) {
    super(task);
  }

  public void setGenerator(String generator) {
    this.generator = generator;
  }

  public void setTarget(String target) {
    this.target = target;
  }

  public String getGenerator() {
    return this.generator;
  }

  public String getTarget() {
    return this.target;
  }
}
