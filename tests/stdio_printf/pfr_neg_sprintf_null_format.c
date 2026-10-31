/* TC
 * id:    pfr_neg_sprintf_null_format
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf _vsnprintf _snwprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf, _snprintf, _vsnprintf, _snwprintf; Parameter validation (NULL format: EINVAL, -1)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <string.h>

static int have_iph;
static int vn(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(sprintf_null_fmt)
{
    char b[8]; const char *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = sprintf(b, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vsnprintf_null_fmt)
{
    char b[8]; const char *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vn(b, sizeof b, f, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(snwprintf_null_fmt)
{
    wchar_t b[8]; const wchar_t *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _snwprintf(b, 8, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(sprintf_null_fmt); TC_RUN(vsnprintf_null_fmt); TC_RUN(snwprintf_null_fmt);
    return tc_end();
}
