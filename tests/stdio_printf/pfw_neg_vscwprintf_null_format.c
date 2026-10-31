/* TC
 * id:    pfw_neg_vscwprintf_null_format
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vscwprintf _scwprintf
 * procs: _vscwprintf_p
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _scprintf, _scwprintf, _vscwprintf, _scprintf_p: format NULL -> invalid parameter handler, returns -1, errno EINVAL
 * note:  _vscwprintf_p is resolved at run time on legacy CRTs; on ucrtbase it is an inline wrapper over __stdio_common_vswprintf_p
 *        (options 6 = legacy-wide-specifiers | standard-snprintf, buffer NULL/0).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
static int sh_vscwp(const wchar_t *f, va_list ap) { return __stdio_common_vswprintf_p(6, NULL, 0, f, NULL, ap); }
#else
typedef int (__cdecl *vscwp_t)(const wchar_t *, va_list);
static vscwp_t p_vscwprintf_p;
static int sh_vscwp(const wchar_t *f, va_list ap) { return p_vscwprintf_p ? p_vscwprintf_p(f, ap) : -99; }
#endif
static int vscw(const wchar_t *f, ...)   { int r; va_list ap; va_start(ap, f); r = _vscwprintf(f, ap); va_end(ap); return r; }
static int vscwp(const wchar_t *f, ...)  { int r; va_list ap; va_start(ap, f); r = sh_vscwp(f, ap); va_end(ap); return r; }
static const wchar_t * volatile nfmt;
static const wchar_t * volatile dfmt = L"%d";
static const wchar_t * volatile pfmt = L"%2$d-%1$d";

TC_CASE(vscwprintf_null)
{
    int r; tc_iph_count = 0; errno = 0;
    r = vscw(nfmt, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(scwprintf_null)
{
    int r; tc_iph_count = 0; errno = 0;
    r = _scwprintf(nfmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(no_sticky_state)
{
    int r;
    tc_iph_count = 0; errno = 0; r = vscw(nfmt, 1); CHECK_EQ_I(r, -1);
    errno = 0; CHECK_EQ_I(vscw(dfmt, 12345), 5);
    CHECK_EQ_I(_scwprintf(dfmt, -7), 2);
    CHECK_EQ_I(errno, 0);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(positional_null)
{
    int r;
#if TC_CRT_VER < 140
    if (!p_vscwprintf_p) TC_SKIP("_vscwprintf_p not exported");
#endif
    tc_iph_count = 0; errno = 0;
    r = vscwp(nfmt, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    errno = 0; CHECK_EQ_I(vscwp(pfmt, 12, 345), 6); CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vscwprintf_p = (vscwp_t)tc_crt_proc("_vscwprintf_p");
#endif
    TC_RUN(vscwprintf_null); TC_RUN(scwprintf_null); TC_RUN(no_sticky_state); TC_RUN(positional_null);
    return tc_end();
}
