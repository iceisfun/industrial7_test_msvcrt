/* TC
 * id:    pfr_neg_vswprintf_s_errors
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: vswprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vsprintf_s, vswprintf_s (ERANGE when buffer too small with buf[0]=0; EINVAL for NULL buffer / NULL format; sizeInWords)
 * note:  The wide va_list _s variant. The name _vswprintf_s is not a separate export; only vswprintf_s is tested.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

static int have_iph;

/* resolve at run time on every target (mingw's headers/import libs differ per CRT) */
typedef int (__cdecl *vswprintf_s_t)(wchar_t *, size_t, const wchar_t *, va_list);
static vswprintf_s_t p_vsws;
static int vs(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsws(b, n, f, ap); va_end(ap); return r; }

TC_CASE(size_equals_length)
{
    wchar_t b[8]; int r, i;
    if (!have_iph) TC_SKIP("no iph");
    for (i = 0; i < 8; i++) b[i] = L'X';
    tc_iph_count = 0; errno = 0;
    r = vs(b, 3, L"%ls", L"abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buffer)
{
    wchar_t *volatile nb = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vs(nb, 8, L"%ls", L"abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    wchar_t b[8]; const wchar_t *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vs(b, 8, f, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(exact_fit)
{
    wchar_t b[8]; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0;
    r = vs(b, 4, L"%ls", L"abc");
    CHECK_EQ_I(r, 3); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(ok_after_errors)
{
    wchar_t b[8]; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0;
    r = vs(b, 8, L"%d", 42);
    CHECK_EQ_I(r, 2); CHECK_WSTR(b, L"42"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
#if TC_CRT_VER >= 140
    p_vsws = (vswprintf_s_t)vswprintf_s;
#else
    p_vsws = (vswprintf_s_t)tc_crt_proc("vswprintf_s");
#endif
    if (!p_vsws) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
    TC_RUN(size_equals_length); TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(exact_fit); TC_RUN(ok_after_errors);
    return tc_end();
}
