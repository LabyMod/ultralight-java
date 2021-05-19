import org.apache.tools.ant.taskdefs.condition.Os

plugins {
    id("application")
    id("java-library")
}

group = "com.labymedia"

fun lwjglClassifier(): String {
    if (Os.isFamily(Os.FAMILY_WINDOWS)) {
        return "natives-windows"
    } else if (Os.isFamily(Os.FAMILY_MAC)) {
        return "natives-macos"
    } else if (Os.isFamily(Os.FAMILY_UNIX)) {
        return "natives-linux"
    } else {
        throw UnsupportedOperationException("This OS is not supported")
    }
}

repositories {
    mavenCentral()
}

dependencies {
    implementation(group = "org.lwjgl", name = "lwjgl", version = "3.2.2")
    runtimeOnly(group = "org.lwjgl", name = "lwjgl", version = "3.2.2", classifier = lwjglClassifier())

    implementation(group = "org.lwjgl", name = "lwjgl-opengl", version = "3.2.2")
    runtimeOnly(group = "org.lwjgl", name = "lwjgl-opengl", version = "3.2.2", classifier = lwjglClassifier())

    implementation(group = "org.lwjgl", name = "lwjgl-glfw", version = "3.2.2")
    runtimeOnly(group = "org.lwjgl", name = "lwjgl-glfw", version = "3.2.2", classifier = lwjglClassifier())

    implementation(project(":ultralight-java-base"))
    implementation(project(":ultralight-java-databind"))
    implementation(project(":ultralight-java-gpu"))
    implementation(project(":ultralight-java-glfw-opengl-util"))

    if (project.path != ":example:example-base") {
        implementation(project(":example:example-base"))
    }
}

fun getBitName(): String {
    return if (System.getProperty("os.arch", "?").contains("64") ||
        System.getProperty("sun.arch.data.model", "?").contains("64")
    ) {
        "64"
    } else {
        "32"
    }
}

fun ultralightOsIdentifier(): String {
    return when {
        Os.isFamily(Os.FAMILY_WINDOWS) -> {
            "win-x${getBitName()}"
        }
        Os.isFamily(Os.FAMILY_MAC) -> {
            "mac-x${getBitName()}"
        }
        Os.isFamily(Os.FAMILY_UNIX) -> {
            "linux-x${getBitName()}"
        }
        else -> {
            throw UnsupportedOperationException("This OS is not supported")
        }
    }
}

val runDir = file ("run")
if (!runDir.exists() && !runDir.mkdirs()) {
    throw GradleException ("Failed to create run directory")
}


tasks.create("copyResources", Copy::class){
    println(File(project(":ultralight-java-native").buildDir, "cmake-gen-${ultralightOsIdentifier()}/ultralight-${ultralightOsIdentifier()}/bin"))
    from(File(project(":ultralight-java-native").buildDir, "cmake-gen-${ultralightOsIdentifier()}/ultralight-${ultralightOsIdentifier()}/bin"))
    include( "**/*.dll", "**/*.so", "**/*.dylib", "resources/*")
    into(runDir)
}

tasks.getByName("run", JavaExec::class){
    workingDir = runDir
    dependsOn("copyResources")
}