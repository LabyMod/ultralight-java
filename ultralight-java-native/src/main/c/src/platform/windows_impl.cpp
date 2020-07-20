#include "ultralight_java/platform/windows_impl.hpp"

#include <windows.h>

struct UltralightThreadStarterParam {
    std::function<void()> executor;
};

/**
 * Execution helper function, the param is a pointer to a
 * _UltralightThreadStarterParam
 */
extern "C" DWORD WINAPI _ultralight_java_thread_starter(void *param) {
    auto *starter = reinterpret_cast<UltralightThreadStarterParam *>(param);
    std::function<void()> executor = std::move(starter->executor);

    executor();

    return 0;
}

namespace ultralight_java {
    void WindowsImpl::create_thread(std::function<void()> executor) {
        // Create a start struct on the heap and pass it to the thread
        auto *starter = new UltralightThreadStarterParam{std::move(executor)};
        CreateThread(nullptr, 0, _ultralight_java_thread_starter, starter, 0, nullptr);
    }
} // namespace ultralight_java