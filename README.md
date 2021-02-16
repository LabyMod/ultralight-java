[Ultralight Discord](https://chat.ultralig.ht) | [Ultralight Upstream](https://github.com/Ultralight-ux/Ultralight) |
![CI](https://github.com/LabyMod/ultralight-java/workflows/CI/badge.svg)
# Ultralight Java
###### Java wrapper for the [Ultralight](https://ultralig.ht) web engine

## Discord Chat
Got a question about Ultralight or Ultralight Java? Come chat with us on Discord!

[**Join the #java channel on the Ultralight Discord!** <img src="artwork/public/discord.svg">](https://chat.ultralig.ht)

# About
Ultralight Java is a complete wrapper over the Ultralight API including the JavaScriptCore API. Ultralight Java allows
you to use the powerful Ultralight engine without leaving the Java platform.

## What is Ultralight?
Ultralight is a lightweight, cross-platform, HTML rendering engine for desktop apps and games. It supports most modern 
HTML5 and JavaScript features while still remaining small in binary size and memory usage.

See the [Ultralight Upstream](https://github.com/Ultralight-ux/Ultralight) for more information.

## What makes Ultralight Java different from JCEF or JavaFX WebView?
Other than JCEF and the JavaFX WebView, Ultralight aims to be a lightweight UI engine. While it is capable of displaying
fully fledged websites, it does so without using as much memory and CPU. It is the ideal solution for a simple, HTML
based UI, while staying with the Java platform.

Moreover, the databind API is capable of translating Java objects directly to JavaScript, allowing interop and Java 
calls directly from within your JavaScript code, making integration with the UI even easier.

# Using the library
The library requires the Ultralight SDK, which can be downloaded 
[here](https://github.com/ultralight-ux/Ultralight/blob/master/README.md#getting-the-latest-sdk). The native binaries of
the SDK need to be available at runtime and can be loaded using the provided Java methods.

### Maven
Using the library with maven:
```xml
<dependency>
  <groupId>com.labymedia</groupId>
  <artifactId>ultralight-java-base</artifactId>
  <!-- Replace with latest version from https://search.maven.org/artifact/com.labymedia/ultralight-java-base !-->
  <version>0.3.4</version>
</dependency>
```

If you need Javascript interop:
```xml
<dependency>
  <groupId>com.labymedia</groupId>
  <artifactId>ultralight-java-databind</artifactId>
  <!-- Replace with latest version from https://search.maven.org/artifact/com.labymedia/ultralight-java-databind !-->
  <version>0.3.4</version>
</dependency>
```

### Gradle
```kotlin
dependencies {
  // Replace with latest version from https://search.maven.org/artifact/com.labymedia/ultralight-java-base
  implementation("com.labymedia", "ultralight-java-base", "0.3.4")

  // // Replace with latest version from https://search.maven.org/artifact/com.labymedia/ultralight-java-databind
  // Remove the comments if you need Javascript interop
  // implementation("com.labymedia", "ultralight-java-databind", "0.3.4")
}
```

The latest version can usually also be found in the 
[VERSION file on the master branch](https://github.com/LabyMod/ultralight-java/blob/master/VERSION).

**You will also need to download the Ultralight SDK and extract the `bin` folder into a directory given to
the Library in order to provide the natives!**

See [Examples](https://github.com/LabyMod/ultralight-java/tree/develop/example) for more info.

# Sponsors
[<img src="https://www.yourkit.com/images/yklogo.png"/>](https://www.yourkit.com/)

YourKit supports open source projects with innovative and intelligent tools for monitoring and profiling Java and .NET applications.
YourKit is the creator of [YourKit Java Profiler](https://www.yourkit.com/java/profiler/), [YourKit .NET Profiler](https://www.yourkit.com/.net/profiler/), and [YourKit YouMonitor](https://www.yourkit.com/youmonitor/).

# Licensing
For Ultralight Java [LGPLv3](https://www.gnu.org/licenses/lgpl-3.0.en.html) is being used, however, Ultralight itself is
licensed under a custom proprietary license. See
[Ultralight licensing](https://github.com/ultralight-ux/Ultralight/blob/master/README.md#licensing) for further 
information.
