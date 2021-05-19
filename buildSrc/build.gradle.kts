plugins {
    id("java-gradle-plugin")
    `kotlin-dsl`
}

repositories {
    mavenCentral()
}

gradlePlugin {
    plugins {
        create("CMakeGradle") {
            id = "com.labymedia.cmake-gradle"
            implementationClass = "com.labymedia.gradle.cmake.GradleCMakePlugin"
        }
    }
}