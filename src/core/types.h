#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cglm/cglm.h>

// common types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef size_t usize;

// maximum values
#define U8_MAX UINT8_MAX
#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX
#define U64_MAX UINT64_MAX

#define I8_MAX INT8_MAX
#define I16_MAX INT16_MAX
#define I32_MAX INT32_MAX
#define I64_MAX INT64_MAX

#define F32_MAX FLT_MAX
#define F64_MAX DBL_MAX

#define USIZE_MAX SIZE_MAX

// minimum values
#define U8_MIN 0
#define U16_MIN 0
#define U32_MIN 0
#define U64_MIN 0

#define I8_MIN INT8_MIN
#define I16_MIN INT16_MIN
#define I32_MIN INT32_MIN
#define I64_MIN INT64_MIN

#define F32_MIN -FLT_MAX
#define F64_MIN -DBL_MAX

#define USIZE_MIN 0

// unused variable macro
#define UNUSED(x) (void)(x)

// array count macro
#define ARR_LEN(arr) (usize)(sizeof(arr) / sizeof(arr[0]))

// glm types
typedef vec2 v2;
typedef vec3 v3;
typedef vec4 v4;

typedef mat2 m2;
typedef mat3 m3;
typedef mat4 m4;