#pragma once

#include <jni.h>

namespace ultralight_java {
    /**
     * Wrapper for JNI references with auto deletion
     */
    class JNIReferenceWrapper {
    protected:
        jobject reference;

    public:
        // Delete copy constructor and copy assignment operator
        JNIReferenceWrapper(const JNIReferenceWrapper &other) = delete;

        JNIReferenceWrapper &operator=(const JNIReferenceWrapper &other) = delete;

        // Implement move constructor, but delete move assignment operator
        JNIReferenceWrapper(JNIReferenceWrapper &&other) noexcept;

        JNIReferenceWrapper &operator=(JNIReferenceWrapper &&other) = delete;

        /**
         * Constructs a new JNIReferenceWrapper wrapping a jobject.
         *
         * @param env The environment to use for wrapping the reference
         * @param reference The reference to wrap, a global ref will be created for it
         */
        explicit JNIReferenceWrapper(JNIEnv *env, jobject reference);

        /**
         * Destructs the JNIReferenceWrapper and deletes the underlying reference.
         */
        virtual ~JNIReferenceWrapper();

        /**
         * Retrieves the wrapped reference.
         *
         * @return The wrapped reference
         */
        operator jobject();

        /**
         * Retrieves the wrapped reference.
         *
         * @return The wrapped reference
         */
        jobject operator*();

        /**
         * Retrieves the wrapped reference.
         *
         * @return The wrapped reference
         */
        jobject get();
    };
}