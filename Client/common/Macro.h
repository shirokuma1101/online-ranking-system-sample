/**
 * @file Macro.h
 * @author shirokuma1101
 * @version 1.1
 * @date 2023-05-14
 *
 * @copyright Copyright (c) 2023 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#ifndef GAME_LIBRARIES_UTILITY_MACRO_H_
#define GAME_LIBRARIES_UTILITY_MACRO_H_

/**
 * @brief Macro to check version of C++.
 */
#define CPP_VERSION __cplusplus

/**
 * @brief Macro to check if the compiler is C++98.
 */
#define MACRO_CPP98 199711L
/**
 * @brief Macro to check if the compiler is C++11.
 */
#define MACRO_CPP11 201103L
/**
 * @brief Macro to check if the compiler is C++14.
 */
#define MACRO_CPP14 201402L
/**
 * @brief Macro to check if the compiler is C++17.
 */
#define MACRO_CPP17 201703L
/**
 * @brief Macro to check if the compiler is C++20.
 */
#define MACRO_CPP20 202002L
/**
 * @brief Macro to check if the compiler is C++23.
 */
#define MACRO_CPP23 202399L // 2023XXL

/**
 * @brief Macro to check if a pointer is valid, assigning the result of a function to it.
 * @param p The pointer to check and assign.
 * @param func The function to check the pointer against.
 * @return A boolean indicating if the pointer is invalid after assignment.
 */
#define MACRO_VALID_POINTER(p, func)   p = func; p
/**
 * @brief Macro to check if a pointer is invalid, assigning the result of a function to it.
 * @param p The pointer to check and assign.
 * @param func The function to check the pointer against.
 * @return A boolean indicating if the pointer is invalid after assignment.
 */
#define MACRO_INVALID_POINTER(p, func) p = func; !p

/**
 * @brief Macro to check if a function returns 0, assigning the result of the function to an error code.
 * @param func The function to check.
 * @param err_code The error code to assign the result of the function to.
 * @return A boolean indicating if the function returned 0.
 */
#define MACRO_SUCCESS_CHECK(func, err_code) int err_code = func; !err_code
/**
 * @brief Macro to check if a function returns 0, assigning the result of the function to an error code.
 * @param func The function to check.
 * @param err_code The error code to assign the result of the function to.
 * @return A boolean indicating if the function returned 0.
 */
#define MACRO_FAIL_CHECK(func, err_code)    int err_code = func; err_code

/**
 * @brief Macro to begin an external namespace.
 */
#define MACRO_NAMESPACE_EXTERNAL_BEGIN namespace detail {} namespace {
/**
 * @brief Macro to end an external namespace.
 */
#define MACRO_NAMESPACE_EXTERNAL_END   }
/**
 * @brief Macro to begin an internal namespace.
 */
#define MACRO_NAMESPACE_INTERNAL_BEGIN namespace detail {
/**
 * @brief Macro to end an internal namespace.
 */
#define MACRO_NAMESPACE_INTERNAL_END   }


#endif
