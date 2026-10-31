/* TC
 * id:    pfu_neg_pct_n_wide_and_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf_s _snprintf_s fprintf_s vsprintf_s
 * procs: _get_printf_count_output
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf format specification (%n), _set_printf_count_output: %n is disabled by default and invokes the invalid parameter handler; applies to the secure printf family
 * note:  Skipped if the process-wide %n setting was already non-zero. Output contents after failure are not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(int, _get_printf_count_output, (void));
static int def_zero;

static int a_vsprintf_s(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vsprintf_s(b, n, f, ap); va_end(ap); return r; }

TC_CASE(default_zero)
{
    def_zero = tc_p__get_printf_count_output ? tc_p__get_printf_count_output() == 0 : 0;
    CHECK(def_zero);
}
TC_CASE(swprintf_s_n)
{
    wchar_t b[32]; int n = 1234, r; volatile const wchar_t *f = L"ab%n";
    if (!def_zero) TC_SKIP("%n enabled");
    tc_iph_count = 0;
    r = swprintf_s(b, 32, (const wchar_t *)f, &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(n, 1234); CHECK(tc_iph_count >= 1);  /* exact count not documented for the wide form */
}
TC_CASE(snprintf_s_n)
{
    char b[32]; int n = 1234, r; volatile const char *f = "ab%n";
    if (!def_zero) TC_SKIP("%n enabled");
    tc_iph_count = 0;
    r = _snprintf_s(b, 32, 10, (const char *)f, &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(n, 1234); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(fprintf_s_n)
{
    static const char fn[] = "tc_pfu_neg_pct_n_wide_and_stream_a.tmp";
    FILE *fp; int n = 1234, r; volatile const char *f = "ab%n";
    if (!def_zero) TC_SKIP("%n enabled");
    fp = fopen(fn, "w");
    CHECK(fp != NULL);
    if (!fp) return;
    tc_iph_count = 0;
    r = fprintf_s(fp, (const char *)f, &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(n, 1234); CHECK_EQ_I(tc_iph_count, 1);
    fclose(fp); remove(fn);
}
TC_CASE(vsprintf_s_n)
{
    char b[32]; int n = 1234, r; volatile const char *f = "ab%n";
    if (!def_zero) TC_SKIP("%n enabled");
    tc_iph_count = 0;
    r = a_vsprintf_s(b, 32, (const char *)f, &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(n, 1234); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_LOAD_PROC(_get_printf_count_output);
    TC_RUN(default_zero); TC_RUN(swprintf_s_n); TC_RUN(snprintf_s_n); TC_RUN(fprintf_s_n); TC_RUN(vsprintf_s_n);
    return tc_end();
}
