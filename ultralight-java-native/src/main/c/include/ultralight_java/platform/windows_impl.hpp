#pragma once

#include <functional>

namespace ultralight_java {
    /**
     * Helper class for Windows only operations
     */
    class WindowsImpl {
    public:
        /**
         * Creates and dispatches a new Windows native thread.
         *
         * @param executor The function to execute on the new thread
         */
        static void create_thread(std::function<void()> executor);
    };
}