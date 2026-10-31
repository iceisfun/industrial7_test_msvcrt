/* TC
 * id:    pfw_neg_wide_percent_n_disabled
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snwprintf
 * procs: _get_printf_count_output
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_printf_count_output: %n support is disabled by default (VS2005+); format specification: %n invalid when disabled
 * note:  With %n disabled the wide printf functions treat it as an invalid parameter: invalid-parameter handler runs,
 *        errno == EINVAL, return -1, and the target int is never written.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define MAGIC 0x5a5a1234

TC_CASE(snwprintf_percent_n)
{
    wchar_t b[32]; volatile int target = MAGIC; int n; int r;
    tc_iph_count = 0; errno = 0;
    r = _snwprintf(b, 32, L"ab%ncd", &target);
    n = r;
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(target, MAGIC);
}
TC_CASE(fwprintf_percent_n)
{
    FILE *f = _wfopen(L"tc_pfw_neg_wide_percent_n_disabled_a.tmp", L"w"); volatile int target = MAGIC; int r;
    if (!f) TC_SKIP("cannot create temp file");
    tc_iph_count = 0; errno = 0;
    r = fwprintf(f, L"ab%ncd", &target);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(target, MAGIC);
    fclose(f);
    _wunlink(L"tc_pfw_neg_wide_percent_n_disabled_a.tmp");
}
TC_CASE(default_is_off)
{
    int (__cdecl *get)(void) = (int (__cdecl *)(void))tc_crt_proc("_get_printf_count_output");
    if (!get) TC_SKIP("_get_printf_count_output not exported");
    CHECK_EQ_I(get(), 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph API\n"); return 1; }
    TC_RUN(snwprintf_percent_n); TC_RUN(fwprintf_percent_n); TC_RUN(default_is_off);
    return tc_end();
}
