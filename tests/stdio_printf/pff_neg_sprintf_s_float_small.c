/* TC
 * id:    pff_neg_sprintf_s_float_small
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s: if the buffer is too small the invalid parameter handler is invoked, buffer[0] set to 0, -1 returned
 */
#include "tc.h"
#include <stdio.h>
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));

TC_CASE(f_small) { char b[16]; volatile double v = 1.5; int n, i; if (!tc_p_sprintf_s) TC_SKIP("none"); for (i = 0; i < 16; i++) b[i] = 'Z';
  tc_iph_count = 0; n = tc_p_sprintf_s(b, 4, "%f", v); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); }
TC_CASE(prec100_small) { char b[16]; volatile double v = 0.5; int n, i; if (!tc_p_sprintf_s) TC_SKIP("none"); for (i = 0; i < 16; i++) b[i] = 'Z';
  tc_iph_count = 0; n = tc_p_sprintf_s(b, 4, "%.100f", v); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); }
TC_CASE(fits) { char b[16]; volatile double v = 1.5; int n; if (!tc_p_sprintf_s) TC_SKIP("none");
  tc_iph_count = 0; n = tc_p_sprintf_s(b, 4, "%.1f", v); CHECK_EQ_I(n, 3); CHECK_STR(b, "1.5"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); tc_iph_install(); TC_LOAD_PROC(sprintf_s); CHECK(tc_p_sprintf_s != NULL);
  TC_RUN(f_small); TC_RUN(prec100_small); TC_RUN(fits); return tc_end(); }
