/* TC
 * id:    pfr_neg_scprintf_null_format
 * area:  stdio_printf
 * kind:  negative
 * funcs: _scprintf _vscprintf _scwprintf _vscwprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _scprintf, _vscprintf, _scwprintf, _vscwprintf (NULL format: EINVAL, -1)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <string.h>

static int have_iph;
static int vc(const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vscprintf(f, ap); va_end(ap); return r; }
static int vcw(const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vscwprintf(f, ap); va_end(ap); return r; }

TC_CASE(scprintf)
{
    const char *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _scprintf(f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vscprintf)
{
    const char *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vc(f, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(scwprintf)
{
    const wchar_t *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _scwprintf(f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vscwprintf)
{
    const wchar_t *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vcw(f, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(scprintf); TC_RUN(vscprintf); TC_RUN(scwprintf); TC_RUN(vscwprintf);
    return tc_end();
}
