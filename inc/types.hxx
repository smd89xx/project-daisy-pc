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

    typedef char* legacyString;

    struct varSizes
    {    enum sizes
        {
            var8Bit,
            var16Bit,
            var32Bit,
            var64Bit,
        };
    };
}