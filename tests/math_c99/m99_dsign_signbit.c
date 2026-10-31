/* TC
 * id:    m99_dsign_signbit
 * area:  math_c99
 * funcs: _dsign _fdsign
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  documented
 * ref:   MS math.h signbit via _dsign/_fdsign; C11 7.12.3.6
 * note:  Only nonzero/zero is asserted (some versions return 0x8000).
 */
#include "tc.h"
#include <math.h>
int __cdecl _dsign(double);
int __cdecl _fdsign(float);
/* some CRT generations return a 16-bit value; only the low 16 bits are meaningful */
#define DS(x) ((short)_dsign(x))
#define FS(x) ((short)_fdsign(x))
static double mkd(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static float mkf(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
TC_CASE(finite) { volatile double n = -1.0, p = 1.0; CHECK(DS(n) != 0); CHECK(DS(p) == 0); }
TC_CASE(zeros) { volatile double n = -0.0, p = 0.0; CHECK(DS(n) != 0); CHECK(DS(p) == 0); }
TC_CASE(inf_nan) { volatile double ni = mkd(0xfff0000000000000ULL), pi = mkd(0x7ff0000000000000ULL), nn = mkd(0xfff8000000000000ULL), pn = mkd(0x7ff8000000000000ULL);
    CHECK(DS(ni) != 0); CHECK(DS(pi) == 0); CHECK(DS(nn) != 0); CHECK(DS(pn) == 0); }
TC_CASE(float_variants) { volatile float n = -1.0f, p = 0.0f, nz = -0.0f, nn = mkf(0xffc00000u), pn = mkf(0x7fc00000u);
    CHECK(FS(n) != 0); CHECK(FS(p) == 0); CHECK(FS(nz) != 0); CHECK(FS(nn) != 0); CHECK(FS(pn) == 0); }
TC_CASE(agrees_with_signbit) { static const unsigned long long bits[] = { 0, 0x8000000000000000ULL, 1, 0x8000000000000001ULL, 0x3ff0000000000000ULL, 0xbff0000000000000ULL, 0x7ff0000000000000ULL, 0xfff0000000000000ULL }; int i;
    for (i = 0; i < 8; i++) { volatile double d = mkd(bits[i]); CHECK_EQ_I(DS(d) != 0, __builtin_signbit(d) != 0); } }
int main(void) { tc_begin(); TC_RUN(finite); TC_RUN(zeros); TC_RUN(inf_nan); TC_RUN(float_variants); TC_RUN(agrees_with_signbit); return tc_end(); }
