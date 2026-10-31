/* TC
 * id:    pfi_neg_snprintf_s_truncate
 * area:  stdio_printf
 * funcs:
 * procs: _snprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s: _TRUNCATE truncates, returns -1, no invalid parameter
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#if TC_CRT_VER >= 140
/* ucrtbase: the header inlines _snprintf_s onto __stdio_common_vsnprintf_s */
#define p_snps _snprintf_s
#else
typedef int (__cdecl *snps_t)(char *, size_t, size_t, const char *, ...);
static snps_t p_snps;
#endif
#define TR ((size_t)-1)

TC_CASE(truncate) {
    char b[16]; int i, r; volatile size_t sz = 4, cnt = TR;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_count = 0;
    r = p_snps(b, sz, cnt, "%d", 12345);
    CHECK_EQ_I(r, -1);
    CHECK_STR(b, "123");
    CHECK_EQ_I(b[3], 0);
    CHECK_EQ_I(b[4], 'Z');
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(fits_with_truncate) {
    char b[16]; int r; volatile size_t sz = 8, cnt = TR;
    tc_iph_count = 0;
    r = p_snps(b, sz, cnt, "%d", 12345);
    CHECK_EQ_I(r, 5); CHECK_STR(b, "12345"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    p_snps = (snps_t)tc_crt_proc("_snprintf_s");
    if (!p_snps) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(truncate); TC_RUN(fits_with_truncate);
    return tc_end();
}
