package com.labymedia.gradle.cmake;

import com.labymedia.gradle.cmake.extension.GradleCMakeExtensionGlobal;
import org.gradle.api.Plugin;
import org.gradle.api.Project;
import org.gradle.api.tasks.Delete;

import javax.annotation.Nonnull;

public class GradleCMakePlugin implements Plugin<Project> {
    private static GradleCMakePlugin instance;
    private GradleCMakeExtensionGlobal cmakeExtension;

    @Nonnull
    public static GradleCMakePlugin getInstance() {
        return instance;
    }

    public GradleCMakePlugin() {
        instance = this;
    }

    public void apply(@Nonnull Project target) {
        this.cmakeExtension = target.getExtensions().create("cmake", GradleCMakeExtensionGlobal.class, target);
        target.getTasks().register("clean", Delete.class, delete -> delete.delete(target.getBuildDir()));
    }

    public GradleCMakeExtensionGlobal getCMakeExtension() {
        return this.cmakeExtension;
    }
}
