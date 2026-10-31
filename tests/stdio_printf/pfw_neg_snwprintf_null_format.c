/* TC
 * id:    pfw_neg_snwprintf_null_format
 * area:  stdio_printf
 * funcs: _snwprintf fwprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation: NULL format string invokes the invalid parameter handler; returns -1, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static const wchar_t *volatile nfmt = NULL;
TC_CASE(snwprintf_null_format)
{
    wchar_t b[8]; int n; b[0] = L'Z';
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    n = _snwprintf(b, 8, nfmt);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(fwprintf_null_file)
{
    int n; FILE *volatile f = NULL;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    n = fwprintf(f, L"x");
    CHECK(n < 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(snwprintf_null_format); TC_RUN(fwprintf_null_file); return tc_end(); }
