#pragma once

#include <Ultralight/Ultralight.h>

#include "ultralight_java/util/jni_reference_wrapper.hpp"

namespace ultralight_java {
    /**
     * Bridge for net/janrupf/ultralight/plugin/loading/UltralightViewLoadListener to ultralight::LoadListener
     */
    class BridgedLoadListener : public ultralight::LoadListener, public JNIReferenceWrapper {
    public:
        /**
         * Constructs a new BridgedLoadListener wrapping an existing java listener.
         *
         * @param env The environment to use for wrapping the listener
         * @param listener The listener which should be wrapped
         */
        explicit BridgedLoadListener(JNIEnv *env, jobject listener);

        /**
         * Called when the view has begun loading.
         *
         * @param caller The view that has begun loading
         */
        void OnBeginLoading(ultralight::View *caller) final;

        /**
         * Called when the view has finished loading.
         *
         * @param caller The view that has finished loading
         */
        void OnFinishLoading(ultralight::View *caller) final;

        /**
         * Called when the view has failed to load.
         *
         * @param caller The view that has failed to load
         * @param url The url that failed to load
         * @param description A description of the error
         * @param error_domain The domain that failed to load
         * @param error_code The error code of the loading status
         */
        void OnFailLoading(
                ultralight::View *caller,
                const ultralight::String &url,
                const ultralight::String &description,
                const ultralight::String &error_domain,
                int error_code
        ) final;

        /**
         * Called when the view has updated the history.
         *
         * @param caller The view that has updated the history
         */
        void OnUpdateHistory(ultralight::View *caller) final;

        /**
         * Called when the window has been (re)created for the view.
         *
         * @param caller The view the window object has been recreated for
         */
        void OnWindowObjectReady(ultralight::View *caller) final;

        /**
         * Called when the DOM has been loaded for the view.
         *
         * @param caller The view the DOM has been loaded for
         */
        void OnDOMReady(ultralight::View *caller) final;
    };
}