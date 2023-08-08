/**
 * @file Convert.h
 * @author shirokuma1101
 * @version 1.1
 * @date 2023-05-14
 *
 * @copyright Copyright (c) 2023 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_MATH_CONVERT_H_
#define GAME_LIBRARIES_MATH_CONVERT_H_

#include <list>
#include <tuple>
#include <type_traits>
#include <vector>

#include "Macro.h"

/**
 * @namespace convert
 * @brief Namespace for conversion functions.
 */
namespace convert {

    #define CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(func_name, arg_name, float_expr, double_expr) \
    MACRO_NAMESPACE_EXTERNAL_BEGIN                                                            \
    MACRO_NAMESPACE_INTERNAL_BEGIN                                                            \
    template<class ArgTy, class>                                                              \
    struct func_name {                                                                        \
        static constexpr auto Func(const ArgTy& arg_name) noexcept {                          \
            return float_expr;                                                                \
        }                                                                                     \
    };                                                                                        \
    template<class ArgTy>                                                                     \
    struct func_name<ArgTy, double> {                                                         \
        static constexpr auto Func(const ArgTy& arg_name) noexcept {                          \
            return double_expr;                                                               \
        }                                                                                     \
    };                                                                                        \
    MACRO_NAMESPACE_INTERNAL_END                                                              \
    template<class ArgTy>                                                                     \
    constexpr auto func_name(const ArgTy& arg_name) noexcept {                                \
        return detail::func_name<ArgTy, ArgTy>::Func(arg_name);                               \
    }                                                                                         \
    template<class ExpectTy, class ArgTy>                                                     \
    constexpr auto func_name(const ArgTy& arg_name) noexcept {                                \
        return detail::func_name<ArgTy, ExpectTy>::Func(arg_name);                            \
    }                                                                                         \
    MACRO_NAMESPACE_EXTERNAL_END

    // 1S = 1000MS = 1000000US = 1000000000NS
    // S to
    /**
     * @brief Converts a value in seconds to nanoseconds.
     * @tparam T The type of the input value.
     * @param t The input value in seconds.
     * @return The input value in nanoseconds.
     */
    template<class T> constexpr auto SToNS(T t) noexcept { return t * 1000000000; }
    /**
     * @brief Converts a value in seconds to microseconds.
     * @tparam T The type of the input value.
     * @param t The input value in seconds.
     * @return The input value in microseconds.
     */
    template<class T> constexpr auto SToUS(T t) noexcept { return t * 1000000; }
    /**
     * @brief Converts a value in seconds to milliseconds.
     * @tparam T The type of the input value.
     * @param t The input value in seconds.
     * @return The input value in milliseconds.
     */
    template<class T> constexpr auto SToMS(T t) noexcept { return t * 1000; }

    // MS to
    /**
     * Converts a value in milliseconds to nanoseconds.
     * @tparam T The type of the input value.
     * @param t The input value in milliseconds.
     * @return The input value in nanoseconds.
     */
    template<class T> constexpr auto MSToNS(T t) noexcept { return t * 1000000; }
    /**
     * Converts a value in milliseconds to microseconds.
     * @tparam T The type of the input value.
     * @param t The input value in milliseconds.
     * @return The input value in microseconds.
     */
    template<class T> constexpr auto MSToUS(T t) noexcept { return t * 1000; }
    /**
     * Converts a value in milliseconds to seconds.
     * @tparam T The type of the input value.
     * @param t The input value in milliseconds.
     * @return The input value in seconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(MSToS, t,
        t * 0.001f,
        t * 0.001);

    // US to
    /**
     * Converts a value in microseconds to nanoseconds.
     * @tparam T The type of the input value.
     * @param t The input value in microseconds.
     * @return The input value in nanoseconds.
     */
    template<class T> constexpr auto USToNS(T t) noexcept { return t * 1000; }
    /**
     * Converts a value in microseconds to milliseconds.
     * @tparam T The type of the input value.
     * @param t The input value in microseconds.
     * @return The input value in milliseconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(USToMS, t,
        t * 0.001f,
        t * 0.001);
    /**
     * Converts a value in microseconds to seconds.
     * @tparam T The type of the input value.
     * @param t The input value in microseconds.
     * @return The input value in seconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(USToS, t,
        t * 0.000001f,
        t * 0.000001);

    // NS to
    /**
     * Converts a value in nanoseconds to microseconds.
     * @tparam T The type of the input value.
     * @param t The input value in nanoseconds.
     * @return The input value in microseconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(NSToUS, t,
        t * 0.001f,
        t * 0.001);
    /**
     * Converts a value in nanoseconds to milliseconds.
     * @tparam T The type of the input value.
     * @param t The input value in nanoseconds.
     * @return The input value in milliseconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(NSToMS, t,
        t * 0.000001f,
        t * 0.000001);
    /**
     * Converts a value in nanoseconds to seconds.
     * @tparam T The type of the input value.
     * @param t The input value in nanoseconds.
     * @return The input value in seconds.
     */
    CONVERT_SWITCHING_FLOAT_DOUBLE_FUNC(NSToS, t,
        t * 0.000000001f,
        t * 0.000000001);

    /**
     * @brief Returns the size of the given argument as a `CastTy` type.
     *
     * This function returns the size of the given argument, which is passed by reference, as a `CastTy` type.
     * The `sizeof` operator returns the size of the argument in bytes, which is then cast to the desired `CastTy` type.
     * The result of the cast is returned.
     *
     * @tparam CastTy The type to cast the size of the argument to.
     * @tparam T The type of the argument.
     * @param arg The argument to get the size of.
     * @return The size of the argument as a `CastTy`.
     */
    template<class CastTy, class T>
    constexpr CastTy SizeOf(const T& arg) {
        return static_cast<CastTy>(sizeof(arg));
    }
}

#endif
