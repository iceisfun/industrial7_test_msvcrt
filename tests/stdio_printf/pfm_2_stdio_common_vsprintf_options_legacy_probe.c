/* TC
 * id:    pfm_2_stdio_common_vsprintf_options_legacy_probe
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   Windows SDK corecrt_stdio_config.h: LEGACY_WIDE_SPECIFIERS (4), LEGACY_MSVCRT_COMPATIBILITY (8)
 * note:  PROBES, one flag per case. Asserted: narrow %s/%d/%c output is unaffected by flag 4 (the flag concerns wide-format
 *        specifier meaning); flag 8 is only recorded (TC NOTE lines: result and output for %hc, %F, %.0a, %s of a NULL pointer) and
 *        checked for self-consistency (return == strlen of buffer, or -1). Real Windows output for flag 8 is what is wanted.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int sv(unsigned __int64 opt, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(opt, b, n, f, NULL, ap); va_end(ap); return r; }
static void note(const char *k, long long v) { char b[32]; tc_write("TC NOTE " TC_TEST_ID " "); tc_write(k); tc_write("="); tc_i64(v, b); tc_write(b); tc_write("\n"); }
static void notes(const char *k, const char *s) { tc_write("TC NOTE " TC_TEST_ID " "); tc_write(k); tc_write("=["); tc_write(s); tc_write("]\n"); }
static size_t len(const char *s) { size_t n = 0; while (n < 100 && s[n]) n++; return n; }

static volatile int vc = 'x';
static volatile double v15 = 1.5;
static const char *volatile nullp = NULL;
static const char *volatile str = "str";

TC_CASE(opt4_narrow_unchanged)
{
    char b[64]; int r = sv(4, b, 64, "%s|%d|%c", str, 12, vc);
    CHECK_STR(b, "str|12|x"); CHECK_EQ_I(r, 8);
}
TC_CASE(opt8_hc)   { char b[64]; int r; b[0] = 0; r = sv(8, b, 64, "[%hc]", vc); note("opt8_hc_ret", r); notes("opt8_hc", b); CHECK(r == -1 || (size_t)r == len(b)); }
TC_CASE(opt8_F)    { char b[64]; int r; b[0] = 0; r = sv(8, b, 64, "[%F]", v15); note("opt8_F_ret", r); notes("opt8_F", b); CHECK(r == -1 || (size_t)r == len(b)); }
TC_CASE(opt8_a)    { char b[64]; int r; b[0] = 0; r = sv(8, b, 64, "[%.0a]", v15); note("opt8_a_ret", r); notes("opt8_a", b); CHECK(r == -1 || (size_t)r == len(b)); }
TC_CASE(opt8_null_s) { char b[64]; int r; b[0] = 0; r = sv(8, b, 64, "[%s]", nullp); note("opt8_nulls_ret", r); notes("opt8_nulls", b); CHECK(r == -1 || (size_t)r == len(b)); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(opt4_narrow_unchanged); TC_RUN(opt8_hc); TC_RUN(opt8_F); TC_RUN(opt8_a); TC_RUN(opt8_null_s);
    return tc_end();
}
