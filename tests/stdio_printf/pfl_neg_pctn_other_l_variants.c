/* TC
 * id:    pfl_neg_pctn_other_l_variants
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsnprintf_l _vsprintf_l _vfprintf_l _vsnwprintf_l _create_locale _free_locale fopen fclose
 * procs: _get_printf_count_output
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_printf_count_output, _get_printf_count_output (%n is disabled by default); printf format specification (%n); "Parameter validation"
 * note:  Extends pfl_neg_printf_n_disabled (_sprintf_l only) to the other _l entry points. Cases skip when the process has %n enabled.
 *        _swprintf_l (unbounded wide) is asserted only as negative. Only the return value, errno, handler count and the untouched target are asserted, not what was written before the failure.
 *        Temp file tc_pfl_neg_pctn_other_l_variants.tmp.
 */
#include "tc.h"
#include "pfl_shim.h"
#include <errno.h>

TC_DECL_PROC(int, _get_printf_count_output, (void));
static _locale_t loc;
static int def_zero;
static const char *fname = "tc_pfl_neg_pctn_other_l_variants.tmp";
static const char * volatile v_pctn = "%n";

TC_CASE(snprintf_l)
{
    char b[16]; int n = 12345, r;
    if (!def_zero) TC_SKIP("%n enabled");
    tc_iph_count = 0; errno = 0;
    r = pl_snprintf_l(b, 16, v_pctn, loc, &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 12345);
}
TC_CASE(fprintf_l)
{
    int n = 12345, r; FILE *f;
    if (!def_zero) TC_SKIP("%n enabled");
    f = fopen(fname, "w"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = pl_fprintf_l(f, "ab%n", loc, &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 12345);
    fclose(f); remove(fname);
}
TC_CASE(vsprintf_l)
{
    char b[16]; int n = 12345, r;
    if (!def_zero) TC_SKIP("%n enabled");
    tc_iph_count = 0; errno = 0;
    r = pl_sprintf_l(b, v_pctn, loc, &n);   /* wrapper forwards a va_list to _vsprintf_l */
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 12345);
}
TC_CASE(swprintf_l)
{
    wchar_t b[16]; int n = 12345, r;
    if (!def_zero) TC_SKIP("%n enabled");
    tc_iph_count = 0; errno = 0;
    r = pl_swprintf_l(b, L"%n", loc, &n);
    CHECK(r < 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 12345);
}
TC_CASE(counter_matches_calls)
{
    char b[16]; wchar_t w[16]; int n = 7, i;
    if (!def_zero) TC_SKIP("%n enabled");
    tc_iph_count = 0;
    for (i = 0; i < 3; i++) pl_snprintf_l(b, 16, v_pctn, loc, &n);
    pl_swprintf_l(w, L"x%n", loc, &n);
    CHECK_EQ_I(tc_iph_count, 4); CHECK_EQ_I(n, 7);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_get_printf_count_output);
    if (tc_p__get_printf_count_output) def_zero = tc_p__get_printf_count_output() == 0;
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::main _create_locale(C) failed\n"); return 1; }
    TC_RUN(snprintf_l); TC_RUN(fprintf_l); TC_RUN(vsprintf_l); TC_RUN(swprintf_l); TC_RUN(counter_matches_calls);
    _free_locale(loc);
    remove(fname);
    return tc_end();
}
