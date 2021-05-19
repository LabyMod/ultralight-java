plugins {
    id("java-library")
    id("maven-publish")
}

group = "com.labymedia"

tasks.getByName("jar", Jar::class) {
    manifest {
        attributes(mapOf<String, Any>("Automatic-Module-Name" to "com.labymedia.ultralight"))
    }
}

repositories{
    mavenCentral()
}

dependencies {
    implementation(project(":ultralight-java-base"))
    implementation(project(":ultralight-java-gpu"))

    implementation(group = "org.lwjgl", name = "lwjgl-glfw", version = "3.2.2")
    implementation(group = "org.lwjgl", name = "lwjgl-opengl", version = "3.2.2")
}