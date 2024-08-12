
#ifndef STD_TYPES_H
#define STD_TYPES_H

#include <stdlib.h>

typedef unsigned char       uint8_t;
typedef signed   char       int8_t;

typedef unsigned short      uint16_t;
typedef signed   short      int16_t;

typedef unsigned long       uint32_t;
typedef signed   long       int32_t;

typedef unsigned long long  uint64_t;
typedef signed   long long  int64_t;

typedef float               f32;
typedef double              f64;
typedef long double         f80;

typedef void (*ptr2fun_Iv_Ov)(void);

// #define NULL	((volatile void*) 0LL)

#endif
