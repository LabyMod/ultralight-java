package com.labymedia.ultralight.math;

import com.labymedia.ultralight.annotation.NativeType;

import java.util.Arrays;

@NativeType("ultralight::Vec4")
public class Vec4 {
    private final float[] value = new float[4];

    public Vec4() {
    }

    public Vec4(float x, float y, float z, float w) {
        this.set(x, y, z, w);
    }

    public Vec4(float value) {
        set(value, value, value, value);
    }

    public Vec4(float[] value) {
        for (int i = 0; i < 4; i++) {
            this.value[i] = value[i];
        }
    }

    public void set(float x, float y, float z, float w) {
        this.value[0] = x;
        this.value[1] = y;
        this.value[2] = z;
        this.value[3] = w;
    }

    public float[] getValue() {
        return value;
    }

    public float getX() {
        return value[0];
    }

    public float getY() {
        return value[1];
    }

    public float getZ() {
        return value[2];
    }

    public float getW() {
        return value[3];
    }


    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Vec4 vec4 = (Vec4) o;
        return Arrays.equals(value, vec4.value);
    }

    @Override
    public int hashCode() {
        return Arrays.hashCode(value);
    }
}
