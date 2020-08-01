#include "ultralight_java/platform/managed_javascript_class.hpp"

namespace ultralight_java {
    ManagedJavascriptFunctionContainer::ManagedJavascriptFunctionContainer()
        : java_initializer(nullptr),
          java_finalizer(nullptr),
          java_has_property_tester(nullptr),
          java_property_getter(nullptr),
          java_property_setter(nullptr),
          java_property_deleter(nullptr),
          java_property_name_collector(nullptr),
          java_call_as_function(nullptr),
          java_call_as_constructor(nullptr),
          java_has_instance_tester(nullptr),
          java_to_type_converter(nullptr) {
    }

    ManagedJavascriptStaticField::ManagedJavascriptStaticField() : getter(nullptr), setter(nullptr), attributes(0) {
    }

    ManagedJavascriptStaticFunction::ManagedJavascriptStaticFunction() : function(nullptr), attributes(0) {
    }

    ManagedJavascriptPrivateData::ManagedJavascriptPrivateData() : real_data(nullptr) {
    }
} // namespace ultralight_java
