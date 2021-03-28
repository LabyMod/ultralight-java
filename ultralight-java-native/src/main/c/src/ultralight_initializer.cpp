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

#include "ultralight_java/ultralight_initializer.hpp"

#include "ultralight_java/java_bridges/javascript_class_definition_jni.hpp"
#include "ultralight_java/java_bridges/javascript_class_jni.hpp"
#include "ultralight_java/java_bridges/javascript_context_jni.hpp"
#include "ultralight_java/java_bridges/javascript_context_lock_jni.hpp"
#include "ultralight_java/java_bridges/javascript_global_context_jni.hpp"
#include "ultralight_java/java_bridges/javascript_object_jni.hpp"
#include "ultralight_java/java_bridges/javascript_value_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_bitmap_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_bitmap_surface_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_key_event_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_matrix4x4_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_matrix_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_platform_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_ref_ptr_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_renderer_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_surface_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_view_jni.hpp"

#define NATIVE_METHOD(name, signature, method)                                                                         \
    JNINativeMethod {                                                                                                  \
        const_cast<char *>(name), const_cast<char *>(signature), reinterpret_cast<void *>(&method)                     \
    }

namespace ultralight_java {
    void init_runtime_struct() {
        runtime.ultralight_platform.native_methods =
            {NATIVE_METHOD(
                 "instance", "()Lcom/labymedia/ultralight/UltralightPlatform;", UltralightPlatformJNI::instance),
             NATIVE_METHOD(
                 "setConfig",
                 "(Lcom/labymedia/ultralight/config/UltralightConfig;)V",
                 UltralightPlatformJNI::set_config),
             NATIVE_METHOD("usePlatformFontLoader", "()V", UltralightPlatformJNI::use_platform_font_loader),
             NATIVE_METHOD(
                 "usePlatformFileSystem", "(Ljava/lang/String;)V", UltralightPlatformJNI::use_platform_file_system),
             NATIVE_METHOD(
                 "setFileSystem",
                 "(Lcom/labymedia/ultralight/plugin/filesystem/UltralightFileSystem;)V",
                 UltralightPlatformJNI::set_file_system),
             NATIVE_METHOD(
                 "setGPUDriver",
                 "(Lcom/labymedia/ultralight/plugin/render/UltralightGPUDriver;)V",
                 UltralightPlatformJNI::set_gpu_driver),
             NATIVE_METHOD("setGPUDriverPointer", "(J)V", UltralightPlatformJNI::set_gpu_driver_pointer),
             NATIVE_METHOD(
                 "setClipboard",
                 "(Lcom/labymedia/ultralight/plugin/clipboard/UltralightClipboard;)V",
                 UltralightPlatformJNI::set_clipboard),
             NATIVE_METHOD(
                 "setLogger",
                 "(Lcom/labymedia/ultralight/plugin/logging/UltralightLogger;)V",
                 UltralightPlatformJNI::set_logger)};

        runtime.face_winding.constants = JavaEnum<ultralight::FaceWinding>(
            ultralight::kFaceWinding_Clockwise,
            "CLOCKWISE",
            ultralight::kFaceWinding_CounterClockwise,
            "COUNTER_CLOCKWISE");

        runtime.font_hinting.constants = JavaEnum<ultralight::FontHinting>(
            ultralight::kFontHinting_Smooth,
            "SMOOTH",
            ultralight::kFontHinting_Normal,
            "NORMAL",
            ultralight::kFontHinting_Monochrome,
            "MONOCHROME");

        runtime.log_level.constants = JavaEnum<ultralight::LogLevel>(
            ultralight::kLogLevel_Error,
            "ERROR",
            ultralight::kLogLevel_Warning,
            "WARNING",
            ultralight::kLogLevel_Info,
            "INFO");

        runtime.ref_ptr.native_methods = {NATIVE_METHOD("delete", "(J)V", UltralightRefPtrJNI::_delete)};

        runtime.ultralight_renderer.native_methods =
            {NATIVE_METHOD("create", "()Lcom/labymedia/ultralight/UltralightRenderer;", UltralightRendererJNI::create),
             NATIVE_METHOD(
                 "createView",
                 "(JJLcom/labymedia/ultralight/config/UltralightViewConfig;)Lcom/labymedia/ultralight/UltralightView;",
                 UltralightRendererJNI::create_view),
             NATIVE_METHOD("update", "()V", UltralightRendererJNI::update),
             NATIVE_METHOD("render", "()V", UltralightRendererJNI::render),
             NATIVE_METHOD("purgeMemory", "()V", UltralightRendererJNI::purgeMemory),
             NATIVE_METHOD("logMemoryUsage", "()V", UltralightRendererJNI::logMemoryUsage)};

        runtime.ultralight_view.native_methods =
            {NATIVE_METHOD("url", "()Ljava/lang/String;", UltralightViewJNI::url),
             NATIVE_METHOD("title", "()Ljava/lang/String;", UltralightViewJNI::title),
             NATIVE_METHOD("width", "()J", UltralightViewJNI::width),
             NATIVE_METHOD("height", "()J", UltralightViewJNI::height),
             NATIVE_METHOD("isLoading", "()Z", UltralightViewJNI::is_loading),
             NATIVE_METHOD("surface", "()Lcom/labymedia/ultralight/UltralightSurface;", UltralightViewJNI::surface),
             NATIVE_METHOD("loadHTML", "(Ljava/lang/String;Ljava/lang/String;Z)V", UltralightViewJNI::load_html),
             NATIVE_METHOD("loadURL", "(Ljava/lang/String;)V", UltralightViewJNI::load_url),
             NATIVE_METHOD("resize", "(JJ)V", UltralightViewJNI::resize),
             NATIVE_METHOD(
                 "lockJavascriptContext",
                 "()Lcom/labymedia/ultralight/javascript/JavascriptContextLock;",
                 UltralightViewJNI::lock_javascript_context),
             NATIVE_METHOD(
                 "evaluateScript", "(Ljava/lang/String;)Ljava/lang/String;", UltralightViewJNI::evaluate_script),
             NATIVE_METHOD("canGoBack", "()Z", UltralightViewJNI::can_go_back),
             NATIVE_METHOD("canGoForward", "()Z", UltralightViewJNI::can_go_forward),
             NATIVE_METHOD("goBack", "()V", UltralightViewJNI::go_back),
             NATIVE_METHOD("goForward", "()V", UltralightViewJNI::go_forward),
             NATIVE_METHOD("goToHistoryOffset", "(I)V", UltralightViewJNI::go_to_history_offset),
             NATIVE_METHOD("reload", "()V", UltralightViewJNI::reload),
             NATIVE_METHOD("stop", "()V", UltralightViewJNI::stop),
             NATIVE_METHOD("focus", "()V", UltralightViewJNI::focus),
             NATIVE_METHOD("unfocus", "()V", UltralightViewJNI::unfocus),
             NATIVE_METHOD("hasFocus", "()Z", UltralightViewJNI::has_focus),
             NATIVE_METHOD("hasInputFocus", "()Z", UltralightViewJNI::has_input_focus),
             NATIVE_METHOD(
                 "fireKeyEvent",
                 "(Lcom/labymedia/ultralight/input/UltralightKeyEvent;)V",
                 UltralightViewJNI::fire_key_event),
             NATIVE_METHOD(
                 "fireMouseEvent",
                 "(Lcom/labymedia/ultralight/input/UltralightMouseEvent;)V",
                 UltralightViewJNI::fire_mouse_event),
             NATIVE_METHOD(
                 "fireScrollEvent",
                 "(Lcom/labymedia/ultralight/input/UltralightScrollEvent;)V",
                 UltralightViewJNI::fire_scroll_event),
             NATIVE_METHOD(
                 "setViewListener",
                 "(Lcom/labymedia/ultralight/plugin/view/UltralightViewListener;)V",
                 UltralightViewJNI::set_view_listener),
             NATIVE_METHOD(
                 "setLoadListener",
                 "(Lcom/labymedia/ultralight/plugin/loading/UltralightLoadListener;)V",
                 UltralightViewJNI::set_load_listener),
             NATIVE_METHOD("setNeedsPaint", "(Z)V", UltralightViewJNI::set_needs_paint),
             NATIVE_METHOD("needsPaint", "()Z", UltralightViewJNI::needs_paint),
             NATIVE_METHOD("inspector", "()Lcom/labymedia/ultralight/UltralightView;", UltralightViewJNI::inspector),
             NATIVE_METHOD(
                 "renderTarget",
                 "()Lcom/labymedia/ultralight/plugin/render/UltralightRenderTarget;",
                 UltralightViewJNI::render_target)};

        runtime.ultralight_surface.native_methods =
            {NATIVE_METHOD("width", "()J", UltralightSurfaceJNI::width),
             NATIVE_METHOD("height", "()J", UltralightSurfaceJNI::height),
             NATIVE_METHOD("rowBytes", "()J", UltralightSurfaceJNI::rowBytes),
             NATIVE_METHOD("size", "()J", UltralightSurfaceJNI::size),
             NATIVE_METHOD("lockPixels", "()Ljava/nio/ByteBuffer;", UltralightSurfaceJNI::lockPixels),
             NATIVE_METHOD("unlockPixels", "()V", UltralightSurfaceJNI::unlockPixels),
             NATIVE_METHOD("resize", "(JJ)V", UltralightSurfaceJNI::resize),
             NATIVE_METHOD(
                 "setDirtyBounds", "(Lcom/labymedia/ultralight/math/IntRect;)V", UltralightSurfaceJNI::setDirtyBounds),
             NATIVE_METHOD(
                 "dirtyBounds", "()Lcom/labymedia/ultralight/math/IntRect;", UltralightSurfaceJNI::dirtyBounds),
             NATIVE_METHOD("clearDirtyBounds", "()V", UltralightSurfaceJNI::clearDirtyBounds)};

        runtime.ultralight_matrix4x4.native_methods =
            {NATIVE_METHOD("setIdentity", "()V", UltralightMatrix4x4JNI::set_identity),
             NATIVE_METHOD("getData", "()[F", UltralightMatrix4x4JNI::get_data),
             NATIVE_METHOD("construct", "()J", UltralightMatrix4x4JNI::construct),
             NATIVE_METHOD("delete", "(J)V", UltralightMatrix4x4JNI::_delete)};

        runtime.ultralight_matrix.native_methods =
            {NATIVE_METHOD(
                 "getMatrix4x4",
                 "()Lcom/labymedia/ultralight/math/UltralightMatrix4x4;",
                 UltralightMatrixJNI::get_matrix_4x4),
             NATIVE_METHOD("delete", "(J)V", UltralightMatrixJNI::_delete),
             NATIVE_METHOD("construct", "()J", UltralightMatrixJNI::construct),
             NATIVE_METHOD("set", "(DDDDDDDDDDDDDDDD)V", UltralightMatrixJNI::set),
             NATIVE_METHOD("set", "(Lcom/labymedia/ultralight/math/UltralightMatrix4x4;)V", UltralightMatrixJNI::set1),
             NATIVE_METHOD("setOrthographicProjection", "(DDZ)V", UltralightMatrixJNI::set_orthographic_projection),
             NATIVE_METHOD(
                 "transform", "(Lcom/labymedia/ultralight/math/UltralightMatrix;)V", UltralightMatrixJNI::transform)};

        runtime.ultralight_bitmap_surface.native_methods = {NATIVE_METHOD(
            "bitmap", "()Lcom/labymedia/ultralight/bitmap/UltralightBitmap;", UltralightBitmapSurfaceJNI::bitmap)};

        runtime.ultralight_command_type.constants = JavaEnum<ultralight::CommandType>(
            ultralight::kCommandType_ClearRenderBuffer,
            "CLEAR_RENDER_BUFFER",
            ultralight::kCommandType_DrawGeometry,
            "DRAW_GEOMETRY");

        runtime.ultralight_shader_type.constants = JavaEnum<
            ultralight::
                ShaderType>(ultralight::kShaderType_Fill, "FILL", ultralight::kShaderType_FillPath, "FILL_PATH");

        runtime.ultralight_bitmap_format.constants = JavaEnum<ultralight::BitmapFormat>(
            ultralight::kBitmapFormat_A8_UNORM,
            "A8_UNORM",
            ultralight::kBitmapFormat_BGRA8_UNORM_SRGB,
            "BGRA8_UNORM_SRGB");

        runtime.ultralight_bitmap.native_methods =
            {JNINativeMethod{
                 const_cast<char *>("create"),
                 const_cast<char *>("()Lcom/labymedia/ultralight/bitmap/UltralightBitmap;"),
                 reinterpret_cast<void *>(static_cast<jobject (*)(JNIEnv *, jclass)>(&UltralightBitmapJNI::create))},
             JNINativeMethod{
                 const_cast<char *>("create"),
                 const_cast<char *>("("
                                    "J"
                                    "J"
                                    "Lcom/labymedia/ultralight/bitmap/UltralightBitmapFormat;"
                                    ")Lcom/labymedia/ultralight/bitmap/UltralightBitmap;"),
                 reinterpret_cast<void *>(
                     static_cast<jobject (*)(JNIEnv *, jclass, jlong, jlong, jobject)>(&UltralightBitmapJNI::create))},
             JNINativeMethod{
                 const_cast<char *>("create"),
                 const_cast<char *>("("
                                    "J"
                                    "J"
                                    "Lcom/labymedia/ultralight/bitmap/UltralightBitmapFormat;"
                                    "J"
                                    "Ljava/nio/ByteBuffer;"
                                    "Z"
                                    ")Lcom/labymedia/ultralight/bitmap/UltralightBitmap;"),
                 reinterpret_cast<void *>(
                     static_cast<jobject (*)(JNIEnv *, jclass, jlong, jlong, jobject, jlong, jobject, jboolean)>(
                         &UltralightBitmapJNI::create))},
             JNINativeMethod{
                 const_cast<char *>("create"),
                 const_cast<char *>("("
                                    "Lcom/labymedia/ultralight/bitmap/UltralightBitmap;"
                                    ")Lcom/labymedia/ultralight/bitmap/UltralightBitmap;"),
                 reinterpret_cast<void *>(
                     static_cast<jobject (*)(JNIEnv *, jclass, jobject)>(&UltralightBitmapJNI::create))},
             NATIVE_METHOD("width", "()J", UltralightBitmapJNI::width),
             NATIVE_METHOD("height", "()J", UltralightBitmapJNI::height),
             NATIVE_METHOD("bounds", "()Lcom/labymedia/ultralight/math/IntRect;", UltralightBitmapJNI::bounds),
             NATIVE_METHOD(
                 "format", "()Lcom/labymedia/ultralight/bitmap/UltralightBitmapFormat;", UltralightBitmapJNI::format),
             NATIVE_METHOD("bpp", "()J", UltralightBitmapJNI::bpp),
             NATIVE_METHOD("rowBytes", "()J", UltralightBitmapJNI::row_bytes),
             NATIVE_METHOD("size", "()J", UltralightBitmapJNI::size),
             NATIVE_METHOD("ownsPixels", "()Z", UltralightBitmapJNI::owns_pixels),
             NATIVE_METHOD("lockPixels", "()Ljava/nio/ByteBuffer;", UltralightBitmapJNI::lock_pixels),
             NATIVE_METHOD("unlockPixels", "()V", UltralightBitmapJNI::unlock_pixels),
             NATIVE_METHOD("rawPixels", "()Ljava/nio/ByteBuffer;", UltralightBitmapJNI::raw_pixels),
             NATIVE_METHOD("isEmpty", "()Z", UltralightBitmapJNI::is_emtpy),
             NATIVE_METHOD("erase", "()V", UltralightBitmapJNI::erase),
             NATIVE_METHOD("set", "(Lcom/labymedia/ultralight/bitmap/UltralightBitmap;)V", UltralightBitmapJNI::set),
             NATIVE_METHOD(
                 "drawBitmap",
                 "("
                 "Lcom/labymedia/ultralight/math/IntRect;"
                 "Lcom/labymedia/ultralight/math/IntRect;"
                 "Lcom/labymedia/ultralight/bitmap/UltralightBitmap;"
                 "Z"
                 ")Z",
                 UltralightBitmapJNI::width),
             NATIVE_METHOD("writePNG", "(Ljava/lang/String;)Z", UltralightBitmapJNI::write_png),
             NATIVE_METHOD(
                 "resample", "(Lcom/labymedia/ultralight/bitmap/UltralightBitmap;Z)Z", UltralightBitmapJNI::resample),
             NATIVE_METHOD("swapRedBlueChannels", "()V", UltralightBitmapJNI::swap_red_blue_channels)};

        runtime.ultralight_key_event.native_methods = {NATIVE_METHOD(
            "getKeyIdentifierFromVirtualKeyCode",
            "(Lcom/labymedia/ultralight/input/UltralightKey;)Ljava/lang/String;",
            UltralightKeyEventJNI::get_key_identifier_from_virtual_key_code)};

        runtime.ultralight_key_event_type.constants = JavaEnum<ultralight::KeyEvent::Type>(
            ultralight::KeyEvent::kType_KeyDown,
            "DOWN",
            ultralight::KeyEvent::kType_KeyUp,
            "UP",
            ultralight::KeyEvent::kType_RawKeyDown,
            "RAW_DOWN",
            ultralight::KeyEvent::kType_Char,
            "CHAR");

        runtime.ultralight_mouse_event_button.constants = JavaEnum<ultralight::MouseEvent::Button>(
            ultralight::MouseEvent::kButton_Left,
            "LEFT",
            ultralight::MouseEvent::kButton_Middle,
            "MIDDLE",
            ultralight::MouseEvent::kButton_Right,
            "RIGHT");

        runtime.ultralight_mouse_event_type.constants = JavaEnum<ultralight::MouseEvent::Type>(
            ultralight::MouseEvent::kType_MouseMoved,
            "MOVED",
            ultralight::MouseEvent::kType_MouseDown,
            "DOWN",
            ultralight::MouseEvent::kType_MouseUp,
            "UP");

        runtime.ultralight_scroll_event_type.constants = JavaEnum<ultralight::ScrollEvent::Type>(
            ultralight::ScrollEvent::kType_ScrollByPixel,
            "BY_PIXEL",
            ultralight::ScrollEvent::kType_ScrollByPage,
            "BY_PAGE");

        runtime.ultralight_cursor.constants = JavaEnum<ultralight::Cursor>(
            ultralight::kCursor_Pointer,
            "POINTER",
            ultralight::kCursor_Cross,
            "CROSS",
            ultralight::kCursor_Hand,
            "HAND",
            ultralight::kCursor_IBeam,
            "I_BEAM",
            ultralight::kCursor_Wait,
            "WAIT",
            ultralight::kCursor_Help,
            "HELP",
            ultralight::kCursor_EastResize,
            "EAST_RESIZE",
            ultralight::kCursor_NorthResize,
            "NORTH_RESIZE",
            ultralight::kCursor_NorthEastResize,
            "NORTH_EAST_RESIZE",
            ultralight::kCursor_NorthWestResize,
            "NORTH_WEST_RESIZE",
            ultralight::kCursor_SouthResize,
            "SOUTH_RESIZE",
            ultralight::kCursor_SouthEastResize,
            "SOUTH_EAST_RESIZE",
            ultralight::kCursor_SouthWestResize,
            "SOUTH_WEST_RESIZE",
            ultralight::kCursor_WestResize,
            "WEST_RESIZE",
            ultralight::kCursor_NorthSouthResize,
            "NORTH_SOUTH_RESIZE",
            ultralight::kCursor_EastWestResize,
            "EAST_WEST_RESIZE",
            ultralight::kCursor_NorthEastSouthWestResize,
            "NORTH_EAST_SOUTH_WEST_RESIZE",
            ultralight::kCursor_NorthWestSouthEastResize,
            "NORTH_WEST_SOUTH_EAST_RESIZE",
            ultralight::kCursor_ColumnResize,
            "COLUMN_RESIZE",
            ultralight::kCursor_RowResize,
            "ROW_RESIZE",
            ultralight::kCursor_MiddlePanning,
            "MIDDLE_PANNING",
            ultralight::kCursor_EastPanning,
            "EAST_PANNING",
            ultralight::kCursor_NorthPanning,
            "NORTH_PANNING",
            ultralight::kCursor_NorthEastPanning,
            "NORTH_EAST_PANNING",
            ultralight::kCursor_NorthWestPanning,
            "NORTH_WEST_PANNING",
            ultralight::kCursor_SouthPanning,
            "SOUTH_PANNING",
            ultralight::kCursor_SouthEastPanning,
            "SOUTH_EAST_PANNING",
            ultralight::kCursor_SouthWestPanning,
            "SOUTH_WEST_PANNING",
            ultralight::kCursor_WestPanning,
            "WEST_PANNING",
            ultralight::kCursor_Move,
            "MOVE",
            ultralight::kCursor_VerticalText,
            "VERTICAL_TEXT",
            ultralight::kCursor_Cell,
            "CELL",
            ultralight::kCursor_ContextMenu,
            "CONTEXT_MENU",
            ultralight::kCursor_Alias,
            "ALIAS",
            ultralight::kCursor_Progress,
            "PROGRESS",
            ultralight::kCursor_NoDrop,
            "NO_DROP",
            ultralight::kCursor_Copy,
            "COPY",
            ultralight::kCursor_None,
            "NONE",
            ultralight::kCursor_NotAllowed,
            "NOT_ALLOWED",
            ultralight::kCursor_ZoomIn,
            "ZOOM_IN",
            ultralight::kCursor_ZoomOut,
            "ZOOM_OUT",
            ultralight::kCursor_Grab,
            "GRAB",
            ultralight::kCursor_Grabbing,
            "GRABBING",
            ultralight::kCursor_Custom,
            "CUSTOM");

        runtime.message_level.constants = JavaEnum<ultralight::MessageLevel>(
            ultralight::kMessageLevel_Log,
            "LOG",
            ultralight::kMessageLevel_Warning,
            "WARNING",
            ultralight::kMessageLevel_Error,
            "ERROR",
            ultralight::kMessageLevel_Debug,
            "DEBUG",
            ultralight::kMessageLevel_Info,
            "INFO");

        runtime.message_source.constants = JavaEnum<ultralight::MessageSource>(
            ultralight::kMessageSource_XML,
            "XML",
            ultralight::kMessageSource_JS,
            "JS",
            ultralight::kMessageSource_Network,
            "NETWORK",
            ultralight::kMessageSource_ConsoleAPI,
            "CONSOLE_API",
            ultralight::kMessageSource_Storage,
            "STORAGE",
            ultralight::kMessageSource_AppCache,
            "APP_CACHE",
            ultralight::kMessageSource_Rendering,
            "RENDERING",
            ultralight::kMessageSource_CSS,
            "CSS",
            ultralight::kMessageSource_Security,
            "SECURITY",
            ultralight::kMessageSource_ContentBlocker,
            "CONTENT_BLOCKER",
            ultralight::kMessageSource_Other,
            "OTHER");

        runtime.ultralight_vertexbuffer_format.constants = JavaEnum<ultralight::VertexBufferFormat>(
            ultralight::kVertexBufferFormat_2f_4ub_2f,
            "FORMAT_2F_4UB_2F",
            ultralight::kVertexBufferFormat_2f_4ub_2f_2f_28f,
            "FORMAT_2F_4UB_2F_2F_28F");

        runtime.javascript_context.native_methods =
            {NATIVE_METHOD(
                 "getGlobalContext",
                 "()Lcom/labymedia/ultralight/javascript/JavascriptGlobalContext;",
                 JavascriptContextJNI::get_global_context),
             NATIVE_METHOD(
                 "getGlobalObject",
                 "()Lcom/labymedia/ultralight/javascript/JavascriptObject;",
                 JavascriptContextJNI::get_global_object),
             NATIVE_METHOD(
                 "makeUndefined",
                 "()Lcom/labymedia/ultralight/javascript/JavascriptValue;",
                 JavascriptContextJNI::make_undefined),
             NATIVE_METHOD(
                 "makeNull",
                 "()Lcom/labymedia/ultralight/javascript/JavascriptValue;",
                 JavascriptContextJNI::make_null),
             NATIVE_METHOD(
                 "makeBoolean",
                 "(Z)Lcom/labymedia/ultralight/javascript/JavascriptValue;",
                 JavascriptContextJNI::make_boolean),
             NATIVE_METHOD(
                 "makeNumber",
                 "(D)Lcom/labymedia/ultralight/javascript/JavascriptValue;",
                 JavascriptContextJNI::make_number),
             NATIVE_METHOD(
                 "makeString",
                 "(Ljava/lang/String;)Lcom/labymedia/ultralight/javascript/JavascriptValue;",
                 JavascriptContextJNI::make_string),
             NATIVE_METHOD(
                 "makeSymbol",
                 "(Ljava/lang/String;)Lcom/labymedia/ultralight/javascript/JavascriptValue;",
                 JavascriptContextJNI::make_symbol),
             NATIVE_METHOD(
                 "makeFromJSON",
                 "(Ljava/lang/String;)Lcom/labymedia/ultralight/javascript/JavascriptValue;",
                 JavascriptContextJNI::make_from_json),
             NATIVE_METHOD(
                 "makeObject",
                 "("
                 "Lcom/labymedia/ultralight/javascript/JavascriptClass;"
                 "Ljava/lang/Object;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptObject;",
                 JavascriptContextJNI::make_object),
             NATIVE_METHOD(
                 "makeArray",
                 "("
                 "[Lcom/labymedia/ultralight/javascript/JavascriptValue;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptObject;",
                 JavascriptContextJNI::make_array),
             NATIVE_METHOD(
                 "makeDate",
                 "("
                 "[Lcom/labymedia/ultralight/javascript/JavascriptValue;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptObject;",
                 JavascriptContextJNI::make_date),
             NATIVE_METHOD(
                 "makeError",
                 "("
                 "[Lcom/labymedia/ultralight/javascript/JavascriptValue;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptObject;",
                 JavascriptContextJNI::make_error),
             NATIVE_METHOD(
                 "makeRegExp",
                 "("
                 "[Lcom/labymedia/ultralight/javascript/JavascriptValue;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptObject;",
                 JavascriptContextJNI::make_reg_exp),
             NATIVE_METHOD(
                 "makeFunction",
                 "("
                 "Ljava/lang/String;"
                 "[Ljava/lang/String;"
                 "Ljava/lang/String;"
                 "Ljava/lang/String;"
                 "I"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptObject;",
                 JavascriptContextJNI::make_function),
             NATIVE_METHOD("garbageCollect", "()V", JavascriptContextJNI::garbage_collect)};

        runtime.javascript_context_lock.native_methods =
            {NATIVE_METHOD(
                 "getContext",
                 "()Lcom/labymedia/ultralight/javascript/JavascriptContext;",
                 JavascriptContextLockJNI::get_context),
             NATIVE_METHOD("release", "()V", JavascriptContextLockJNI::release)};

        runtime.javascript_global_context.native_methods =
            {NATIVE_METHOD("contextUnlocking", "()V", JavascriptGlobalContextJNI::context_unlocking),
             NATIVE_METHOD("getName", "()Ljava/lang/String;", JavascriptGlobalContextJNI::get_name),
             NATIVE_METHOD("setName", "(Ljava/lang/String;)V", JavascriptGlobalContextJNI::set_name)};

        runtime.javascript_value.native_methods =
            {NATIVE_METHOD(
                 "protect",
                 "()Lcom/labymedia/ultralight/javascript/JavascriptProtectedValue;",
                 JavascriptValueJNI::protect),
             NATIVE_METHOD("contextUnlocking", "()V", JavascriptValueJNI::context_unlocking),
             NATIVE_METHOD(
                 "getType", "()Lcom/labymedia/ultralight/javascript/JavascriptType;", JavascriptValueJNI::get_type),
             NATIVE_METHOD("isUndefined", "()Z", JavascriptValueJNI::is_undefined),
             NATIVE_METHOD("isNull", "()Z", JavascriptValueJNI::is_null),
             NATIVE_METHOD("isBoolean", "()Z", JavascriptValueJNI::is_boolean),
             NATIVE_METHOD("isNumber", "()Z", JavascriptValueJNI::is_number),
             NATIVE_METHOD("isString", "()Z", JavascriptValueJNI::is_string),
             NATIVE_METHOD(
                 "isOfClass",
                 "(Lcom/labymedia/ultralight/javascript/JavascriptClass;)Z",
                 JavascriptValueJNI::is_of_class),
             NATIVE_METHOD("isObject", "()Z", JavascriptValueJNI::is_object),
             NATIVE_METHOD("isArray", "()Z", JavascriptValueJNI::is_array),
             NATIVE_METHOD("isDate", "()Z", JavascriptValueJNI::is_date),
             NATIVE_METHOD(
                 "getTypedArrayType",
                 "()Lcom/labymedia/ultralight/javascript/JavascriptTypedArrayType;",
                 JavascriptValueJNI::get_typed_array_type),
             NATIVE_METHOD(
                 "isEqual", "(Lcom/labymedia/ultralight/javascript/JavascriptValue;)Z", JavascriptValueJNI::is_equal),
             NATIVE_METHOD(
                 "isStrictEqual",
                 "(Lcom/labymedia/ultralight/javascript/JavascriptValue;)Z",
                 JavascriptValueJNI::is_strict_equal),
             NATIVE_METHOD(
                 "isInstanceOfConstructor",
                 "(Lcom/labymedia/ultralight/javascript/JavascriptObject;)Z",
                 JavascriptValueJNI::is_instance_of_constructor),
             NATIVE_METHOD("toJson", "(S)Ljava/lang/String;", JavascriptValueJNI::to_json),
             NATIVE_METHOD("toBoolean", "()Z", JavascriptValueJNI::to_boolean),
             NATIVE_METHOD("toNumber", "()D", JavascriptValueJNI::to_number),
             NATIVE_METHOD("toStringCopy", "()Ljava/lang/String;", JavascriptValueJNI::to_string_copy),
             NATIVE_METHOD(
                 "toObject",
                 "()Lcom/labymedia/ultralight/javascript/JavascriptObject;",
                 JavascriptValueJNI::to_object)};

        runtime.javascript_object.native_methods =
            {NATIVE_METHOD(
                 "getPrototype",
                 "()Lcom/labymedia/ultralight/javascript/JavascriptValue;",
                 JavascriptObjectJNI::get_prototype),
             NATIVE_METHOD(
                 "setPrototype",
                 "(Lcom/labymedia/ultralight/javascript/JavascriptValue;)V",
                 JavascriptObjectJNI::set_prototype),
             NATIVE_METHOD("hasProperty", "(Ljava/lang/String;)Z", JavascriptObjectJNI::has_property),
             NATIVE_METHOD(
                 "getProperty",
                 "(Ljava/lang/String;)Lcom/labymedia/ultralight/javascript/JavascriptValue;",
                 JavascriptObjectJNI::get_property),
             NATIVE_METHOD(
                 "setProperty",
                 "(Ljava/lang/String;Lcom/labymedia/ultralight/javascript/JavascriptValue;I)V",
                 JavascriptObjectJNI::set_property),
             NATIVE_METHOD("deleteProperty", "(Ljava/lang/String;)Z", JavascriptObjectJNI::delete_property),
             NATIVE_METHOD(
                 "hasPropertyForKey",
                 "(Lcom/labymedia/ultralight/javascript/JavascriptValue;)Z",
                 JavascriptObjectJNI::has_property_for_key),
             NATIVE_METHOD(
                 "getPropertyForKey",
                 "("
                 "Lcom/labymedia/ultralight/javascript/JavascriptValue;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptValue;",
                 JavascriptObjectJNI::get_property_for_key),
             NATIVE_METHOD(
                 "setPropertyForKey",
                 "("
                 "Lcom/labymedia/ultralight/javascript/JavascriptValue;"
                 "Lcom/labymedia/ultralight/javascript/JavascriptValue;"
                 "I"
                 ")V",
                 JavascriptObjectJNI::set_property_for_key),
             NATIVE_METHOD(
                 "deletePropertyForKey",
                 "(Lcom/labymedia/ultralight/javascript/JavascriptValue;)Z",
                 JavascriptObjectJNI::delete_property_for_key),
             NATIVE_METHOD(
                 "getPropertyAtIndex",
                 "(J)Lcom/labymedia/ultralight/javascript/JavascriptValue;",
                 JavascriptObjectJNI::get_property_at_index),
             NATIVE_METHOD(
                 "setPropertyAtIndex",
                 "(JLcom/labymedia/ultralight/javascript/JavascriptValue;)V",
                 JavascriptObjectJNI::set_property_at_index),
             NATIVE_METHOD("getPrivate", "()Ljava/lang/Object;", JavascriptObjectJNI::get_private),
             NATIVE_METHOD("setPrivate", "(Ljava/lang/Object;)V", JavascriptObjectJNI::set_private),
             NATIVE_METHOD("isFunction", "()Z", JavascriptObjectJNI::is_function),
             NATIVE_METHOD(
                 "callAsFunction",
                 "("
                 "Lcom/labymedia/ultralight/javascript/JavascriptObject;"
                 "[Lcom/labymedia/ultralight/javascript/JavascriptValue;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptValue;",
                 JavascriptObjectJNI::call_as_function),
             NATIVE_METHOD("isConstructor", "()Z", JavascriptObjectJNI::is_constructor),
             NATIVE_METHOD(
                 "callAsConstructor",
                 "("
                 "[Lcom/labymedia/ultralight/javascript/JavascriptValue;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptObject;",
                 JavascriptObjectJNI::call_as_constructor),
             NATIVE_METHOD("copyPropertyNames", "()[Ljava/lang/String;", JavascriptObjectJNI::copy_property_names)};

        runtime.javascript_type.constants = JavaEnum<JSType>(
            kJSTypeUndefined,
            "UNDEFINED",
            kJSTypeNull,
            "NULL",
            kJSTypeBoolean,
            "BOOLEAN",
            kJSTypeNumber,
            "NUMBER",
            kJSTypeString,
            "STRING",
            kJSTypeObject,
            "OBJECT",
            kJSTypeSymbol,
            "SYMBOL");

        runtime.javascript_typed_array_type.constants = JavaEnum<JSTypedArrayType>(
            kJSTypedArrayTypeInt8Array,
            "INT8",
            kJSTypedArrayTypeInt16Array,
            "INT16",
            kJSTypedArrayTypeInt32Array,
            "INT32",
            kJSTypedArrayTypeUint8Array,
            "UINT8",
            kJSTypedArrayTypeUint8ClampedArray,
            "UINT8_CLAMPED",
            kJSTypedArrayTypeUint16Array,
            "UINT16",
            kJSTypedArrayTypeUint32Array,
            "UINT32",
            kJSTypedArrayTypeFloat32Array,
            "FLOAT32",
            kJSTypedArrayTypeFloat64Array,
            "FLOAT64",
            kJSTypedArrayTypeArrayBuffer,
            "BUFFER",
            kJSTypedArrayTypeNone,
            "NONE");

        runtime.javascript_class.native_methods = {NATIVE_METHOD("release", "(J)V", JavascriptClassJNI::release)};

        runtime.javascript_class_definition.native_methods =
            {NATIVE_METHOD(
                 "name",
                 "(Ljava/lang/String;)Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::name),
             NATIVE_METHOD(
                 "attributes",
                 "(I)Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::attributes),
             NATIVE_METHOD(
                 "parentClass",
                 "("
                 "Lcom/labymedia/ultralight/javascript/JavascriptClass;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::parent_class),
             NATIVE_METHOD(
                 "staticValue",
                 "("
                 "Ljava/lang/String;"
                 "Lcom/labymedia/ultralight/javascript/interop/JavascriptObjectPropertyGetter;"
                 "Lcom/labymedia/ultralight/javascript/interop/JavascriptObjectPropertySetter;"
                 "I"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::static_value),
             NATIVE_METHOD(
                 "staticFunction",
                 "("
                 "Ljava/lang/String;"
                 "Lcom/labymedia/ultralight/javascript/interop/JavascriptObjectFunction;"
                 "I"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::static_function),
             NATIVE_METHOD(
                 "onInitialize",
                 "("
                 "Lcom/labymedia/ultralight/javascript/interop/JavascriptObjectInitializer;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_initialize),
             NATIVE_METHOD(
                 "onFinalize",
                 "("
                 "Lcom/labymedia/ultralight/javascript/interop/JavascriptObjectFinalizer;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_finalize),
             NATIVE_METHOD(
                 "onHasProperty",
                 "("
                 "Lcom/labymedia/ultralight/javascript/interop/JavascriptObjectHasPropertyTester;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_has_property),
             NATIVE_METHOD(
                 "onGetProperty",
                 "("
                 "Lcom/labymedia/ultralight/javascript/interop/JavascriptObjectPropertyGetter;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_get_property),
             NATIVE_METHOD(
                 "onSetProperty",
                 "("
                 "Lcom/labymedia/ultralight/javascript/interop/JavascriptObjectPropertySetter;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_set_property),
             NATIVE_METHOD(
                 "onDeleteProperty",
                 "("
                 "Lcom/labymedia/ultralight/javascript/interop/JavascriptObjectPropertyDeleter;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_delete_property),
             NATIVE_METHOD(
                 "onGetPropertyNames",
                 "("
                 "Lcom/labymedia/ultralight/javascript/interop/JavascriptObjectPropertyNamesCollector;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_get_property_names),
             NATIVE_METHOD(
                 "onCallAsFunction",
                 "("
                 "Lcom/labymedia/ultralight/javascript/interop/JavascriptObjectFunction;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_call_as_function),
             NATIVE_METHOD(
                 "onCallAsConstructor",
                 "("
                 "Lcom/labymedia/ultralight/javascript/interop/JavascriptObjectConstructor;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_call_as_constructor),
             NATIVE_METHOD(
                 "onHasInstance",
                 "("
                 "Lcom/labymedia/ultralight/javascript/interop/JavascriptObjectHasInstanceTester;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_has_instance),
             NATIVE_METHOD(
                 "onConvertToType",
                 "("
                 "Lcom/labymedia/ultralight/javascript/interop/JavascriptObjectToTypeConverter;"
                 ")Lcom/labymedia/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_convert_to_type),
             NATIVE_METHOD(
                 "bake", "()Lcom/labymedia/ultralight/javascript/JavascriptClass;", JavascriptClassDefinitionJNI::bake),
             NATIVE_METHOD("createEmpty", "()J", JavascriptClassDefinitionJNI::create_empty),
             NATIVE_METHOD("free", "(J)V", JavascriptClassDefinitionJNI::free)};

        runtime.bridged_logger = nullptr;
        runtime.bridged_file_system = nullptr;
        runtime.bridged_gpu_driver = nullptr;
        runtime.bridged_clipboard = nullptr;
    }
} // namespace ultralight_java
