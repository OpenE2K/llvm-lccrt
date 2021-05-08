/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#ifndef _MATH_H
# error "Never use <bits/mathinline.h> directly; include <math.h> instead."
#endif

# ifndef __extern_inline
#  define __MATH_INLINE __inline
# else
#  define __MATH_INLINE __extern_inline
# endif  /* __cplusplus */

#ifdef __GNUC__

# define __inline_mathcode(float_type, func, code) \
__MATH_INLINE float_type                           \
__NTH (func (float_type __x))                      \
{                                                  \
  code;                                            \
}

# define __inline_mathcode2(float_type, func, code)\
__MATH_INLINE float_type                           \
__NTH (func (float_type __x, float_type __y))      \
{                                                  \
  code;                                            \
}

# define __inline_sqrtf(__xx) \
_Pragma ("asm_inline")                                        \
  __asm ("fsqrts %0,%0" : "+r" (__xx));                       \
  return __xx

# define __inline_sqrt(__xx) \
  double __tmp;                                               \
_Pragma ("asm_inline")                                        \
  __asm ("fsqrtid %1, %0" : "=r" (__tmp) : "r" (__xx));       \
_Pragma ("asm_inline")                                        \
  __asm ("fsqrttd %0, %1, %0" : "+r" (__xx) : "r" (__tmp));   \
  return __xx

# define __inline_sqrtl(__xx) \
  long double __tmp;                                          \
_Pragma ("asm_inline")                                        \
  __asm ("fxsqrtixx %1, %0" : "=r" (__tmp) : "r" (__xx));     \
_Pragma ("asm_inline")                                        \
  __asm ("fxsqrtuxx %1, %0, %0" : "+r" (__tmp) : "r" (__xx)); \
_Pragma ("asm_inline")                                        \
  __asm ("fxsqrttxx %0, %1, %0" : "+r" (__xx) : "r" (__tmp)); \
  return __xx

#if defined __USE_ISOC99 && (!__GNUC_PREREQ (3, 0) || defined __LCC__)

#undef isgreater
#undef isgreaterequal
#undef isless
#undef islessequal
#undef islessgreater
#undef isunordered

#define __compare(x, y) \
    ({  unsigned __r;                                                 \
        if (sizeof (x) == 4 && sizeof (y) == 4) {                     \
            float __x = (x), __y = (y);                               \
_Pragma ("asm_inline")                                                \
            asm ("fcmpudsf %[src1_name], %[src2_name], %[dst_name]"   \
                : [dst_name] "=r" (__r)                               \
                : [src1_name] "r" (__x), [src2_name] "ri" (__y));     \
        }                                                             \
        else if (sizeof (x) <= 8 && sizeof (y) <= 8) {                \
            double __x = (x), __y = (y);                              \
_Pragma ("asm_inline")                                                \
            asm ("fcmpuddf %[src1_name], %[src2_name], %[dst_name]"   \
                : [dst_name] "=r" (__r)                               \
                : [src1_name] "r" (__x), [src2_name] "ri" (__y));     \
        }                                                             \
        else {                                                        \
            long double __x = (x), __y = (y);                         \
_Pragma ("asm_inline")                                                \
            asm ("fxcmpudxf %[src1_name], %[src2_name], %[dst_name]"  \
                : [dst_name] "=r" (__r)                               \
                : [src1_name] "r" (__x), [src2_name] "r" (__y));      \
        }                                                             \
        __r; })

#define isgreater(x, y) (__compare (x, y) == 0)
#define isgreaterequal(x, y) ({unsigned __rr = __compare (x, y); __rr == 0 || __rr == 64;})
#define isless(x, y) (__compare (x, y) == 1)
#define islessequal(x, y) ({unsigned __rr = __compare (x, y); __rr == 1 || __rr == 64;})
#define islessgreater(x, y) __builtin_islessgreater (x, y)
#define isunordered(x, y) __builtin_isunordered (x, y)

/* Test for negative number.  Used in the signbit() macro.  */
__MATH_INLINE int
__NTH (__signbitf (float __x))
{
  __extension__ union { float __f; unsigned int __i; } __u = { __f: __x };
  return __u.__i >> 31;
}

__MATH_INLINE int
__NTH (__signbit (double __x))
{
  __extension__ union { double __f; unsigned long long __i; } __u = { __f: __x };
  return __u.__i >> 63;
}

__MATH_INLINE int
__NTH (__signbitl (long double __x))
{
  __extension__ union { long double __f; struct { long long __ll; short __si; } twolong; } __u = { __f: __x };
  return __u.twolong.__si & 0x8000;
}

#endif /* __USE_ISOC99 */


#if (!defined __NO_MATH_INLINES || defined __LIBC_INTERNAL_MATH_INLINES) && defined __OPTIMIZE__

#  ifdef __USE_ISOC99

/* Test for Inf.  Used in the isinf() macro.  */
__MATH_INLINE int
__NTH (__isinff (float __x))
{
  __extension__ union { float __f; int __i; } __u = { __f: __x };
  return (__u.__i & 0x7fffffff) == 0x7f800000 ? (__u.__i >> 30) : 0;
}

__MATH_INLINE int
__NTH (__isinf (double __x))
{
  __extension__ union { double __f; long long __i; } __u = { __f: __x };
  return (__u.__i & 0x7fffffffffffffffLL) == 0x7ff0000000000000LL ?
      (__u.__i >> 62) : 0;
}

__MATH_INLINE int
__NTH (__isinfl (long double __x))
{
  __extension__ union { long double __f; struct { long long __ll; short __si; } twolong; } __u = { __f: __x };
  return ((__u.twolong.__si & 0x7fff) == 0x7fff) && (__u.twolong.__ll == 0x8000000000000000LL) ?
      (__u.twolong.__si >> 14) : 0;
}


/* Test for NaN.  Used in the isnan() macro.  */
__MATH_INLINE int
__NTH (__isnanf (float __xx))
{
  return isunordered (__xx, __xx);
}

__MATH_INLINE int
__NTH (__isnan (double __xx))
{
  return isunordered (__xx, __xx);
}

__MATH_INLINE int
__NTH (__isnanl (long double __xx))
{
  return isunordered (__xx, __xx);
}


/* This function is used in the `isfinite' macro.  */
__MATH_INLINE int
__NTH (__finitef (float __x))
{
  __extension__ union { float __f; int __i; } __xx = {__f: __x};

  return ((__xx.__i & 0x7fffffff) < 0x7f800000);
}

__MATH_INLINE int
__NTH (__finite (double __x))
{
  __extension__ union { double __f; long long __i; } __xx = {__f: __x};

  return ((__xx.__i & 0x7fffffffffffffffLL) < 0x7ff0000000000000LL);
}

__MATH_INLINE int
__NTH (__finitel (long double __x))
{
  __extension__ union { long double __f; struct { long long __ll; short __si; } twolong; } __xx = {__f: __x};

  return ((__xx.twolong.__si & 0x7fff) < 0x7fff);
}


/* This function is used in the `fpclassify' macro.  */
__MATH_INLINE int
__NTH (__fpclassifyf (float __x))
{
  __extension__ union { float __f; int __i; } __xx = { __f: __x };
  int __ex, __m, __retval = FP_NORMAL;

  __ex = (__xx.__i >> 23) & 0xff;
  __m = __xx.__i & 0x7fffff;
  if (__ex == 0)
    __retval = __m == 0 ? FP_ZERO : FP_SUBNORMAL;
  else if (__ex == 0xff)
    __retval = __m == 0 ? FP_INFINITE : FP_NAN;
  return __retval;
}

__MATH_INLINE int
__NTH (__fpclassify (double __x))
{
  __extension__ union { double __f; long long __i; } __xx = { __f: __x };
  int __ex, __retval = FP_NORMAL;
  long long __m;

  __ex = (__xx.__i >> 52) & 0x7ff;
  __m = __xx.__i & 0xfffffffffffffLL;
  if (__ex == 0)
    __retval = __m == 0 ? FP_ZERO : FP_SUBNORMAL;
  else if (__ex == 0x7ff)
    __retval = __m == 0 ? FP_INFINITE : FP_NAN;
  return __retval;
}

__MATH_INLINE int
__NTH (__fpclassifyl (long double __x))
{
  __extension__ union { long double __f; struct { long long __ll; short __si; } twolong; } __xx = { __f: __x };
  int __retval = FP_NORMAL;
  int __ex = __xx.twolong.__si & 0x7fff;
  long long __m = __xx.twolong.__ll & ~0x8000000000000000LL;

  if (__ex == 0) {
    if (__xx.twolong.__ll == 0)
      __retval = FP_ZERO;
    else if (__xx.twolong.__ll == __m)
      __retval = FP_SUBNORMAL;
  }
  else if (__ex == 0x7fff)
    __retval = __m == 0 ? FP_INFINITE : FP_NAN;
  return __retval;
}

#  endif /* __USE_ISOC99 */


#  ifndef __NO_MATH_INLINES
__inline_mathcode (long double, fabsl, \
_Pragma ("asm_inline")                 \
  __asm ("movxa %0,%0" : "+r" (__x));  \
  return __x;)

#   if defined __FAST_MATH__
__inline_mathcode (float, sqrtf, __inline_sqrtf(__x))
__inline_mathcode (double, sqrt, __inline_sqrt(__x))
__inline_mathcode (long double, sqrtl, __inline_sqrtl(__x))

#    if __iset__ >= 4

__MATH_INLINE double
__NTH (scalbn (double __x, int __n))
{
#pragma asm_inline
  __asm ("fscaled %0,%1,%0" : "+r" (__x) : "r" (__n));
  return __x;
}

__MATH_INLINE double
__NTH (ldexp (double __x, int __exp))
{
#pragma asm_inline
  __asm ("fscaled %0,%1,%0" : "+r" (__x) : "r" (__exp));
  return __x;
}

#    endif /* __iset__ >= 4 */
#   endif /* !__FAST_MATH__ */

#  endif /* !__NO_MATH_INLINES */

/* This code is used internally in the GNU libc.  */
#  ifdef __LIBC_INTERNAL_MATH_INLINES
__inline_mathcode (float, __ieee754_sqrtf, __inline_sqrtf(__x))
__inline_mathcode (double, __ieee754_sqrt, __inline_sqrt(__x))
__inline_mathcode (long double, __ieee754_sqrtl, __inline_sqrtl(__x))

#    if __iset__ < 4

__MATH_INLINE double
__NTH (__scalbn (double __x, int __n))
{
  __extension__ union { double __f; long long __i; } __xx = { __f: __x }, __tmp;
  long long __k;

  __k = (__xx.__i >> 52) & 0x7ff;                     /* extract exponent */
  if (__k == 0) {                  /* 0 or subnormal x */
    if (__x == 0)
      return __x;                                  /* +-0 */
    __xx.__f *= 4.503599627370496e15; /* 2 ^ 52 */
    __k = ((__xx.__i >> 52) & 0x7ff) - 52;
  }
  if (__k == 0x7ff)
    return __x + __x;                                       /* NaN or Inf */
  __xx.__i &= ~0x7ff0000000000000LL; /* обнуляем порядок */
  __k += __n;
  if (__k <= -54) {
    __tmp.__f = 2.2250738585072014e-308; /* DB_MIN */
    __xx.__i |= __tmp.__i;
    return 2.2250738585072014e-308 * __xx.__f;           /* underflow */
  }
  if (__k > 0x7fe) {
    __tmp.__f = 1.7976931348623157e+308; /* DB_MAX */
    __xx.__i |= __tmp.__i;
    return 1.7976931348623157e+308 * __xx.__f;           /* overflow */
  }
  if (__k > 0) {                 /* normal result */
    __xx.__i |= (__k << 52);
    return __xx.__f;
  }
  __k += 54;                                      /* subnormal result */
  __xx.__i |= (__k << 52);
  return __xx.__f * 5.551115123125782702e-17; /* 2 ^ -54 */
}

__MATH_INLINE float
__NTH (__scalbnf (float __x, int __n))
{
  __extension__ union { float __f; int __i; } __xx = { __f: __x }, __tmp;
  int __k;

  __k = (__xx.__i >> 23) & 0xff;                     /* extract exponent */
  if (__k == 0) {                  /* 0 or subnormal x */
    if (__x == 0)
      return __x;                                  /* +-0 */
    __xx.__f *= 8388608.0F; /* 2 ^ 23 */
    __k = ((__xx.__i >> 23) & 0xff) - 23;
  }
  if (__k == 0xff)
    return __x + __x;                                       /* NaN or Inf */
  __xx.__i &= ~0x7f800000;                          /* обнуляем порядок */
  if ((__k + (long long) __n) <= -25) {
    __tmp.__f = 1.17549435e-38F; /* FL_MIN */
    __xx.__i |= __tmp.__i;
    return 1.17549435e-38F * __xx.__f;           /*underflow*/
  }
  if ((__k + (long long) __n) > 0xfe) {
    __tmp.__f = 3.402823466E+38F; /* FL_MAX */
    __xx.__i |= __tmp.__i;
    return 3.402823466E+38F * __xx.__f;           /* overflow */
  }
  __k += __n;
  if (__k > 0) {                   /* normal result */
    __xx.__i |= (__k << 23);
    return __xx.__f;
  }
  __k += 25;                                      /* subnormal result */
  __xx.__i |= (__k << 23);
  return __xx.__f * 2.9802322388e-08F; /* 2 ^ -25 */
}

#    else /* __iset__ < 4 */

__MATH_INLINE double
__NTH (__scalbn (double __x, int __n))
{
#pragma asm_inline
  __asm ("fscaled %0,%1,%0" : "+r" (__x) : "r" (__n));
  return __x;
}

__MATH_INLINE float
__NTH (__scalbnf (float __x, int __n))
{
#pragma asm_inline
  __asm ("fscales %0,%1,%0" : "+r" (__x) : "r" (__n));
  return __x;
}

#    endif /* __iset__ < 4 */

#  endif /* __LIBC_INTERNAL_MATH_INLINES */

# ifdef __USE_ISOC99

#  ifndef __NO_MATH_INLINES

__MATH_INLINE long int
__NTH (lrint (double __x))
{
  long int __res;
#ifdef __ptr32__
#pragma asm_inline
    __asm ("fdtois %1,%0" : "=r" (__res) : "r" (__x));
#else /* __ptr32__ */
#pragma asm_inline
    __asm ("fdtoid %1,%0" : "=r" (__res) : "r" (__x));
#endif /* __ptr32__ */
  return __res;
}

__MATH_INLINE long long int
__NTH (llrint (double __x))
{
  long long int __res;
#pragma asm_inline
  __asm ("fdtoid %1,%0" : "=r" (__res) : "r" (__x));
  return __res;
}

__MATH_INLINE long int
__NTH (lrintf (float __x))
{
  long int __res;
#ifdef __ptr32__
#pragma asm_inline
    __asm ("fstois %1,%0" : "=r" (__res) : "r" (__x));
#else /* __ptr32__ */
#pragma asm_inline
    __asm ("fstoid %1,%0" : "=r" (__res) : "r" (__x));
#endif /* __ptr32__ */
  return __res;
}

__MATH_INLINE long long int
__NTH (llrintf (float __x))
{
  long long int __res;
#pragma asm_inline
  __asm ("fstoid %1,%0" : "=r" (__res) : "r" (__x));
  return __res;
}

__MATH_INLINE long int
__NTH (lround (double __x))
{
  __extension__ union { double __f; long long __i; } __xx = { __f: __x }, __con;

  __con.__f = 0x1.fffffffffffffp-2; /* почти 0.5 */
  __con.__i |= __xx.__i & 0x8000000000000000LL;
  return (long int) (__xx.__f + __con.__f);
}

__MATH_INLINE long long int
__NTH (llround (double __x))
{
  __extension__ union { double __f; long long __i; } __xx = { __f: __x }, __con;

  __con.__f = 0x1.fffffffffffffp-2; /* почти 0.5 */
  __con.__i |= __xx.__i & 0x8000000000000000LL;
  return (long long int) (__xx.__f + __con.__f);
}

__MATH_INLINE long int
__NTH (lroundf (float __x))
{
  __extension__ union { float __f; int __i; } __xx = { __f: __x }, __con;

  __con.__f = 0x1.fffffep-2f; /* почти 0.5 */
  __con.__i |= __xx.__i & 0x80000000;
  return (long int) (__xx.__f + __con.__f);
}

__MATH_INLINE long long int
__NTH (llroundf (float __x))
{
  __extension__ union { float __f; int __i; } __xx = { __f: __x }, __con;

  __con.__f = 0x1.fffffep-2f; /* почти 0.5 */
  __con.__i |= __xx.__i & 0x80000000;
  return (long long int) (__xx.__f + __con.__f);
}

# if __iset__ >= 4

__MATH_INLINE float
__NTH (scalbnf (float __x, int __n))
{
#pragma asm_inline
  __asm ("fscales %0,%1,%0" : "+r" (__x) : "r" (__n));
  return __x;
}

__MATH_INLINE float
__NTH (scalblnf (float __x, long int __n))
{
#ifdef __ptr64__
  if (__n > 500) __n = 500;
  if (__n < -500) __n = -500;
#endif /* __ptr64__ */
#pragma asm_inline
  __asm ("fscales %0,%1,%0" : "+r" (__x) : "r" (__n));
  return __x;
}

__MATH_INLINE double
__NTH (scalbln (double __x, long int __n))
{
#ifdef __ptr64__
  if (__n > 5000) __n = 5000;
  if (__n < -5000) __n = -5000;
#endif /* __ptr64__ */
#pragma asm_inline
  __asm ("fscaled %0,%1,%0" : "+r" (__x) : "r" (__n));
  return __x;
}

# endif /* __iset__ >= 4 */

#   if defined __FINITE_MATH_ONLY__ && __FINITE_MATH_ONLY__ > 0

/* Determine maximum of two values.  */
__MATH_INLINE float
__NTH (fmaxf (float __x, float __y))
{
#pragma asm_inline
  __asm ("fmaxs %0,%1,%0" : "+r" (__x) : "r" (__y));
  return __x;
}
__MATH_INLINE double
__NTH (fmax (double __x, double __y))
{
#pragma asm_inline
  __asm ("fmaxd %0,%1,%0" : "+r" (__x) : "r" (__y));
  return __x;
}

/* Determine minimum of two values.  */
__MATH_INLINE float
__NTH (fminf (float __x, float __y))
{
#pragma asm_inline
  __asm ("fmins %0,%1,%0" : "+r" (__x) : "r" (__y));
  return __x;
}
__MATH_INLINE double
__NTH (fmin (double __x, double __y))
{
#pragma asm_inline
  __asm ("fmind %0,%1,%0" : "+r" (__x) : "r" (__y));
  return __x;
}

__MATH_INLINE double
__NTH (fdim (double __x, double __y))
{
  return __x <= __y ? 0 : __x - __y;
}

__MATH_INLINE float
__NTH (fdimf (float __x, float __y))
{
  return __x <= __y ? 0 : __x - __y;
}

#   endif /* defined __FINITE_MATH_ONLY__ && __FINITE_MATH_ONLY__ > 0 */
#  endif /* !__NO_MATH_INLINES */
# endif /* __USE_ISOC99 */


#if !__GNUC_PREREQ (4, 0)
__inline_mathcode2 (float, __copysignf, return __builtin_copysignf (__x, __y))
__inline_mathcode2 (double, __copysign, return __builtin_copysign (__x, __y))
__inline_mathcode2 (long double, __copysignl, return __builtin_copysignl (__x, __y))
#endif
__inline_mathcode2 (float, copysignf, return __builtin_copysignf (__x, __y))
__inline_mathcode2 (double, copysign, return __builtin_copysign (__x, __y))
__inline_mathcode2 (long double, copysignl, return __builtin_copysignl (__x, __y))


/* Miscellaneous functions */

#if __iset__ <= 2

__MATH_INLINE double
__NTH (floor (double __x))
{
  double __xx = __x, __absx;

  __absx = __builtin_fabs (__x);
  if (!(__absx < 4.503599627370496e15 /* 2 ^ 52 */))
    return __x;
#pragma asm_inline
  __asm ("fdtoid %0,%0\n\tidtofd %0,%0" : "+r" (__xx));
  if (__xx > __x)
    __xx -= 1.0;
  return __xx;
}

__MATH_INLINE double
__NTH (ceil (double __x))
{
  double __xx = __x, __absx;

  __absx = __builtin_fabs (__x);
  if (!(__absx < 4.503599627370496e15 /* 2 ^ 52 */))
    return __x;
#pragma asm_inline
  __asm ("fdtoid %0,%0\n\tidtofd %0,%0" : "+r" (__xx));
  if (__xx < __x)
    __xx += 1.0;
  return __xx;
}

__MATH_INLINE double
__NTH (rint (double __x))
{
  double __absx = __builtin_fabs (__x);

  if (!(__absx < 4.503599627370496e15 /* 2 ^ 52 */))
    return __x;
#pragma asm_inline
  __asm ("fdtoid %0,%0\n\tidtofd %0,%0" : "+r" (__x));
  return __x;
}

__MATH_INLINE double
__NTH (trunc (double __x))
{
  double __absx = __builtin_fabs (__x);

  if (!(__absx < 4.503599627370496e15 /* 2 ^ 52 */))
    return __x;

#pragma asm_inline
  __asm ("fdtoidtr %0,%0\n\tidtofd %0,%0" : "+r" (__x));
  return __x;
}

__MATH_INLINE double
__NTH (round (double __x))
{
  double __absx = __builtin_fabs (__x);
  __extension__ union { double __f; long long __i; } __xx = { __f: __x };
  long long sign = __xx.__i & 0x8000000000000000LL;

  if (!(__absx < 4.503599627370496e15 /* 2 ^ 52 */))
    return __x;
  __absx += 0x1.fffffffffffffp-2; /* |x| + почти 0.5 */

  __xx.__f = (double) (long long) __absx;

  __xx.__i |= sign;
  return __xx.__f;
}

#else /*  __iset__ <= 2 */

__inline_mathcode (double, floor,           \
_Pragma ("asm_inline")                      \
  __asm ("fdtoifd 0x1,%0,%0" : "+r" (__x)); \
  return __x;)

__inline_mathcode (double, ceil,            \
_Pragma ("asm_inline")                      \
  __asm ("fdtoifd 0x2,%0,%0" : "+r" (__x)); \
  return __x;)

__inline_mathcode (double, rint,            \
_Pragma ("asm_inline")                      \
  __asm ("fdtoifd 0x4,%0,%0" : "+r" (__x)); \
  return __x;)

__inline_mathcode (double, trunc,           \
_Pragma ("asm_inline")                      \
  __asm ("fdtoifd 0x3,%0,%0" : "+r" (__x)); \
  return __x;)

__MATH_INLINE double
__NTH (round (double __x))
{
  double __absx = __builtin_fabs (__x);
  __extension__ union { double __f; long long __i; } __xx = { __f: __x };
  long long sign = __xx.__i & 0x8000000000000000LL;

  __absx += 0x1.fffffffffffffp-2; /* |x| + почти 0.5 */
#pragma asm_inline
  __asm ("fdtoifd 0x3,%0,%0" : "+r" (__absx)); /* отбрасывание дробной части */
  __xx.__f = __absx;
  __xx.__i |= sign;
  return __xx.__f;
}

__inline_mathcode (double, nearbyint,       \
_Pragma ("asm_inline")                      \
  __asm ("fdtoifd 0xc,%0,%0" : "+r" (__x)); \
  return __x;)

#endif /*  __iset__ <= 2 */


#if defined __USE_ISOC99

# if __iset__ <= 2

__MATH_INLINE float
__NTH (floorf (float __x))
{
  float __xx = __x, __absx;

  __absx = __builtin_fabsf (__x);
  if (!(__absx < 8388608.0F /* 2 ^ 23 */))
    return __x;
  __xx = (float) (int) __xx;
  if (__xx > __x)
    __xx -= 1.0f;
  return __xx;
}

__MATH_INLINE float
__NTH (ceilf (float __x))
{
  float __xx = __x, __absx;

  __absx = __builtin_fabsf (__x);
  if (!(__absx < 8388608.0F /* 2 ^ 23 */))
    return __x;
  __xx = (float) (int) __xx;
  if (__xx < __x)
    __xx += 1.0f;
  return __xx;
}

__MATH_INLINE float
__NTH (rintf (float __x))
{
  float __absx = __builtin_fabsf (__x);

  if (!(__absx < 8388608.0F /* 2 ^ 23 */))
    return __x;
#pragma asm_inline
  __asm ("fstois %0,%0\n\tistofs %0,%0" : "+r" (__x));
  return __x;
}

__MATH_INLINE float
__NTH (truncf (float __x))
{
  float __absx = __builtin_fabsf (__x);

  if (!(__absx < 8388608.0F /* 2 ^ 23 */))
    return __x;
  return (float) (int) __x;
}

__MATH_INLINE float
__NTH (roundf (float __x))
{
  float __absx = __builtin_fabsf (__x);
  __extension__ union { float __f; int __i; } __xx = { __f: __x };
  int sign = __xx.__i & 0x80000000;

  if (!(__absx < 8388608.0F /* 2 ^ 23 */))
    return __x;
  __xx.__f = (float) (int) (__absx + 0x1.fffffep-2f); /* |x| + почти 0.5 */
  __xx.__i |= sign;
  return __xx.__f;
}

# else /* __iset__ <= 2 */

__inline_mathcode (float, floorf,           \
_Pragma ("asm_inline")                      \
  __asm ("fstoifs 0x1,%0,%0" : "+r" (__x)); \
  return __x;)

__inline_mathcode (float, ceilf,            \
_Pragma ("asm_inline")                      \
  __asm ("fstoifs 0x2,%0,%0" : "+r" (__x)); \
  return __x;)

__inline_mathcode (float, rintf,            \
_Pragma ("asm_inline")                      \
  __asm ("fstoifs 0x4,%0,%0" : "+r" (__x)); \
  return __x;)

__inline_mathcode (float, truncf,           \
_Pragma ("asm_inline")                      \
  __asm ("fstoifs 0x3,%0,%0" : "+r" (__x)); \
  return __x;)

__MATH_INLINE float
__NTH (roundf (float __x))
{
  float __absx = __builtin_fabsf (__x);
  __extension__ union { float __f; int __i; } __xx = { __f: __x };
  int sign = __xx.__i & 0x80000000;

  __absx += 0x1.fffffep-2f;
#pragma asm_inline
  __asm ("fstoifs 0x3,%0,%0" : "+r" (__absx)); /* отбрасывание дробной части */
  __xx.__f = __absx;
  __xx.__i |= sign;
  return __xx.__f;
}

__inline_mathcode (float, nearbyintf,       \
_Pragma ("asm_inline")                      \
  __asm ("fstoifs 0xc,%0,%0" : "+r" (__x)); \
  return __x;)

# endif /* __iset__ <= 2 */

#endif /* __USE_ISOC99 */


/* __FAST_MATH__ is defined by gcc -ffast-math.  */
#if defined __USE_ISOC99 && defined __FAST_MATH__

/* The argument range of the inline version of hypotl is slightly reduced.  */
__MATH_INLINE float
__NTH (hypotf (float __x, float __y))
{
  double __xx = __x * (double)__x + __y * (double)__y;
  __inline_sqrt (__xx);
}

__MATH_INLINE double
__NTH (hypot (double __x, double __y))
{
  long double __xx = __x * (long double)__x + __y * (long double)__y;
  __inline_sqrtl (__xx);
}

__MATH_INLINE long double
__NTH (hypotl (long double __x, long double __y))
{
  long double __xx = __x * __x + __y * __y;
  __inline_sqrtl (__xx);
}

__MATH_INLINE float
__NTH (logbf (float __x))
{
  __extension__ union { float __f; int __i; } __xx = { __f: __x };
  int __ex = (__xx.__i >> 23) & 0xff;

  if (__x == 0)
    return -__builtin_inff(); /* -inf */
  if (__ex == 0xff)
    return __x * __x; /* +inf or nan */
  if (__ex == 0)
    __ex = 1;
  return (float) (__ex - 127);
}

__MATH_INLINE double
__NTH (logb (double __x))
{
  __extension__ union { double __f; long long __i; } __xx = { __f: __x };
  int __ex = (__xx.__i >> 52) & 0x7ff;

  if (__x == 0)
    return -__builtin_inf(); /* -inf */
  if (__ex == 0x7ff)
    return __x * __x; /* +inf or nan */
  if (__ex == 0)
    __ex = 1;
  return (double) (__ex - 1023);
}


# define __inline_fma(NAME, TYPE)                                       \
__MATH_INLINE TYPE                                                      \
__NTH (NAME (TYPE __x, TYPE __y, TYPE __z))                             \
{                                                                       \
  return (__x * __y) + __z;                                             \
}

__inline_fma (fmaf, float)
__inline_fma (fma, double)
__inline_fma (fmal, long double)

# undef __inline_fma

# if __iset__ >= 4

__MATH_INLINE float
__NTH (ldexpf (float __x, int __exp))
{
#pragma asm_inline
  __asm ("fscales %0,%1,%0" : "+r" (__x) : "r" (__exp));
  return __x;
}

# endif /* __iset__ >= 4 */

#endif /* __USE_ISOC99 && __FAST_MATH__ */

#endif /* !__NO_MATH_INLINES && __OPTIMIZE__ */

#undef __inline_sqrtl
#undef __inline_sqrt
#undef __inline_sqrtf
#undef __inline_mathcode2
#undef __inline_mathcode

#endif /* __GNUC__ */
