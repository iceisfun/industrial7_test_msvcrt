/* TC
 * id:    pfm_1_star_prec_negative
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p4 (negative precision taken as if omitted); MS docs printf precision
 * note:  Exponent digits: ucrt 2, older CRTs 3.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#if TC_AT_LEAST(140)
#define X0 ""
#else
#define X0 "0"
#endif

#define TI(fmt, p, v, want) do { char b[64]; volatile int tp = (p); volatile int tv = (v); sprintf(b, fmt, tp, tv); CHECK_STR(b, want); } while (0)
#define TD(fmt, p, v, want) do { char b[64]; volatile int tp = (p); volatile double tv = (v); sprintf(b, fmt, tp, tv); CHECK_STR(b, want); } while (0)

TC_CASE(d) { TI("%.*d", -1, 42, "42"); TI("%.*x", -100, 255, "ff"); TI("%.*d", -1, 0, "0"); }
TC_CASE(s) { char b[64]; volatile int p = -1; sprintf(b, "%.*s", p, "abc"); CHECK_STR(b, "abc"); }
TC_CASE(f) { TD("%.*f", -1, 1.5, "1.500000"); }
TC_CASE(e) { TD("%.*e", -5, 1.0, "1.000000e+0" X0 "0"); }
TC_CASE(g) { TD("%.*g", -2, 0.5, "0.5"); TD("%.*g", -2, 1.0 / 3.0, "0.333333"); }

int main(void) { tc_begin(); TC_RUN(d); TC_RUN(s); TC_RUN(f); TC_RUN(e); TC_RUN(g); return tc_end(); }
