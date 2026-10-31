/* TC
 * id:    pfw_neg_fwprintf_null_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fwprintf vfwprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf, _fprintf_l, fwprintf, _fwprintf_l (parameter validation: stream NULL -> EINVAL, invalid parameter handler)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

static int vfw(FILE *f, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = vfwprintf(f, fmt, ap); va_end(ap); return r; }

static FILE *volatile nstream = NULL;

TC_CASE(fwprintf_null)
{
    int r; tc_iph_count = 0; errno = 0;
    r = fwprintf(nstream, L"hello %d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vfwprintf_null)
{
    int r; tc_iph_count = 0; errno = 0;
    r = vfw(nstream, L"hello %d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph API\n"); return 1; }
    TC_RUN(fwprintf_null); TC_RUN(vfwprintf_null);
    return tc_end();
}
