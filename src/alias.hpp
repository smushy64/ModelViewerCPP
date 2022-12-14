/**
 * Description:  Type aliases
 *               Includes cstdint
 * Author:       Alicia Amarilla (smushy)
 * File Created: November 05, 2022
*/

#ifndef SMUSHY_TYPE_ALIAS
#define SMUSHY_TYPE_ALIAS 1

#include <cstdint>

/// @brief Pointer-sized unsigned integer
typedef uintptr_t usize;
/// @brief Pointer-sized signed integer
typedef intptr_t  isize;

/// @brief 8-bit unsigned integer
typedef uint8_t   u8;
/// @brief 16-bit unsigned integer
typedef uint16_t u16;
/// @brief 32-bit unsigned integer
typedef uint32_t u32;
/// @brief 64-bit unsigned integer
typedef uint64_t u64;
/// @brief 32-bit unsigned integer for representing boolean values
typedef uint32_t bool32;

/// @brief 8-bit signed integer
typedef int8_t   i8;
/// @brief 16-bit signed integer
typedef int16_t i16;
/// @brief 32-bit signed integer
typedef int32_t i32;
/// @brief 64-bit signed integer
typedef int64_t i64;

/// @brief 32-bit floating-point number
typedef float  f32;
/// @brief 64-bit floating-point number
typedef double f64;

/// @brief Float 32-bit constants
namespace F32 {
    /// @brief Largest finite f32 value
    inline const constexpr f32 MAX = 3.40282347E+38f;
    /// @brief Smallest finite f32 value
    inline const constexpr f32 MIN = -3.40282347E+38f;
    /// @brief Not a number
    inline const f32 NaN = 0.0f / 0.0f;
    /// @brief Smallest positive f32 value
    inline const constexpr f32 MIN_POS = 1.17549435E-38f;
    /// @brief Positive infinity
    inline const f32 POS_INFINITY = 1.0f / 0.0f;
    /// @brief Positive infinity
    inline const f32 NEG_INFINITY = -1.0f / 0.0f;
    /// @brief Pi constant
    inline const constexpr f32 PI = 3.141592741f;
    /// @brief Tau constant
    inline const constexpr f32 TAU = 2.0f * PI;
    /// @brief Epsilon constant
    inline const constexpr f32 EPSILON = 1.1920929E-7f;
    /// @brief Approximate number of significant digits in base-10
    inline const constexpr u32 SIGNIFICANT_DIGITS = 6;
    /// @brief Number of significant digits in base-2
    inline const constexpr u32 MANTISSA_DIGITS = 24;
};

/// @brief Float 64-bit constants
namespace F64 {
    /// @brief Largest finite f64 value
    inline const constexpr f64 MAX = 1.7976931348623157E+308;
    /// @brief Smallest finite f64 value
    inline const constexpr f64 MIN = -1.7976931348623157E+308;
    /// @brief Not a number
    inline const f64 NaN = 0.0 / 0.0;
    /// @brief Smallest positive f32 value
    inline const constexpr f64 MIN_POS = 2.2250738585072014E-308;
    /// @brief Positive infinity
    inline const f64 POS_INFINITY = 1.0 / 0.0;
    /// @brief Positive infinity
    inline const f64 NEG_INFINITY = -1.0 / 0.0;
    /// @brief Pi constant
    inline const constexpr f64 PI = 3.14159265358979323846;
    /// @brief Tau constant
    inline const constexpr f64 TAU = 2.0 * PI;
    /// @brief Epsilon constant
    inline const constexpr f64 EPSILON = 2.2204460492503131E-16;
    /// @brief Approximate number of significant digits in base-10
    inline const constexpr u32 SIGNIFICANT_DIGITS = 15;
    /// @brief Number of significant digits in base-2
    inline const constexpr u32 MANTISSA_DIGITS = 54;
};

/// @brief Unsigned integer 8-bit constants
namespace U8 {
    /// @brief Largest u8 value
    inline const constexpr u8 MAX = 255;
    /// @brief Smallest u8 value
    inline const constexpr u8 MIN = 0;
};

/// @brief Unsigned integer 16-bit constants
namespace U16 {
    /// @brief Largest u16 value
    inline const constexpr u16 MAX = 65535;
    /// @brief Smallest u16 value
    inline const constexpr u16 MIN = 0;
};

/// @brief Unsigned integer 32-bit constants
namespace U32 {
    /// @brief Largest u32 value
    inline const constexpr u32 MAX = 4294967295;
    /// @brief Smallest u32 value
    inline const constexpr u32 MIN = 0;
};

/// @brief Unsigned integer 64-bit constants
namespace U64 {
    /// @brief Largest u64 value
    inline const constexpr u64 MAX = 18446744073709551615ULL;
    /// @brief Smallest u64 value
    inline const constexpr u64 MIN = 0;
};

/// @brief Integer 8-bit constants
namespace I8 {
    /// @brief Largest i8 value
    inline const constexpr i8 MAX = 127;
    /// @brief Smallest i8 value
    inline const constexpr i8 MIN = -128;
};

/// @brief Integer 16-bit constants
namespace I16 {
    /// @brief Largest i16 value
    inline const constexpr i16 MAX = 32767;
    /// @brief Smallest i16 value
    inline const constexpr i16 MIN = -32768;
};

/// @brief Integer 32-bit constants
namespace I32 {
    /// @brief Largest i32 value
    inline const constexpr i32 MAX = 2147483647;
    /// @brief Smallest i32 value
    inline const constexpr i32 MIN = -2147483648;
};

/// @brief Integer 64-bit constants
namespace I64 {
    /// @brief Largest i64 value
    inline const constexpr i64 MAX = 9223372036854775807;
    /// @brief Smallest i64 value
    inline const constexpr i64 MIN = -9223372036854775807 - 1;
};

#endif