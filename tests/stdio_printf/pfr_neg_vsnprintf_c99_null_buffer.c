/* TC
 * id:    pfr_neg_vsnprintf_c99_null_buffer
 * area:  stdio_printf
 * kind:  negative
 * funcs: vsnprintf vswprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.6.12 vsnprintf (n==0: nothing written, buffer may be null, returns would-be length); MS docs vsnprintf, vswprintf (NULL buffer with count > 0, NULL format: EINVAL, -1)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

static int have_iph;
static int vn(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vsnprintf(b, n, f, ap); va_end(ap); return r; }
static int vw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vswprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(null_count5)
{
    char *volatile nb = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vn(nb, 5, "abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_count0_query)
{
    char *volatile nb = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0;
    r = vn(nb, 0, "abcd");
    CHECK_EQ_I(r, 4); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_null_count5)
{
    wchar_t *volatile nb = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vw(nb, 5, L"abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[8]; const char *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vn(b, 5, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(null_count5); TC_RUN(null_count0_query); TC_RUN(wide_null_count5); TC_RUN(null_format);
    return tc_end();
}
