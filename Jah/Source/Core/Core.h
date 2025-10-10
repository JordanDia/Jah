#pragma once

#include <iostream>
#include <format>
#include <cstdlib>

#ifdef JAH_DEBUG
    #define JAH_ENABLE_ASSERTS
#endif

#ifdef JAH_ENABLE_ASSERTS
    #if defined(_MSC_VER)
        #define JAH_DEBUGBREAK() __debugbreak()
    #else
        #include <signal.h>
        #define JAH_DEBUGBREAK() raise(SIGTRAP)
    #endif

    #define JAH_ASSERT(x, ...) { \
                if(!(x)) { \
                    std::cerr << "[ASSERTION FAILED] " << std::format(__VA_ARGS__) << '\n'; \
                    JAH_DEBUGBREAK(); \
                } \
            }

    #else
    #define JAH_ASSERT(x, ...)
#endif
