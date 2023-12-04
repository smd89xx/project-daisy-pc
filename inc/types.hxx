#pragma once

namespace types
{
    typedef unsigned char u8;
    typedef signed char s8;

    typedef unsigned short u16;
    typedef signed short s16;

    typedef unsigned long u32;
    typedef signed long s32;

    typedef unsigned long long u64;
    typedef signed long long s64;

    struct varSizes
    {    enum sizes
        {
            var8Bit = 1,
            var16Bit = 2,
            var32Bit = 4,
            var64Bit = 8,
        };
    };
}