/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 - 2021 LabyMedia and contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

package com.labymedia.ultralight.databind.call.property.generated;

import javassist.CannotCompileException;
import javassist.ClassPool;
import javassist.CtClass;
import javassist.CtNewMethod;
import javassist.NotFoundException;

import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.security.SecureClassLoader;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;
import java.util.function.Consumer;

public class PropertyCallerGenerator {

    private static final Class<?> GENERATION_INTERFACE = SingleGeneratedPropertyCaller.class;

    private static final Method CALL_METHOD = GENERATION_INTERFACE.getDeclaredMethods()[0];

    private static final String CLASS_NAME_BASE = "Generated" + GENERATION_INTERFACE + "_%s";

    private static final String INSTANCE_PARAMETER_NAME = "instance";

    private static final String PARAMETERS_PARAMETER_NAME = "parameters";

    private static final Map<Class<?>, Class<?>> PRIMITIVE_TO_WRAPPER = new HashMap<Class<?>, Class<?>>() {
        {
            this.put(boolean.class, Boolean.class);
            this.put(byte.class, Byte.class);
            this.put(char.class, Character.class);
            this.put(double.class, Double.class);
            this.put(float.class, Float.class);
            this.put(int.class, Integer.class);
            this.put(long.class, Long.class);
            this.put(short.class, Short.class);
        }
    };

    private final DefinableClassLoader classLoader;

    private final ClassPool classPool;

    public PropertyCallerGenerator() {
        this.classLoader = new DefinableClassLoader(this.getClass().getClassLoader());
        this.classPool = ClassPool.getDefault();
    }

    public SingleGeneratedPropertyCaller generateMethodCaller(Method method) throws NotFoundException, CannotCompileException, IOException, InvocationTargetException, NoSuchMethodException, InstantiationException, IllegalAccessException {
        boolean returnsVoid = method.getReturnType().equals(void.class);

        StringBuilder methodContentBuilder = new StringBuilder(returnsVoid ? "" : "return ");
        this.wrapPrimitiveType(methodContentBuilder, method.getReturnType(), builder -> {
            this.castInstance(builder, method.getDeclaringClass(), method.getModifiers())
                    .append(".")
                    .append(method.getName())
                    .append("(");
            this.appendParameters(builder, method.getParameterTypes())
                    .append(")");
        }).append(";");

        if (returnsVoid) {
            methodContentBuilder.append("return null;");
        }
        return this.generateCaller(methodContentBuilder.toString());
    }

    public SingleGeneratedPropertyCaller generateConstructorCaller(Constructor<?> constructor) throws NotFoundException, CannotCompileException, IOException, InvocationTargetException, NoSuchMethodException, InstantiationException, IllegalAccessException {
        String declaringClassName = constructor.getDeclaringClass().getName();

        StringBuilder methodContentBuilder = new StringBuilder("return new ")
                .append(declaringClassName)
                .append("(");
        this.appendParameters(methodContentBuilder, constructor.getParameterTypes()).append(");");
        return this.generateCaller(methodContentBuilder.toString());
    }

    public SingleGeneratedPropertyCaller generateFieldCaller(Field field) throws NotFoundException, CannotCompileException, IOException, InvocationTargetException, NoSuchMethodException, InstantiationException, IllegalAccessException {
        Class<?> type = field.getType();

        StringBuilder methodContentBuilder = new StringBuilder("if (")
                .append(PARAMETERS_PARAMETER_NAME)
                .append(" != null && ")
                .append(PARAMETERS_PARAMETER_NAME)
                .append(".length == 1) { ");

        this.castInstance(methodContentBuilder, field.getDeclaringClass(), field.getModifiers())
                .append(".")
                .append(field.getName())
                .append(" = ");

        this.castParameter(methodContentBuilder, type, 0)
                .append(";")
                .append(" return null; } return ");

        this.wrapPrimitiveType(methodContentBuilder, type, builder ->
                this.castInstance(builder, field.getDeclaringClass(), field.getModifiers())
                        .append(".")
                        .append(field.getName()))
                .append(";");
        return this.generateCaller(methodContentBuilder.toString());
    }

    private SingleGeneratedPropertyCaller generateCaller(String methodContent) throws NotFoundException, CannotCompileException, IOException, NoSuchMethodException, InvocationTargetException, InstantiationException, IllegalAccessException {
        CtClass ctClass = this.classPool.makeClass(
                String.format(CLASS_NAME_BASE, UUID.randomUUID().toString().replace("-", "")));
        ctClass.addInterface(this.classPool.get(GENERATION_INTERFACE.getName()));

        ctClass.addMethod(CtNewMethod.make(String.format(
                "public Object %s(Object %s, Object[] %s) { %s }",
                CALL_METHOD.getName(),
                INSTANCE_PARAMETER_NAME,
                PARAMETERS_PARAMETER_NAME,
                methodContent), ctClass));

        Class<?> generatedClass = this.classLoader.defineClass(ctClass.getName(), ctClass.toBytecode());
        return (SingleGeneratedPropertyCaller) generatedClass.getConstructor().newInstance();
    }

    private StringBuilder castInstance(StringBuilder builder, Class<?> type, int propertyModifiers) {
        boolean isStatic = Modifier.isStatic(propertyModifiers);

        if (!isStatic) {
            builder.append("((")
                    .append(this.getCastType(type))
                    .append(")");
        }
        builder.append(isStatic ? type.getName() : INSTANCE_PARAMETER_NAME);
        if (!isStatic) {
            builder.append(")");
        }

        return builder;
    }

    private StringBuilder castParameter(StringBuilder builder, Class<?> type, int index) {
        Class<?> wrappedType = PRIMITIVE_TO_WRAPPER.get(type);
        boolean isWrapped = wrappedType != null;

        builder.append("((")
                .append(this.getCastType(type))
                .append(")")
                .append(PARAMETERS_PARAMETER_NAME)
                .append("[")
                .append(index)
                .append("]")
                .append(")");

        if (isWrapped) {
            builder.append(".")
                    .append(type.getName())
                    .append("Value()");
        }

        return builder;
    }

    private String getCastType(Class<?> type) {
        Class<?> wrappedType = PRIMITIVE_TO_WRAPPER.get(type);
        Class<?> rawType = type.getComponentType() != null ? type.getComponentType() : type;

        StringBuilder builder = new StringBuilder(wrappedType != null ? wrappedType.getName() : rawType.getName());
        for (; type.isArray(); type = type.getComponentType()) {
            builder.append("[]");
        }

        return builder.toString();
    }

    private StringBuilder wrapPrimitiveType(StringBuilder builder, Class<?> type, Consumer<StringBuilder> propertyReturner) {
        Class<?> wrappedType = PRIMITIVE_TO_WRAPPER.get(type);
        boolean isWrapped = wrappedType != null;

        if (isWrapped) {
            builder.append(wrappedType.getName())
                    .append(".valueOf(");
        }

        propertyReturner.accept(builder);

        if (isWrapped) {
            builder.append(")");
        }

        return builder;
    }

    private StringBuilder appendParameters(StringBuilder builder, Class<?>[] parameterTypes) {
        for (int i = 0; i < parameterTypes.length; i++) {
            Class<?> parameterType = parameterTypes[i];
            this.castParameter(builder, parameterType, i);

            if (parameterTypes.length != (i + 1)) {
                builder.append(",");
            }
        }

        return builder;
    }

    private static class DefinableClassLoader extends SecureClassLoader {

        public DefinableClassLoader(ClassLoader parent) {
            super(parent);
        }

        public Class<?> defineClass(String className, byte[] byteCode) {
            return super.defineClass(className, byteCode, 0, byteCode.length);
        }
    }
}
