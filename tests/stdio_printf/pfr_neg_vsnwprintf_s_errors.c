/* TC
 * id:    pfr_neg_vsnwprintf_s_errors
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vsnwprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _vsnprintf_s, _vsnwprintf_s (count > size and too small: ERANGE, buf[0]=0; _TRUNCATE: truncate, -1, no handler; NULL: EINVAL)
 * note:  errno after _TRUNCATE truncation is probed separately (pfr_neg_probe_snprintf_s_strunc_errno).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

static int have_iph;

typedef int (__cdecl *vsnws_t)(wchar_t *, size_t, size_t, const wchar_t *, va_list);
static vsnws_t p_vsnws;
static int vs(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsnws(b, n, c, f, ap); va_end(ap); return r; }

TC_CASE(count_gt_size_erange)
{
    wchar_t b[8]; int r, i;
    if (!have_iph) TC_SKIP("no iph");
    for (i = 0; i < 8; i++) b[i] = L'X';
    tc_iph_count = 0; errno = 0;
    r = vs(b, 4, 10, L"%ls", L"abcdef");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(truncate)
{
    wchar_t b[8]; int r, i;
    if (!have_iph) TC_SKIP("no iph");
    for (i = 0; i < 8; i++) b[i] = L'X';
    tc_iph_count = 0;
    r = vs(b, 4, (size_t)-1, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(b[4], L'X'); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_buffer)
{
    wchar_t *volatile nb = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vs(nb, 8, 4, L"%ls", L"x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    wchar_t b[8]; const wchar_t *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vs(b, 8, 4, f, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    p_vsnws = (vsnws_t)tc_crt_proc("_vsnwprintf_s");
#if TC_CRT_VER >= 140
    if (!p_vsnws) p_vsnws = (vsnws_t)_vsnwprintf_s;
#endif
    if (!p_vsnws) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
    TC_RUN(count_gt_size_erange); TC_RUN(truncate); TC_RUN(null_buffer); TC_RUN(null_format);
    return tc_end();
}
