/* TC
 * id:    pfw_neg_vfwprintf_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: vfwprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vfprintf, _vfprintf_l, vfwprintf (format NULL -> invalid parameter handler, EINVAL)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

#define FN L"tc_pfw_neg_vfwprintf_null_args_a.tmp"
static const wchar_t *volatile nfmt = NULL;

static int vfw(FILE *f, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = vfwprintf(f, fmt, ap); va_end(ap); return r; }

TC_CASE(null_format)
{
    FILE *f = _wfopen(FN, L"w"); int r;
    if (!f) TC_SKIP("cannot create temp file");
    tc_iph_count = 0; errno = 0;
    r = vfw(f, nfmt, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
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
