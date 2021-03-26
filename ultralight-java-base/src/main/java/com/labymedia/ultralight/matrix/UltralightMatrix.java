package com.labymedia.ultralight.matrix;

import com.labymedia.ultralight.annotation.NativeType;

@NativeType("ultralight::Matrix")
public class UltralightMatrix {
    native void set(
            @NativeType("double") double m11, @NativeType("double") double m12, @NativeType("double") double m13, @NativeType("double") double m14,
            @NativeType("double") double m21, @NativeType("double") double m22, @NativeType("double") double m23, @NativeType("double") double m24,
            @NativeType("double") double m31, @NativeType("double") double m32, @NativeType("double") double m33, @NativeType("double") double m34,
            @NativeType("double") double m41, @NativeType("double") double m42, @NativeType("double") double m43, @NativeType("double") double m44);
}
