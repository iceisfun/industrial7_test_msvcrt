/* TC
 * id:    scn_neg_common_vsscanf_null_args
 * area:  scanf
 * kind:  negative
 * funcs:
 * procs: __stdio_common_vsscanf __stdio_common_vswscanf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs sscanf, swscanf (NULL buffer/format -> invalid parameter handler, EOF, errno EINVAL); the __stdio_common_* functions are what the inline sscanf/swscanf call
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

typedef int (__cdecl *cn_t)(unsigned long long, const char *, size_t, const char *, void *, va_list);
typedef int (__cdecl *cw_t)(unsigned long long, const wchar_t *, size_t, const wchar_t *, void *, va_list);
static cn_t p_cn; static cw_t p_cw;

static int callN(const char *s, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = p_cn(0, s, (size_t)-1, f, NULL, ap); va_end(ap); return r; }
static int callW(const wchar_t *s, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = p_cw(0, s, (size_t)-1, f, NULL, ap); va_end(ap); return r; }
static const char *volatile ns; static const wchar_t *volatile nws;

TC_CASE(input_null)
{
    int x = 5, r; if (!p_cn) TC_SKIP("not exported"); tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = callN((const char *)ns, "%d", &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(x, 5);
}
TC_CASE(format_null)
{
    int x = 5, r; if (!p_cn) TC_SKIP("not exported"); tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = callN("1", (const char *)ns, &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(x, 5);
}
TC_CASE(wide_input_null)
{
    int x = 5, r; if (!p_cw) TC_SKIP("not exported"); tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = callW((const wchar_t *)nws, L"%d", &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(x, 5);
}
TC_CASE(wide_format_null)
{
    int x = 5, r; if (!p_cw) TC_SKIP("not exported"); tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = callW(L"1", (const wchar_t *)nws, &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(x, 5);
}
TC_CASE(valid_control)
{
    int x = 0, y = 0, r;
    if (!p_cn || !p_cw) TC_SKIP("not exported"); tc_iph_install(); tc_iph_count = 0;
    r = callN("17", "%d", &x); CHECK_EQ_I(r, 1); CHECK_EQ_I(x, 17);
    r = callW(L"18", L"%d", &y); CHECK_EQ_I(r, 1); CHECK_EQ_I(y, 18);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    p_cn = (cn_t)tc_crt_proc("__stdio_common_vsscanf"); p_cw = (cw_t)tc_crt_proc("__stdio_common_vswscanf");
    CHECK_EQ_I(p_cn != NULL && p_cw != NULL, 1);
    TC_RUN(input_null); TC_RUN(format_null); TC_RUN(wide_input_null); TC_RUN(wide_format_null); TC_RUN(valid_control);
    return tc_end();
}
