/* TC
 * id:    err_neg_probe_strerror_s_extreme
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: strerror_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs strerror_s, _strerror_s, _wcserror_s
 * note:  PROBE: strerror_s(buf, 100, INT_MAX / INT_MIN / -1) returns 0 and writes a non-empty NUL-terminated "Unknown error"-style text
 *        (out-of-range numbers are not errors for strerror_s; only buffer problems are), without invoking the handler.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>

TC_DECL_PROC(int, strerror_s, (char *, size_t, int));
static volatile int v_e;
static int blen(const char *s, int max) { int i; for (i = 0; i < max; i++) if (!s[i]) return i; return -1; }

TC_CASE(int_max) { char b[100]; int r; b[0] = 0; v_e = (INT_MAX); tc_iph_count = 0; r = tc_p_strerror_s(b, sizeof b, v_e); CHECK_EQ_I(r, 0); CHECK(blen(b, 100) > 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(int_min) { char b[100]; int r; b[0] = 0; v_e = (INT_MIN); tc_iph_count = 0; r = tc_p_strerror_s(b, sizeof b, v_e); CHECK_EQ_I(r, 0); CHECK(blen(b, 100) > 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(minus_one) { char b[100]; int r; b[0] = 0; v_e = (-1); tc_iph_count = 0; r = tc_p_strerror_s(b, sizeof b, v_e); CHECK_EQ_I(r, 0); CHECK(blen(b, 100) > 0); CHECK_EQ_I(tc_iph_count, 0); }

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(strerror_s);
    if (!tc_p_strerror_s) { tc_write("TC FAIL " TC_TEST_ID "::strerror_s missing\n"); return 1; }
    tc_iph_install();
    TC_RUN(int_max); TC_RUN(int_min); TC_RUN(minus_one);
    return tc_end();
}
