#pragma once

#include <jni.h>
#include <Ultralight/Ultralight.h>

#include <unordered_map>

#include "ultralight_java/java_bridges/bridged_view_listener.hpp"
#include "ultralight_java/java_bridges/bridged_load_listener.hpp"

namespace ultralight_java {
    /**
     * Class for interfacing with ultralight::View from java.
     */
    class UltralightViewJNI {
    private:
        static std::unordered_map<ultralight::View *, BridgedViewListener *> existing_view_listeners;
        static std::unordered_map<ultralight::View *, BridgedLoadListener *> existing_load_listeners;

    public:
        /**
         * Deletes all active listeners, it is assumed that at this point the
         * Ultralight platform is not performing any work anymore.
         */
        static void clean_up();

        /**
         * Retrieves the URL from this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @return The current URL of the view
         */
        static jstring url(JNIEnv *env, jobject instance);

        /**
         * Retrieves the title from this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @return The current title of the view
         */
        static jstring title(JNIEnv *env, jobject instance);

        /**
         * Retrieves the width of this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @return The current width of the view
         */
        static jlong width(JNIEnv *env, jobject instance);

        /**
         * Retrieves the height of this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @return The current height of the view
         */
        static jlong height(JNIEnv *env, jobject instance);

        /**
        * Retrieves whether this view is loading.
        *
        * @param env The JNI environment to use for accessing java
        * @param instance The java instance of the view
        * @return Whether this view is loading
        */
        static jboolean is_loading(JNIEnv *env, jobject instance);

        /**
         * Retrieves the surface backing this view. Only valid when GPU rendering
         * is disabled.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @return The surface backing this view, or nullptr, if GPU rendering is enabled
         */
        static jobject surface(JNIEnv *env, jobject instance);

        /**
         * Loads a string of HTML into this view
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @param html The HTML string to load
         * @param url The URL to emulate, or null for an empty URL
         * @param add_to_history Whether this load should be added to the history
         */
        static void load_html(JNIEnv *env, jobject instance, jstring html, jstring url, jboolean add_to_history);

        /**
         * Loads content from an URL into this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @param url The URL to load the content from into this view
         */
        static void load_url(JNIEnv *env, jobject instance, jstring url);

        /**
         * Resizes this view to a new size.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @param width The new width of the view
         * @param height The new height of the view
         */
        static void resize(JNIEnv *env, jobject instance, jlong width, jlong height);

        /**
         * Retrieves whether this view is loading.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @param script The script to evaluate in this view
         * @return The result of the evaluation as a string, or nullptr, possibly throws a
         * a java exception.
         */
        static jstring evaluate_script(JNIEnv *env, jobject instance, jstring script);

        /**
         * Checks if this view is able to navigate back.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @return Whether this view is able to navigate back
         */
        static jboolean can_go_back(JNIEnv *env, jobject instance);

        /**
         * Checks if this view is able to navigate forward.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @return Whether this view is able to navigate forward
         */
        static jboolean can_go_forward(JNIEnv *env, jobject instance);

        /**
         * Navigates this view backwards.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         */
        static void go_back(JNIEnv *env, jobject instance);

        /**
         * Navigates this view forwards.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         */
        static void go_forward(JNIEnv *env, jobject instance);

        /**
         * Navigates this view to a certain point in this history.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @param offset The new offset in the history
         */
        static void go_to_history_offset(JNIEnv *env, jobject instance, jint offset);

        /**
         * Reloads this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         */
        static void reload(JNIEnv *env, jobject instance);

        /**
         * Stops all loading of this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         */
        static void stop(JNIEnv *env, jobject instance);

        /**
         * Gives focus to this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         */
        static void focus(JNIEnv *env, jobject instance);

        /**
         * Removes focus from this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         */
        static void unfocus(JNIEnv *env, jobject instance);

        /**
         * Checks if this view has focus.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @return Whether this view has focus
         */
        static jboolean has_focus(JNIEnv *env, jobject instance);

        /**
         * Checks if this view has input focus.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @return Whether this view has input focus
         */
        static jboolean has_input_focus(JNIEnv *env, jobject instance);

        /**
         * Sends a key event to this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @param event The event to fire
         */
        static void fire_key_event(JNIEnv *env, jobject instance, jobject event);

        /**
         * Sends a mouse event to this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @param event The event to fire
         */
        static void fire_mouse_event(JNIEnv *env, jobject instance, jobject event);

        /**
         * Sends a scroll event to this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @param event The event to fire
         */
        static void fire_scroll_event(JNIEnv *env, jobject instance, jobject event);

        /**
         * Sets the view listener for this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @param listener The listener to set on this view
         */
        static void set_view_listener(JNIEnv *env, jobject instance, jobject listener);

        /**
         * Sets the load listener for this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @param listener The listener to set on this view
         */
        static void set_load_listener(JNIEnv *env, jobject instance, jobject listener);

        /**
         * Sets the if this view needs a repaint.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @param needs_paint The new status of the repaint requirement
         */
        static void set_needs_paint(JNIEnv *env, jobject instance, jboolean needs_paint);

        /**
         * Checks if this view needs a repaint
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @return Whether this view needs a repaint
         */
        static jboolean needs_paint(JNIEnv *env, jobject instance);

        /**
         * Retrieves the inspector view for this view.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the view
         * @return The java object for inspector view of this view
         */
        static jobject inspector(JNIEnv *env, jobject instance);
    };
}