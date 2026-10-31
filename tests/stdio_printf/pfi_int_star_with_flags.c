/* TC
 * id:    pfi_int_star_with_flags
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf size prefixes (I64, ll)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(plus) { char b[80]; volatile int w = 6; sprintf(b, "[%+*d]", w, 42); CHECK_STR(b, "[   +42]"); }
TC_CASE(minus_prec) { char b[80]; volatile int w = 6, p = 3; sprintf(b, "[%-*.*d]", w, p, 7); CHECK_STR(b, "[007   ]"); }
TC_CASE(hash_x) { char b[80]; volatile int w = 6; sprintf(b, "[%#*x]", w, 255); CHECK_STR(b, "[  0xff]"); }
TC_CASE(i64) {
    char b[80]; volatile int w = 6; volatile long long v = -12;
    sprintf(b, "[%*I64d]", w, v); CHECK_STR(b, "[   -12]");
}
TC_CASE(ll) {
#if TC_AT_LEAST(80)
    char b[80]; volatile int w = 6; volatile long long v = 1234567890123LL;
    sprintf(b, "[%*lld]", w, v); CHECK_STR(b, "[1234567890123]");
    sprintf(b, "[%*lld]", 16, v); CHECK_STR(b, "[   1234567890123]");
#else
    TC_SKIP("ll not asserted before msvcr80");
#endif
}
TC_CASE(h) {
    char b[80]; volatile int w = 6, v = 70000;  /* 70000 = 0x11170; (short) -> 0x1170 = 4464 */
    sprintf(b, "[%*hd]", w, v); CHECK_STR(b, "[  4464]");
}

int main(void) { tc_begin(); TC_RUN(plus); TC_RUN(minus_prec); TC_RUN(hash_x); TC_RUN(i64); TC_RUN(ll); TC_RUN(h); return tc_end(); }
