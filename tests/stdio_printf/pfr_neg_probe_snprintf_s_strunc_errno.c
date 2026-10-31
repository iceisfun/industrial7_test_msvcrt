/* TC
 * id:    pfr_neg_probe_snprintf_s_strunc_errno
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _snprintf_s, _vsnprintf_s, _snwprintf_s
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _snprintf_s ("If truncation occurs ... -1"); errno.h STRUNCATE (80); strcpy_s _TRUNCATE
 * note:  PROBE (errno_after_truncate only): a _TRUNCATE truncation by the printf _s family is most likely NOT an errno-setting
 *        event (STRUNCATE is documented only for the str*_s copy functions), so errno stays at sentinel 4242; also left
 *        untouched when output fits. Sibling probes (pfu_/pfw_) test the STRUNCATE/ERANGE alternatives. Returns (-1) and
 *        NUL termination are documented and asserted in separate cases.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

#define SENT 4242

typedef int (__cdecl *snprintf_s_t)(char *, size_t, size_t, const char *, ...);
typedef int (__cdecl *vsnprintf_s_t)(char *, size_t, size_t, const char *, va_list);
typedef int (__cdecl *snwprintf_s_t)(wchar_t *, size_t, size_t, const wchar_t *, ...);
static snprintf_s_t p_sn; static vsnprintf_s_t p_vsn; static snwprintf_s_t p_snw;
static int vn(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsn(b, n, c, f, ap); va_end(ap); return r; }

TC_CASE(snprintf_s)
{
    char b[16]; int r;
    tc_iph_count = 0;
    r = p_sn(b, 4, (size_t)-1, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(vsnprintf_s)
{
    char b[16]; int r;
    tc_iph_count = 0;
    r = vn(b, 4, (size_t)-1, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(snwprintf_s)
{
    wchar_t b[16]; int r;
    tc_iph_count = 0;
    r = p_snw(b, 4, (size_t)-1, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(errno_after_truncate)
{
    char b[16]; wchar_t w[16];
    errno = SENT; (void)p_sn(b, 4, (size_t)-1, "%s", "abcdefgh");
    CHECK_EQ_I(errno, SENT);
    errno = SENT; (void)vn(b, 4, (size_t)-1, "%s", "abcdefgh");
    CHECK_EQ_I(errno, SENT);
    errno = SENT; (void)p_snw(w, 4, (size_t)-1, L"%ls", L"abcdefgh");
    CHECK_EQ_I(errno, SENT);
}
TC_CASE(fits_errno_untouched)
{
    char b[16]; int r;
    tc_iph_count = 0; errno = SENT;
    r = p_sn(b, 16, (size_t)-1, "%s", "abc");
    CHECK_EQ_I(r, 3); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, SENT);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_sn = (snprintf_s_t)tc_crt_proc("_snprintf_s");
    p_vsn = (vsnprintf_s_t)tc_crt_proc("_vsnprintf_s");
    p_snw = (snwprintf_s_t)tc_crt_proc("_snwprintf_s");
#if TC_CRT_VER >= 140
    if (!p_sn) p_sn = (snprintf_s_t)_snprintf_s;
    if (!p_vsn) p_vsn = (vsnprintf_s_t)_vsnprintf_s;
    if (!p_snw) p_snw = (snwprintf_s_t)_snwprintf_s;
#endif
    if (!p_sn || !p_vsn || !p_snw) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
    TC_RUN(snprintf_s); TC_RUN(vsnprintf_s); TC_RUN(snwprintf_s); TC_RUN(errno_after_truncate); TC_RUN(fits_errno_untouched);
    return tc_end();
}
