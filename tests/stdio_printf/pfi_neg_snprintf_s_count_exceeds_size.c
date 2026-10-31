/* TC
 * id:    pfi_neg_snprintf_s_count_exceeds_size
 * area:  stdio_printf
 * funcs:
 * procs: _snprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s / Security Features in the CRT: output does not fit in sizeOfBuffer without _TRUNCATE -> invalid parameter handler, buffer[0]=0, ERANGE, -1
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

TC_CASE(too_small) {
    char b[16]; int i, r; volatile size_t sz = 4, cnt = 10;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_count = 0; errno = 0;
    r = p_snps(b, sz, cnt, "%d", 12345);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(b[0], 0);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    p_snps = (snps_t)tc_crt_proc("_snprintf_s");
    if (!p_snps) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(too_small);
    return tc_end();
}
