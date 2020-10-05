[Ultralight Discord](https://chat.ultralig.ht) | [Ultralight Upstream](https://github.com/Ultralight-ux/Ultralight)
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

# Licensing
For Ultralight Java [GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html) is being used, however, Ultralight itself is
licensed under a custom proprietary license. See
[Ultralight licensing](https://github.com/ultralight-ux/Ultralight/blob/master/README.md#licensing) for further 
information.

# Using the library
The library requires the Ultralight SDK, which can be downloaded 
[here](https://github.com/ultralight-ux/Ultralight/blob/master/README.md#getting-the-latest-sdk). The native binaries of
the SDK need to be available at runtime and can be loaded using the provided Java methods.
