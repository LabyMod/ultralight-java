#pragma once

#include <jni.h>
#include <array>

#include "ultralight_java/java_bridges/ultralight_bitmap_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_bitmap_surface_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_key_event_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_platform_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_ref_ptr_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_renderer_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_surface_jni.hpp"
#include "ultralight_java/java_bridges/ultralight_view_jni.hpp"

namespace ultralight_java {
    class BridgedLogger;

    struct UltralightJavaRuntime {
        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /**
             * Method returning the native pointer of the underlying C(++) object
             */
            jmethodID get_handle_method = nullptr;
        } object_with_handle;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /**
             * Constructor taking a long as a handle
             */
            jmethodID constructor = nullptr;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 5> native_methods = {
                    JNINativeMethod{
                            const_cast<char *>("instance"),
                            const_cast<char *>("()Lnet/janrupf/ultralight/UltralightPlatform;"),
                            reinterpret_cast<void *>(&UltralightPlatformJNI::instance)
                    },
                    JNINativeMethod{
                            const_cast<char *>("setConfig"),
                            const_cast<char *>("(Lnet/janrupf/ultralight/config/UltralightConfig;)V"),
                            reinterpret_cast<void *>(&UltralightPlatformJNI::set_config)
                    },
                    JNINativeMethod{
                            const_cast<char *>("usePlatformFontLoader"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightPlatformJNI::use_platform_font_loader)
                    },
                    JNINativeMethod{
                            const_cast<char *>("usePlatformFileSystem"),
                            const_cast<char *>("(Ljava/lang/String;)V"),
                            reinterpret_cast<void *>(&UltralightPlatformJNI::use_platform_file_system)
                    },
                    JNINativeMethod{
                            const_cast<char *>("setLogger"),
                            const_cast<char *>("(Lnet/janrupf/ultralight/plugin/logging/UltralightLogger;)V"),
                            reinterpret_cast<void *>(&UltralightPlatformJNI::set_logger)
                    }
            };
        } ultralight_platform;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /* String */      jfieldID resource_path_field = nullptr;
            /* String */      jfieldID cache_path_field = nullptr;
            /* boolean */     jfieldID use_gpu_renderer_field = nullptr;
            /* double */      jfieldID device_scale_field = nullptr;
            /* FaceWinding */ jfieldID face_winding_field = nullptr;
            /* boolean */     jfieldID enable_images_field = nullptr;
            /* boolean */     jfieldID enable_javascript_field = nullptr;
            /* FontHinting */ jfieldID font_hinting_field = nullptr;
            /* double */      jfieldID font_gamma_field = nullptr;
            /* String */      jfieldID font_family_standard_field = nullptr;
            /* String */      jfieldID font_family_fixed_field = nullptr;
            /* String */      jfieldID font_family_serif_field = nullptr;
            /* String */      jfieldID font_family_sans_serif_field = nullptr;
            /* String */      jfieldID user_agent_field = nullptr;
            /* String */      jfieldID user_stylesheet_field = nullptr;
            /* boolean */     jfieldID force_repaint_field = nullptr;
            /* double */      jfieldID animation_timer_delay_field = nullptr;
            /* double */      jfieldID scroll_timer_delay_field = nullptr;
            /* double */      jfieldID recycle_delay_field = nullptr;
            /* long */        jfieldID memory_cache_size_field = nullptr;
            /* long */        jfieldID page_cache_size_field = nullptr;
            /* long */        jfieldID override_ram_size_field = nullptr;
            /* long */        jfieldID min_large_heap_size_field = nullptr;
            /* long */        jfieldID min_small_heap_size_field = nullptr;
        } ultralight_config;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /* FaceWinding */ jfieldID clockwise_field = nullptr;
            /* FaceWinding */ jfieldID counter_clockwise_field = nullptr;
        } face_winding;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /* FontHinting */ jfieldID smooth_field = nullptr;
            /* FontHinting */ jfieldID normal_field = nullptr;
            /* FontHinting */ jfieldID monochrome_field = nullptr;
        } font_hinting;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /*
             * Method taking a log level and message to log
             */
            jmethodID log_message_method = nullptr;
        } logger;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /* LogLevel */ jfieldID error_field = nullptr;
            /* LogLevel */ jfieldID warning_field = nullptr;
            /* LogLevel */ jfieldID info_field = nullptr;
        } log_level;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /**
             * Constructor taking a long as a handle
             */
            jmethodID constructor = nullptr;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 1> native_methods = {
                    JNINativeMethod{
                            const_cast<char *>("delete"),
                            const_cast<char *>("(J)V"),
                            reinterpret_cast<void *>(&UltralightRefPtrJNI::_delete)
                    }
            };
        } ref_ptr;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /**
             * Constructor taking a java RefPtr as handle
             */
            jmethodID constructor = nullptr;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 6> native_methods = {
                    JNINativeMethod{
                            const_cast<char *>("create"),
                            const_cast<char *>("()Lnet/janrupf/ultralight/UltralightRenderer;"),
                            reinterpret_cast<void *>(&UltralightRendererJNI::create)
                    },
                    JNINativeMethod{
                            const_cast<char *>("createView"),
                            const_cast<char *>("(JJZ)Lnet/janrupf/ultralight/UltralightView;"),
                            reinterpret_cast<void *>(&UltralightRendererJNI::createView)
                    },
                    JNINativeMethod{
                            const_cast<char *>("update"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightRendererJNI::update)
                    },
                    JNINativeMethod{
                            const_cast<char *>("render"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightRendererJNI::render)
                    },
                    JNINativeMethod{
                            const_cast<char *>("purgeMemory"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightRendererJNI::purgeMemory)
                    },
                    JNINativeMethod{
                            const_cast<char *>("logMemoryUsage"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightRendererJNI::logMemoryUsage)
                    }
            };
        } ultralight_renderer;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /**
             * Constructor taking a java RefPtr as handle
             */
            jmethodID constructor = nullptr;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 28> native_methods = {
                    JNINativeMethod{
                            const_cast<char *>("url"),
                            const_cast<char *>("()Ljava/lang/String;"),
                            reinterpret_cast<void *>(&UltralightViewJNI::url)
                    },
                    JNINativeMethod{
                            const_cast<char *>("title"),
                            const_cast<char *>("()Ljava/lang/String;"),
                            reinterpret_cast<void *>(&UltralightViewJNI::title)
                    },
                    JNINativeMethod{
                            const_cast<char *>("width"),
                            const_cast<char *>("()J"),
                            reinterpret_cast<void *>(&UltralightViewJNI::width)
                    },
                    JNINativeMethod{
                            const_cast<char *>("height"),
                            const_cast<char *>("()J"),
                            reinterpret_cast<void *>(&UltralightViewJNI::height)
                    },
                    JNINativeMethod{
                            const_cast<char *>("isLoading"),
                            const_cast<char *>("()Z"),
                            reinterpret_cast<void *>(&UltralightViewJNI::is_loading)
                    },
                    JNINativeMethod{
                            const_cast<char *>("surface"),
                            const_cast<char *>("()Lnet/janrupf/ultralight/UltralightSurface;"),
                            reinterpret_cast<void *>(&UltralightViewJNI::surface)
                    },
                    JNINativeMethod{
                            const_cast<char *>("loadHTML"),
                            const_cast<char *>("(Ljava/lang/String;Ljava/lang/String;Z)V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::load_html)
                    },
                    JNINativeMethod{
                            const_cast<char *>("loadURL"),
                            const_cast<char *>("(Ljava/lang/String;)V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::load_url)
                    },
                    JNINativeMethod{
                            const_cast<char *>("resize"),
                            const_cast<char *>("(JJ)V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::resize)
                    },
                    JNINativeMethod{
                            const_cast<char *>("evaluateScript"),
                            const_cast<char *>("(Ljava/lang/String;)Ljava/lang/String;"),
                            reinterpret_cast<void *>(&UltralightViewJNI::evaluate_script)
                    },
                    JNINativeMethod{
                            const_cast<char *>("canGoBack"),
                            const_cast<char *>("()Z"),
                            reinterpret_cast<void *>(&UltralightViewJNI::can_go_back)
                    },
                    JNINativeMethod{
                            const_cast<char *>("canGoForward"),
                            const_cast<char *>("()Z"),
                            reinterpret_cast<void *>(&UltralightViewJNI::can_go_forward)
                    },
                    JNINativeMethod{
                            const_cast<char *>("goBack"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::go_back)
                    },
                    JNINativeMethod{
                            const_cast<char *>("goForward"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::go_forward)
                    },
                    JNINativeMethod{
                            const_cast<char *>("goToHistoryOffset"),
                            const_cast<char *>("(I)V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::go_to_history_offset)
                    },
                    JNINativeMethod{
                            const_cast<char *>("reload"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::reload)
                    },
                    JNINativeMethod{
                            const_cast<char *>("stop"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::stop)
                    },
                    JNINativeMethod{
                            const_cast<char *>("focus"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::focus)
                    },
                    JNINativeMethod{
                            const_cast<char *>("unfocus"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::unfocus)
                    },
                    JNINativeMethod{
                            const_cast<char *>("hasFocus"),
                            const_cast<char *>("()Z"),
                            reinterpret_cast<void *>(&UltralightViewJNI::has_focus)
                    },
                    JNINativeMethod{
                            const_cast<char *>("hasInputFocus"),
                            const_cast<char *>("()Z"),
                            reinterpret_cast<void *>(&UltralightViewJNI::has_input_focus)
                    },
                    JNINativeMethod{
                            const_cast<char *>("fireKeyEvent"),
                            const_cast<char *>("(Lnet/janrupf/ultralight/input/UltralightKeyEvent;)V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::fire_key_event)
                    },
                    JNINativeMethod{
                            const_cast<char *>("fireMouseEvent"),
                            const_cast<char *>("(Lnet/janrupf/ultralight/input/UltralightMouseEvent;)V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::fire_mouse_event)
                    },
                    JNINativeMethod{
                            const_cast<char *>("fireScrollEvent"),
                            const_cast<char *>("(Lnet/janrupf/ultralight/input/UltralightScrollEvent;)V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::fire_scroll_event)
                    },
                    JNINativeMethod{
                            const_cast<char *>("setLoadListener"),
                            const_cast<char *>("(Lnet/janrupf/ultralight/plugin/loading/UltralightLoadListener;)V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::set_load_listener)
                    },
                    JNINativeMethod{
                            const_cast<char *>("setNeedsPaint"),
                            const_cast<char *>("(Z)V"),
                            reinterpret_cast<void *>(&UltralightViewJNI::set_needs_paint)
                    },
                    JNINativeMethod{
                            const_cast<char *>("needsPaint"),
                            const_cast<char *>("()Z"),
                            reinterpret_cast<void *>(&UltralightViewJNI::needs_paint)
                    },
                    JNINativeMethod{
                            const_cast<char *>("inspector"),
                            const_cast<char *>("()Lnet/janrupf/ultralight/UltralightView;"),
                            reinterpret_cast<void *>(&UltralightViewJNI::inspector)
                    }
            };
        } ultralight_view;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /**
             * Constructor taking 4 ints as the bounds in coordinates
             */
            jmethodID bounds_constructor = nullptr;

            /* int */ jfieldID left_field = nullptr;
            /* int */ jfieldID top_field = nullptr;
            /* int */ jfieldID right_field = nullptr;
            /* int */ jfieldID bottom_field = nullptr;
        } int_rect;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /**
             * Constructor taking a UltralightView and a long as a handle
             */
            jmethodID constructor = nullptr;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 10> native_methods = {
                    JNINativeMethod{
                            const_cast<char *>("width"),
                            const_cast<char *>("()J"),
                            reinterpret_cast<void *>(&UltralightSurfaceJNI::width)
                    },
                    JNINativeMethod{
                            const_cast<char *>("height"),
                            const_cast<char *>("()J"),
                            reinterpret_cast<void *>(&UltralightSurfaceJNI::height)
                    },
                    JNINativeMethod{
                            const_cast<char *>("rowBytes"),
                            const_cast<char *>("()J"),
                            reinterpret_cast<void *>(&UltralightSurfaceJNI::rowBytes)
                    },
                    JNINativeMethod{
                            const_cast<char *>("size"),
                            const_cast<char *>("()J"),
                            reinterpret_cast<void *>(&UltralightSurfaceJNI::size)
                    },
                    JNINativeMethod{
                            const_cast<char *>("lockPixels"),
                            const_cast<char *>("()Ljava/nio/ByteBuffer;"),
                            reinterpret_cast<void *>(&UltralightSurfaceJNI::lockPixels)
                    },
                    JNINativeMethod{
                            const_cast<char *>("unlockPixels"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightSurfaceJNI::unlockPixels)
                    },
                    JNINativeMethod{
                            const_cast<char *>("resize"),
                            const_cast<char *>("(JJ)V"),
                            reinterpret_cast<void *>(&UltralightSurfaceJNI::resize)
                    },
                    JNINativeMethod{
                            const_cast<char *>("setDirtyBounds"),
                            const_cast<char *>("(Lnet/janrupf/ultralight/math/IntRect;)V"),
                            reinterpret_cast<void *>(&UltralightSurfaceJNI::setDirtyBounds)
                    },
                    JNINativeMethod{
                            const_cast<char *>("dirtyBounds"),
                            const_cast<char *>("()Lnet/janrupf/ultralight/math/IntRect;"),
                            reinterpret_cast<void *>(&UltralightSurfaceJNI::dirtyBounds)
                    },
                    JNINativeMethod{
                            const_cast<char *>("clearDirtyBounds"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightSurfaceJNI::clearDirtyBounds)
                    }
            };
        } ultralight_surface;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /**
             * Constructor taking a UltralightView and a long as a handle
             */
            jmethodID constructor = nullptr;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 1> native_methods = {
                    JNINativeMethod{
                            const_cast<char *>("bitmap"),
                            const_cast<char *>("()Lnet/janrupf/ultralight/bitmap/UltralightBitmap;"),
                            reinterpret_cast<void *>(&UltralightBitmapSurfaceJNI::bitmap)
                    }
            };
        } ultralight_bitmap_surface;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /* UltralightBitmapFormat */ jfieldID a8_unorm_field = nullptr;
            /* UltralightBitmapFormat */ jfieldID bgra8_unorm_srgb_field = nullptr;
        } ultralight_bitmap_format;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /**
             * Constructor taking a RefPtr as handle
             */
            jmethodID constructor = nullptr;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 22> native_methods = {
                    JNINativeMethod{
                            const_cast<char *>("create"),
                            const_cast<char *>("()Lnet/janrupf/ultralight/bitmap/UltralightBitmap;"),
                            reinterpret_cast<void *>(static_cast<jobject(*)(JNIEnv *,
                                                                            jclass)>(UltralightBitmapJNI::create))
                    },
                    JNINativeMethod{
                            const_cast<char *>("create"),
                            const_cast<char *>(
                                    "("
                                    "J"
                                    "J"
                                    "Lnet/janrupf/ultralight/bitmap/UltralightBitmapFormat;"
                                    ")Lnet/janrupf/ultralight/bitmap/UltralightBitmap;"),
                            reinterpret_cast<void *>(static_cast<jobject(*)(JNIEnv *, jclass, jlong, jlong,
                                                                            jobject)>(&UltralightBitmapJNI::create))
                    },
                    JNINativeMethod{
                            const_cast<char *>("create"),
                            const_cast<char *>(
                                    "("
                                    "J"
                                    "J"
                                    "Lnet/janrupf/ultralight/bitmap/UltralightBitmapFormat;"
                                    "J"
                                    "Ljava/nio/ByteBuffer;"
                                    "Z"
                                    ")Lnet/janrupf/ultralight/bitmap/UltralightBitmap;"),
                            reinterpret_cast<void *>(static_cast<jobject(*)(JNIEnv *, jclass, jlong, jlong, jobject,
                                                                            jlong, jobject,
                                                                            jboolean)>(&UltralightBitmapJNI::create))
                    },
                    JNINativeMethod{
                            const_cast<char *>("create"),
                            const_cast<char *>(
                                    "("
                                    "Lnet/janrupf/ultralight/bitmap/UltralightBitmap;"
                                    ")Lnet/janrupf/ultralight/bitmap/UltralightBitmap;"),
                            reinterpret_cast<void *>(static_cast<jobject(*)(JNIEnv *, jclass,
                                                                            jobject)>(&UltralightBitmapJNI::create))
                    },
                    JNINativeMethod{
                            const_cast<char *>("width"),
                            const_cast<char *>("()J"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::width)
                    },
                    JNINativeMethod{
                            const_cast<char *>("height"),
                            const_cast<char *>("()J"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::height)
                    },
                    JNINativeMethod{
                            const_cast<char *>("bounds"),
                            const_cast<char *>("()Lnet/janrupf/ultralight/math/IntRect;"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::bounds)
                    },
                    JNINativeMethod{
                            const_cast<char *>("format"),
                            const_cast<char *>("()Lnet/janrupf/ultralight/bitmap/UltralightBitmapFormat;"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::format)
                    },
                    JNINativeMethod{
                            const_cast<char *>("bpp"),
                            const_cast<char *>("()J"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::bpp)
                    },
                    JNINativeMethod{
                            const_cast<char *>("rowBytes"),
                            const_cast<char *>("()J"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::row_bytes)
                    },
                    JNINativeMethod{
                            const_cast<char *>("size"),
                            const_cast<char *>("()J"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::size)
                    },
                    JNINativeMethod{
                            const_cast<char *>("ownsPixels"),
                            const_cast<char *>("()Z"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::owns_pixels)
                    },
                    JNINativeMethod{
                            const_cast<char *>("lockPixels"),
                            const_cast<char *>("()Ljava/nio/ByteBuffer;"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::lock_pixels)
                    },
                    JNINativeMethod{
                            const_cast<char *>("unlockPixels"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::unlock_pixels)
                    },
                    JNINativeMethod{
                            const_cast<char *>("rawPixels"),
                            const_cast<char *>("()Ljava/nio/ByteBuffer;"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::raw_pixels)
                    },
                    JNINativeMethod{
                            const_cast<char *>("isEmpty"),
                            const_cast<char *>("()Z"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::is_emtpy)
                    },
                    JNINativeMethod{
                            const_cast<char *>("erase"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::erase)
                    },
                    JNINativeMethod{
                            const_cast<char *>("set"),
                            const_cast<char *>("(Lnet/janrupf/ultralight/bitmap/UltralightBitmap;)V"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::set)
                    },
                    JNINativeMethod{
                            const_cast<char *>("drawBitmap"),
                            const_cast<char *>(
                                    "("
                                    "Lnet/janrupf/ultralight/math/IntRect;"
                                    "Lnet/janrupf/ultralight/math/IntRect;"
                                    "Lnet/janrupf/ultralight/bitmap/UltralightBitmap;"
                                    "Z"
                                    ")Z"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::width)
                    },
                    JNINativeMethod{
                            const_cast<char *>("writePNG"),
                            const_cast<char *>("(Ljava/lang/String;)Z"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::write_png)
                    },
                    JNINativeMethod{
                            const_cast<char *>("resample"),
                            const_cast<char *>("(Lnet/janrupf/ultralight/bitmap/UltralightBitmap;Z)Z"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::resample)
                    },
                    JNINativeMethod{
                            const_cast<char *>("swapRedBlueChannels"),
                            const_cast<char *>("()V"),
                            reinterpret_cast<void *>(&UltralightBitmapJNI::swap_red_blue_channels)
                    }
            };
        } ultralight_bitmap;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            jmethodID on_begin_loading_method = nullptr;
            jmethodID on_finish_loading_method = nullptr;
            jmethodID on_fail_loading_method = nullptr;
            jmethodID on_update_history_method = nullptr;
            jmethodID on_window_object_ready_method = nullptr;
            jmethodID on_dom_ready_method = nullptr;
        } ultralight_load_listener;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /* int */ jfieldID id_field = nullptr;
        } ultralight_key;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /* UltralightKeyEventType */ jfieldID type_field = nullptr;
            /* int */ jfieldID modifiers_field = nullptr;
            /* UltralightKey */ jfieldID virtual_key_code_field = nullptr;
            /* int */ jfieldID native_key_code_field = nullptr;
            /* String */ jfieldID key_identifier_field = nullptr;
            /* String */ jfieldID text_field = nullptr;
            /* String */ jfieldID unmodified_text_field = nullptr;
            /* boolean */ jfieldID is_keypad_field = nullptr;
            /* boolean */ jfieldID is_auto_repeat_field = nullptr;
            /* boolean */ jfieldID is_system_key_field = nullptr;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 1> native_methods = {
                    JNINativeMethod{
                            const_cast<char *>("getKeyIdentifierFromVirtualKeyCode"),
                            const_cast<char *>("(Lnet/janrupf/ultralight/input/UltralightKey;)Ljava/lang/String;"),
                            reinterpret_cast<void *>(&UltralightKeyEventJNI::get_key_identifier_from_virtual_key_code)
                    }
            };
        } ultralight_key_event;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /* UltralightKeyEventType */ jfieldID down_field = nullptr;
            /* UltralightKeyEventType */ jfieldID up_field = nullptr;
            /* UltralightKeyEventType */ jfieldID raw_down_field = nullptr;
            /* UltralightKeyEventType */ jfieldID char_field = nullptr;
        } ultralight_key_event_type;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /* UltralightMouseEventType */ jfieldID type_field = nullptr;
            /* int */ jfieldID x_field = nullptr;
            /* int */ jfieldID y_field = nullptr;
            /* UltralightMouseButton */ jfieldID button_field = nullptr;
        } ultralight_mouse_event;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /* UltralightMouseEventButton */ jfieldID left_field = nullptr;
            /* UltralightMouseEventButton */ jfieldID middle_field = nullptr;
            /* UltralightMouseEventButton */ jfieldID right_field = nullptr;
        } ultralight_mouse_event_button;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /* UltralightMouseEventType */ jfieldID moved_field = nullptr;
            /* UltralightMouseEventType */ jfieldID down_field = nullptr;
            /* UltralightMouseEventType */ jfieldID up_field = nullptr;
        } ultralight_mouse_event_type;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /* UltralightScrollEventType */ jfieldID type_field = nullptr;
            /* int */ jfieldID delta_x_field = nullptr;
            /* int */ jfieldID delta_y_field = nullptr;
        } ultralight_scroll_event;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;

            /* UltralightScrollEventType */ jfieldID by_pixel_field = nullptr;
            /* UltralightScrollEventType */ jfieldID by_page_field = nullptr;
        } ultralight_scroll_event_type;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;
        } javascript_exception;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;
        } null_pointer_exception;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;
        } illegal_state_exception;

        struct {
            /**
             * The java class itself
             */
            jclass clazz = nullptr;
        } illegal_argument_exception;

        /**
         * The JavaVM instance executing the virtual machine.
         */
        JavaVM *vm = nullptr;

        /**
         * Current global logger instance, or nullptr, if none
         */
        BridgedLogger *bridged_logger = nullptr;
    };

    /**
     * Global runtime struct for easy access of constants.
     */
    extern UltralightJavaRuntime runtime;

    /**
     * Initializes the library using the JavaVM instance.
     *
     * @param vm The currently executing JavaVM
     * @return
     */
    int setup(JavaVM *vm);

    /**
     * Frees global references.
     *
     * @param vm The currently executing JavaVM
     */
    void destruct(JavaVM *vm);
}