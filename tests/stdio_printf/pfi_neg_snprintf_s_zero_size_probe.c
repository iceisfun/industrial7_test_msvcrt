/* TC
 * id:    pfi_neg_snprintf_s_zero_size_probe
 * area:  stdio_printf
 * funcs:
 * procs: _snprintf_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs: sizeOfBuffer must be > 0 for _s functions
 * note:  PROBE: _snprintf_s with valid buffer and sizeOfBuffer==0 returns -1, EINVAL and calls the invalid parameter handler (may be ERANGE or no-op on some CRTs).
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

TC_CASE(size_zero) {
    char b[8]; volatile size_t sz = 0, cnt = 4; int r;
    b[0] = 'Q';
    tc_iph_count = 0; errno = 0;
    r = p_snps(b, sz, cnt, "%d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    p_snps = (snps_t)tc_crt_proc("_snprintf_s");
    if (!p_snps) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(size_zero);
    return tc_end();
}
