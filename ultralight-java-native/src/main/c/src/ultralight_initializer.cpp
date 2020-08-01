#include "ultralight_java/ultralight_initializer.hpp"

#include "ultralight_java/java_bridges/javascript_class_definition_jni.hpp"
#include "ultralight_java/java_bridges/javascript_class_jni.hpp"
#include "ultralight_java/java_bridges/javascript_context_jni.hpp"
#include "ultralight_java/java_bridges/javascript_context_lock_jni.hpp"
#include "ultralight_java/java_bridges/javascript_global_context_jni.hpp"
#include "ultralight_java/java_bridges/javascript_value_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_bitmap_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_bitmap_surface_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_key_event_jni.hpp"
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
        runtime.ultralight_platform.native_methods = {
            NATIVE_METHOD("instance", "()Lnet/janrupf/ultralight/UltralightPlatform;", UltralightPlatformJNI::instance),
            NATIVE_METHOD(
                "setConfig", "(Lnet/janrupf/ultralight/config/UltralightConfig;)V", UltralightPlatformJNI::set_config),
            NATIVE_METHOD("usePlatformFontLoader", "()V", UltralightPlatformJNI::use_platform_font_loader),
            NATIVE_METHOD(
                "usePlatformFileSystem", "(Ljava/lang/String;)V", UltralightPlatformJNI::use_platform_file_system),
            NATIVE_METHOD(
                "setFileSystem",
                "(Lnet/janrupf/ultralight/plugin/filesystem/UltralightFileSystem;)V",
                UltralightPlatformJNI::set_file_system),
            NATIVE_METHOD(
                "setClipboard",
                "(Lnet/janrupf/ultralight/plugin/clipboard/UltralightClipboard;)V",
                UltralightPlatformJNI::set_clipboard),
            NATIVE_METHOD(
                "setLogger",
                "(Lnet/janrupf/ultralight/plugin/logging/UltralightLogger;)V",
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
            {NATIVE_METHOD("create", "()Lnet/janrupf/ultralight/UltralightRenderer;", UltralightRendererJNI::create),
             NATIVE_METHOD(
                 "createView", "(JJZ)Lnet/janrupf/ultralight/UltralightView;", UltralightRendererJNI::createView),
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
             NATIVE_METHOD("surface", "()Lnet/janrupf/ultralight/UltralightSurface;", UltralightViewJNI::surface),
             NATIVE_METHOD("loadHTML", "(Ljava/lang/String;Ljava/lang/String;Z)V", UltralightViewJNI::load_html),
             NATIVE_METHOD("loadURL", "(Ljava/lang/String;)V", UltralightViewJNI::load_url),
             NATIVE_METHOD("resize", "(JJ)V", UltralightViewJNI::resize),
             NATIVE_METHOD(
                 "lockJavascriptContext",
                 "()Lnet/janrupf/ultralight/javascript/JavascriptContextLock;",
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
                 "(Lnet/janrupf/ultralight/input/UltralightKeyEvent;)V",
                 UltralightViewJNI::fire_key_event),
             NATIVE_METHOD(
                 "fireMouseEvent",
                 "(Lnet/janrupf/ultralight/input/UltralightMouseEvent;)V",
                 UltralightViewJNI::fire_mouse_event),
             NATIVE_METHOD(
                 "fireScrollEvent",
                 "(Lnet/janrupf/ultralight/input/UltralightScrollEvent;)V",
                 UltralightViewJNI::fire_scroll_event),
             NATIVE_METHOD(
                 "setViewListener",
                 "(Lnet/janrupf/ultralight/plugin/view/UltralightViewListener;)V",
                 UltralightViewJNI::set_view_listener),
             NATIVE_METHOD(
                 "setLoadListener",
                 "(Lnet/janrupf/ultralight/plugin/loading/UltralightLoadListener;)V",
                 UltralightViewJNI::set_load_listener),
             NATIVE_METHOD("setNeedsPaint", "(Z)V", UltralightViewJNI::set_needs_paint),
             NATIVE_METHOD("needsPaint", "()Z", UltralightViewJNI::needs_paint),
             NATIVE_METHOD("inspector", "()Lnet/janrupf/ultralight/UltralightView;", UltralightViewJNI::inspector)};

        runtime.ultralight_surface.native_methods =
            {NATIVE_METHOD("width", "()J", UltralightSurfaceJNI::width),
             NATIVE_METHOD("height", "()J", UltralightSurfaceJNI::height),
             NATIVE_METHOD("rowBytes", "()J", UltralightSurfaceJNI::rowBytes),
             NATIVE_METHOD("size", "()J", UltralightSurfaceJNI::size),
             NATIVE_METHOD("lockPixels", "()Ljava/nio/ByteBuffer;", UltralightSurfaceJNI::lockPixels),
             NATIVE_METHOD("unlockPixels", "()V", UltralightSurfaceJNI::unlockPixels),
             NATIVE_METHOD("resize", "(JJ)V", UltralightSurfaceJNI::resize),
             NATIVE_METHOD(
                 "setDirtyBounds", "(Lnet/janrupf/ultralight/math/IntRect;)V", UltralightSurfaceJNI::setDirtyBounds),
             NATIVE_METHOD("dirtyBounds", "()Lnet/janrupf/ultralight/math/IntRect;", UltralightSurfaceJNI::dirtyBounds),
             NATIVE_METHOD("clearDirtyBounds", "()V", UltralightSurfaceJNI::clearDirtyBounds)};

        runtime.ultralight_bitmap_surface.native_methods = {NATIVE_METHOD(
            "bitmap", "()Lnet/janrupf/ultralight/bitmap/UltralightBitmap;", UltralightBitmapSurfaceJNI::bitmap)};

        runtime.ultralight_bitmap_format.constants = JavaEnum<ultralight::BitmapFormat>(
            ultralight::kBitmapFormat_A8_UNORM,
            "A8_UNORM",
            ultralight::kBitmapFormat_BGRA8_UNORM_SRGB,
            "BGRA8_UNORM_SRGB");

        runtime.ultralight_bitmap.native_methods =
            {JNINativeMethod{
                 const_cast<char *>("create"),
                 const_cast<char *>("()Lnet/janrupf/ultralight/bitmap/UltralightBitmap;"),
                 reinterpret_cast<void *>(static_cast<jobject (*)(JNIEnv *, jclass)>(&UltralightBitmapJNI::create))},
             JNINativeMethod{
                 const_cast<char *>("create"),
                 const_cast<char *>("("
                                    "J"
                                    "J"
                                    "Lnet/janrupf/ultralight/bitmap/UltralightBitmapFormat;"
                                    ")Lnet/janrupf/ultralight/bitmap/UltralightBitmap;"),
                 reinterpret_cast<void *>(
                     static_cast<jobject (*)(JNIEnv *, jclass, jlong, jlong, jobject)>(&UltralightBitmapJNI::create))},
             JNINativeMethod{
                 const_cast<char *>("create"),
                 const_cast<char *>("("
                                    "J"
                                    "J"
                                    "Lnet/janrupf/ultralight/bitmap/UltralightBitmapFormat;"
                                    "J"
                                    "Ljava/nio/ByteBuffer;"
                                    "Z"
                                    ")Lnet/janrupf/ultralight/bitmap/UltralightBitmap;"),
                 reinterpret_cast<void *>(
                     static_cast<jobject (*)(JNIEnv *, jclass, jlong, jlong, jobject, jlong, jobject, jboolean)>(
                         &UltralightBitmapJNI::create))},
             JNINativeMethod{
                 const_cast<char *>("create"),
                 const_cast<char *>("("
                                    "Lnet/janrupf/ultralight/bitmap/UltralightBitmap;"
                                    ")Lnet/janrupf/ultralight/bitmap/UltralightBitmap;"),
                 reinterpret_cast<void *>(
                     static_cast<jobject (*)(JNIEnv *, jclass, jobject)>(&UltralightBitmapJNI::create))},
             NATIVE_METHOD("width", "()J", UltralightBitmapJNI::width),
             NATIVE_METHOD("height", "()J", UltralightBitmapJNI::height),
             NATIVE_METHOD("bounds", "()Lnet/janrupf/ultralight/math/IntRect;", UltralightBitmapJNI::bounds),
             NATIVE_METHOD(
                 "format", "()Lnet/janrupf/ultralight/bitmap/UltralightBitmapFormat;", UltralightBitmapJNI::format),
             NATIVE_METHOD("bpp", "()J", UltralightBitmapJNI::bpp),
             NATIVE_METHOD("rowBytes", "()J", UltralightBitmapJNI::row_bytes),
             NATIVE_METHOD("size", "()J", UltralightBitmapJNI::size),
             NATIVE_METHOD("ownsPixels", "()Z", UltralightBitmapJNI::owns_pixels),
             NATIVE_METHOD("lockPixels", "()Ljava/nio/ByteBuffer;", UltralightBitmapJNI::lock_pixels),
             NATIVE_METHOD("unlockPixels", "()V", UltralightBitmapJNI::unlock_pixels),
             NATIVE_METHOD("rawPixels", "()Ljava/nio/ByteBuffer;", UltralightBitmapJNI::raw_pixels),
             NATIVE_METHOD("isEmpty", "()Z", UltralightBitmapJNI::is_emtpy),
             NATIVE_METHOD("erase", "()V", UltralightBitmapJNI::erase),
             NATIVE_METHOD("set", "(Lnet/janrupf/ultralight/bitmap/UltralightBitmap;)V", UltralightBitmapJNI::set),
             NATIVE_METHOD(
                 "drawBitmap",
                 "("
                 "Lnet/janrupf/ultralight/math/IntRect;"
                 "Lnet/janrupf/ultralight/math/IntRect;"
                 "Lnet/janrupf/ultralight/bitmap/UltralightBitmap;"
                 "Z"
                 ")Z",
                 UltralightBitmapJNI::width),
             NATIVE_METHOD("writePNG", "(Ljava/lang/String;)Z", UltralightBitmapJNI::write_png),
             NATIVE_METHOD(
                 "resample", "(Lnet/janrupf/ultralight/bitmap/UltralightBitmap;Z)Z", UltralightBitmapJNI::resample),
             NATIVE_METHOD("swapRedBlueChannels", "()V", UltralightBitmapJNI::swap_red_blue_channels)};

        runtime.ultralight_key_event.native_methods = {NATIVE_METHOD(
            "getKeyIdentifierFromVirtualKeyCode",
            "(Lnet/janrupf/ultralight/input/UltralightKey;)Ljava/lang/String;",
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

        runtime.javascript_context.native_methods = {NATIVE_METHOD(
            "getGlobalContext",
            "()Lnet/janrupf/ultralight/javascript/JavascriptGlobalContext;",
            JavascriptContextJNI::get_global_context)};

        runtime.javascript_context_lock.native_methods =
            {NATIVE_METHOD(
                 "getContext",
                 "()Lnet/janrupf/ultralight/javascript/JavascriptContext;",
                 JavascriptContextLockJNI::get_context),
             NATIVE_METHOD("release", "()V", JavascriptContextLockJNI::release)};

        runtime.javascript_global_context.native_methods =
            {NATIVE_METHOD("contextUnlocking", "()V", JavascriptGlobalContextJNI::context_unlocking),
             NATIVE_METHOD("getName", "()Ljava/lang/String;", JavascriptGlobalContextJNI::get_name),
             NATIVE_METHOD("setName", "(Ljava/lang/String;)V", JavascriptGlobalContextJNI::set_name)};

        runtime.javascript_value.native_methods =
            {NATIVE_METHOD(
                 "protect",
                 "()Lnet/janrupf/ultralight/javascript/JavascriptProtectedValue;",
                 JavascriptValueJNI::protect),
             NATIVE_METHOD("contextUnlocking", "()V", JavascriptValueJNI::context_unlocking),
             NATIVE_METHOD(
                 "getType", "()Lnet/janrupf/ultralight/javascript/JavascriptType;", JavascriptValueJNI::get_type),
             NATIVE_METHOD("isUndefined", "()Z", JavascriptValueJNI::is_undefined),
             NATIVE_METHOD("isNull", "()Z", JavascriptValueJNI::is_null),
             NATIVE_METHOD("isBoolean", "()Z", JavascriptValueJNI::is_boolean),
             NATIVE_METHOD("isNumber", "()Z", JavascriptValueJNI::is_number),
             NATIVE_METHOD("isString", "()Z", JavascriptValueJNI::is_string),
             NATIVE_METHOD("isObject", "()Z", JavascriptValueJNI::is_object),
             NATIVE_METHOD("isArray", "()Z", JavascriptValueJNI::is_array),
             NATIVE_METHOD("isDate", "()Z", JavascriptValueJNI::is_date),
             NATIVE_METHOD(
                 "getTypedArrayType",
                 "()Lnet/janrupf/ultralight/javascript/JavascriptTypedArrayType;",
                 JavascriptValueJNI::get_typed_array_type),
             NATIVE_METHOD(
                 "isEqual", "(Lnet/janrupf/ultralight/javascript/JavascriptValue;)Z", JavascriptValueJNI::is_equal),
             NATIVE_METHOD(
                 "isStrictEqual",
                 "(Lnet/janrupf/ultralight/javascript/JavascriptValue;)Z",
                 JavascriptValueJNI::is_strict_equal),
             NATIVE_METHOD("toJson", "(S)Ljava/lang/String;", JavascriptValueJNI::to_json),
             NATIVE_METHOD("toBoolean", "()Z", JavascriptValueJNI::to_boolean),
             NATIVE_METHOD("toNumber", "()D", JavascriptValueJNI::to_number),
             NATIVE_METHOD("toStringCopy", "()Ljava/lang/String;", JavascriptValueJNI::to_string_copy)};

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
                 "(Ljava/lang/String;)Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::name),
             NATIVE_METHOD(
                 "attributes",
                 "(I)Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::attributes),
             NATIVE_METHOD(
                 "parentClass",
                 "("
                 "Lnet/janrupf/ultralight/javascript/JavascriptClass;"
                 ")Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::parent_class),
             NATIVE_METHOD(
                 "staticValue",
                 "("
                 "Ljava/lang/String;"
                 "Lnet/janrupf/ultralight/javascript/interop/JavascriptObjectPropertyGetter;"
                 "Lnet/janrupf/ultralight/javascript/interop/JavascriptObjectPropertySetter;"
                 "I"
                 ")Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::static_value),
             NATIVE_METHOD(
                 "staticFunction",
                 "("
                 "Ljava/lang/String;"
                 "Lnet/janrupf/ultralight/javascript/interop/JavascriptObjectFunction;"
                 "I"
                 ")Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::static_function),
             NATIVE_METHOD(
                 "onInitialize",
                 "("
                 "Lnet/janrupf/ultralight/javascript/interop/JavascriptObjectInitializer;"
                 ")Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_initialize),
             NATIVE_METHOD(
                 "onFinalize",
                 "("
                 "Lnet/janrupf/ultralight/javascript/interop/JavascriptObjectFinalizer;"
                 ")Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_finalize),
             NATIVE_METHOD(
                 "onGetProperty",
                 "("
                 "Lnet/janrupf/ultralight/javascript/interop/JavascriptObjectPropertyGetter;"
                 ")Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_get_property),
             NATIVE_METHOD(
                 "onSetProperty",
                 "("
                 "Lnet/janrupf/ultralight/javascript/interop/JavascriptObjectPropertySetter;"
                 ")Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_set_property),
             NATIVE_METHOD(
                 "onDeleteProperty",
                 "("
                 "Lnet/janrupf/ultralight/javascript/interop/JavascriptObjectPropertyDeleter;"
                 ")Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_delete_property),
             NATIVE_METHOD(
                 "onGetPropertyNames",
                 "("
                 "Lnet/janrupf/ultralight/javascript/interop/JavascriptObjectPropertyNamesCollector;"
                 ")Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_get_property_names),
             NATIVE_METHOD(
                 "onCallAsFunction",
                 "("
                 "Lnet/janrupf/ultralight/javascript/interop/JavascriptObjectFunction;"
                 ")Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_call_as_function),
             NATIVE_METHOD(
                 "onCallAsConstructor",
                 "("
                 "Lnet/janrupf/ultralight/javascript/interop/JavascriptObjectConstructor;"
                 ")Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_call_as_constructor),
             NATIVE_METHOD(
                 "onHasInstance",
                 "("
                 "Lnet/janrupf/ultralight/javascript/interop/JavascriptObjectHasInstanceTester;"
                 ")Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_has_instance),
             NATIVE_METHOD(
                 "onConvertToType",
                 "("
                 "Lnet/janrupf/ultralight/javascript/interop/JavascriptObjectToTypeConverter;"
                 ")Lnet/janrupf/ultralight/javascript/JavascriptClassDefinition;",
                 JavascriptClassDefinitionJNI::on_convert_to_type),
             NATIVE_METHOD("createEmpty", "()J", JavascriptClassDefinitionJNI::create_empty),
             NATIVE_METHOD("free", "(J)V", JavascriptClassDefinitionJNI::free)};

        runtime.bridged_logger = nullptr;
        runtime.bridged_file_system = nullptr;
        runtime.bridged_clipboard = nullptr;
    }
} // namespace ultralight_java
