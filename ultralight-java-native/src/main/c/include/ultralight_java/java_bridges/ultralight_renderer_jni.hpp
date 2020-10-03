#pragma once

#include <jni.h>

namespace ultralight_java {
    /**
     * Class for interfacing with ultralight::Renderer from java.
     */
    class UltralightRendererJNI {
    public:
        /**
         * Creates a new ultralight::Renderer object and wraps it into a java object.
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The java class calling the method, should always be UltralightRenderer
         * @return The java renderer object
         */
        static jobject create(JNIEnv *env, jclass caller_class);

        /**
         * Creates a new ultralight::View object and wraps it into a java object.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java object of the ultralight::Renderer creating this view
         * @param width The width of the view in pixels
         * @param height The height of the view in pixels
         * @param transparent Whether the view should be transparent
         * @param force_cpu_renderer Whether this view should always be rendered using the CPU
         * @return The java object wrapping the created view
         */
        static jobject create_view(
            JNIEnv *env,
            jobject instance,
            jlong width,
            jlong height,
            jboolean transparent,
            jboolean force_cpu_renderer);

        /**
         * Calls the ultralight::Renderer::Update method.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java object of the ultralight::Renderer instance
         */
        static void update(JNIEnv *env, jobject instance);

        /**
         * Calls the ultralight::Renderer::Render method.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java object of the ultralight::Renderer instance
         */
        static void render(JNIEnv *env, jobject instance);

        /**
         * Calls the ultralight::Renderer::PurgeMemory method.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java object of the ultralight::Renderer instance
         */
        static void purgeMemory(JNIEnv *env, jobject instance);

        /**
         * Calls the ultralight::Renderer::LogMemoryUsage method.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java object of the ultralight::Renderer instance
         */
        static void logMemoryUsage(JNIEnv *env, jobject instance);
    };
} // namespace ultralight_java