/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2021 LabyMedia and contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "ultralight_java/gpudriver/com_labymedia_ultralight_gpu_UltralightGPUDriverNativeUtil.h"
#include "ultralight_java/gpudriver/gl/GPUContextGL.h"
#include "ultralight_java/gpudriver/gl/GPUDriverGL.h"
#include "ultralight_java/gpudriver/gl/glad.h"

JNIEnv *env;

void *JniLoaderFunc(const char *name) {
    jclass glfw = env->FindClass("org/lwjgl/glfw/GLFW");
    jmethodID nglfwGetProcAddress = env->GetStaticMethodID(glfw, "nglfwGetProcAddress", "(J)J");

    jlong res = env->CallStaticLongMethod(glfw, nglfwGetProcAddress, (jlong) name);
    return (void *) res;
}

extern "C" jint JNI_OnLoad(JavaVM *jvm, void *) {
    jvm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_8);

    return JNI_VERSION_1_8;
}

JNIEXPORT jlong JNICALL Java_com_labymedia_ultralight_gpu_UltralightGPUDriverNativeUtil_createOpenGLContext(
    JNIEnv *e, jobject, jlong window, jboolean msaa) {

    env = e;
    gladLoadGLLoader((GLADloadproc) JniLoaderFunc);

    auto *context = new ultralight::GPUContextGL((void *) window, msaa);

    return (jlong) context;
}

JNIEXPORT jlong JNICALL Java_com_labymedia_ultralight_gpu_UltralightGPUDriverNativeUtil_getDriverFromContext(
    JNIEnv *, jobject, jlong contextHandle) {
    auto *context = (ultralight::GPUContextGL *) contextHandle;

    return (jlong) context->driver();
}

JNIEXPORT void JNICALL
Java_com_labymedia_ultralight_gpu_UltralightGPUDriverNativeUtil_beginSynchronize(JNIEnv *, jobject, jlong handle) {
    auto *driver = (ultralight::GPUDriverGL *) handle;
    driver->BeginSynchronize();
}

JNIEXPORT void JNICALL
Java_com_labymedia_ultralight_gpu_UltralightGPUDriverNativeUtil_endSynchronize(JNIEnv *, jobject, jlong handle) {
    auto *driver = (ultralight::GPUDriverGL *) handle;
    driver->EndSynchronize();
}

JNIEXPORT jboolean JNICALL
Java_com_labymedia_ultralight_gpu_UltralightGPUDriverNativeUtil_hasCommandsPending(JNIEnv *, jobject, jlong handle) {
    auto *driver = (ultralight::GPUDriverGL *) handle;
    return (jboolean) driver->HasCommandsPending();
}

JNIEXPORT void JNICALL
Java_com_labymedia_ultralight_gpu_UltralightGPUDriverNativeUtil_drawCommandList(JNIEnv *, jobject, jlong handle) {
    auto *driver = (ultralight::GPUDriverGL *) handle;
    driver->DrawCommandList();
}

JNIEXPORT void JNICALL Java_com_labymedia_ultralight_gpu_UltralightGPUDriverNativeUtil_setActiveWindow(
    JNIEnv *, jobject, jlong handle, jlong window) {
    auto *driver = (ultralight::GPUDriverGL *) handle;
    driver->GetContext()->set_active_window(reinterpret_cast<void *>(window));
}

JNIEXPORT void JNICALL Java_com_labymedia_ultralight_gpu_UltralightGPUDriverNativeUtil_bindTexture(
    JNIEnv *, jobject, jlong handle, jlong textureId, jlong texture) {
    auto *driver = (ultralight::GPUDriverGL *) handle;
    driver->BindTexture(textureId, texture);
}
