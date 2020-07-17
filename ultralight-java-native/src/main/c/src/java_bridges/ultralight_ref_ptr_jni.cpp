#include "ultralight_java/java_bridges/ultralight_ref_ptr_jni.hpp"

namespace ultralight_java {
    void UltralightRefPtrJNI::_delete(JNIEnv *, jclass, jlong handle) {
        // Get back the wrapper
        auto *ptr = reinterpret_cast<WrappedRefPtr *>(handle);

        // Invoke the deleter to delete the wrapped pointer and
        // then delete the wrapper itself
        ptr->deleter(ptr->ptr_value);
        delete ptr;
    }
}