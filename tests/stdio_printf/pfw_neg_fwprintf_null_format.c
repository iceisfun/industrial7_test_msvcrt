/* TC
 * id:    pfw_neg_fwprintf_null_format
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fwprintf (format NULL -> invalid parameter handler, EINVAL, returns negative / -1)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define FN L"tc_pfw_neg_fwprintf_null_format_a.tmp"
static const wchar_t *volatile nfmt = NULL;

TC_CASE(null_format)
{
    FILE *f = _wfopen(FN, L"w"); int r; long sz;
    if (!f) TC_SKIP("cannot create temp file");
    tc_iph_count = 0; errno = 0;
    r = fwprintf(f, nfmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fflush(f);
    CHECK_EQ_I(fseek(f, 0, SEEK_END), 0);
    sz = ftell(f);
    CHECK_EQ_I(sz, 0);
    fclose(f);
    _wunlink(FN);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph API\n"); return 1; }
    TC_RUN(null_format);
    return tc_end();
}
