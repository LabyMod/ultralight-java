plugins {
    id("java-gradle-plugin")
}


gradlePlugin {
    plugins {
        create("CMakeGradle") {
            id = "com.labymedia.cmake-gradle"
            implementationClass = "com.labymedia.gradle.cmake.GradleCMakePlugin"
        }
    }
}