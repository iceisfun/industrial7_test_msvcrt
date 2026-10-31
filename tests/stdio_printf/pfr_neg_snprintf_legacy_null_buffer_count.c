/* TC
 * id:    pfr_neg_snprintf_legacy_null_buffer_count
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf _snwprintf _vsnprintf _vsnwprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf, _snwprintf, _vsnprintf, _vsnwprintf (parameter validation: NULL buffer with count > 0 -> EINVAL, -1)
 * note:  Count 0 with a NULL buffer is covered elsewhere; here count is 5. After the failures a valid call must work and the
 *        invalid-parameter handler count must not have changed.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

static int have_iph;
static int vn(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap); return r; }
static int vnw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnwprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(snprintf_null)
{
    char *volatile nb = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _snprintf(nb, 5, "abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(snwprintf_null)
{
    wchar_t *volatile nb = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _snwprintf(nb, 5, L"abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vsnprintf_null)
{
    char *volatile nb = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vn(nb, 5, "%s", "abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vsnwprintf_null)
{
    wchar_t *volatile nb = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vnw(nb, 5, L"%ls", L"abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(valid_after)
{
    char b[16]; wchar_t w[16]; int r1, r2;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0;
    r1 = _snprintf(b, sizeof b, "%d", 123);
    r2 = _snwprintf(w, 16, L"%d", 456);
    CHECK_EQ_I(r1, 3); CHECK_STR(b, "123");
    CHECK_EQ_I(r2, 3); CHECK_WSTR(w, L"456");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(snprintf_null); TC_RUN(snwprintf_null); TC_RUN(vsnprintf_null); TC_RUN(vsnwprintf_null); TC_RUN(valid_after);
    return tc_end();
}
