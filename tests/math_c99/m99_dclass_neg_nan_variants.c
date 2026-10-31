/* TC
 * id:    m99_dclass_neg_nan_variants
 * area:  math_c99
 * funcs: _dclass _fdclass
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.3.1 fpclassify (every NaN encoding classifies as NaN); MS docs _dclass, _fdclass
 * note:  MS class values: FP_NAN=2, FP_INFINITE=1, FP_ZERO=0, FP_NORMAL=-1, FP_SUBNORMAL=-2 (literals used; mingw's math.h differs). Encodings built via integer union and passed through volatile.
 */
#include "tc.h"
#include <math.h>
short __cdecl _dclass(double);
short __cdecl _fdclass(float);
#define MS_NAN 2
#define MS_INF 1
#define MS_SUB (-2)
static double fromb(unsigned long long b) { volatile union { double d; unsigned long long u; } x; x.u = b; return x.d; }
static float fromf(unsigned int b) { volatile union { float f; unsigned int u; } x; x.u = b; return x.f; }
TC_CASE(snan) { CHECK_EQ_I(_dclass(fromb(0x7ff0000000000001ULL)), MS_NAN); }
TC_CASE(neg_qnan) { CHECK_EQ_I(_dclass(fromb(0xfff8000000000000ULL)), MS_NAN); }
TC_CASE(neg_inf) { CHECK_EQ_I(_dclass(fromb(0xfff0000000000000ULL)), MS_INF); }
TC_CASE(largest_denormal) { CHECK_EQ_I(_dclass(fromb(0x000fffffffffffffULL)), MS_SUB); }
TC_CASE(float_snan) { CHECK_EQ_I(_fdclass(fromf(0x7f800001u)), MS_NAN); CHECK_EQ_I(_fdclass(fromf(0xffc00000u)), MS_NAN); }
int main(void) { tc_begin(); TC_RUN(snan); TC_RUN(neg_qnan); TC_RUN(neg_inf); TC_RUN(largest_denormal); TC_RUN(float_snan); return tc_end(); }
