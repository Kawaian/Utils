// Copyright(c) 2017 Kawaian. All rights reserved.
// Info: Functions to assertion functionalities
// accross different platforms.
#pragma once

#ifdef _MSC_VER
#include <intrin.h>
#define XASSERT_DEBUG_BREAK __debugbreak();

#elif defined(_GNUC_)
#include <signal.h>
#define XASSERT_DEBUG_BREAK raise(SIGINT);
// #define XASSERT_DEBUG_BREAK __asm__ __volatile__("int 0x03");

#else 
#define XASSERT_DEBUG_BREAK
#endif

#ifndef XASSERT_DISABLE

#define XASSERT_AUX_STRINGIZE_HELPER(x) #x
#define XASSERT_AUX_STRINGIZE(x) XASSERT_AUX_STRINGIZE_HELPER(x) 

#define XASSERT(condition, message)                         \
    if (!condition) {                                       \
        ::xassert::Assert(                                  \
            message,                                        \
            XASSERT_AUX_STRINGIZE(condition),               \
            __FILE__,                                       \
            XASSERT_AUX_STRINGIZE(__LINE__));               \
                                                            \
        if (::xassert::detail::g_assertDebugBreak)          \
            XASSERT_DEBUG_BREAK;                            \
    }

#else
#define XASSERT(condition, message) do { (condition); } while(0);

#endif

namespace xassert {
    enum class Option
    {
        Continue = 0x00,
        Stop,
        Break,
    };

    //! @brief Performs an assertion. This is platform specific!
    //! @param[in]  condition_str    Condition the assert was triggered on.
    //! @param[in]  msg              Message to assert on.
    //! @param[out] filename         Name of the sourcefile where the assertion occured.
    //! @param[out] line             Line in the sourcefile where the assertion occured.
    //!
    //! @note API users should use the XASSERT_ASSERT macro to call assert.
    void Assert(const char* msg, const char* condition_str = "", const char *filename = "", const char *line = "");
    
    namespace detail {
        extern thread_local char g_assertDebugBreak;

    } // namespace detail
} // namespace xassert