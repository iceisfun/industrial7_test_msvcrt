/* TC
 * id:    scn_neg_snscanf_null_args
 * area:  scanf
 * funcs: _snscanf _snwscanf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snscanf, _snwscanf: NULL input (with count > 0) or NULL format invokes the invalid parameter handler, returns EOF, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#ifdef _UCRT
/* mingw headers give no usable _snwscanf declaration on ucrt; call the ucrt entry point the way MS headers do */
#include <stdarg.h>
static int tcx_snwscanf(const wchar_t *s, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswscanf(_CRT_INTERNAL_LOCAL_SCANF_OPTIONS, s, n, f, NULL, ap); va_end(ap); return r; }
#undef _snwscanf
#define _snwscanf tcx_snwscanf
#endif

static const char *volatile v;
static const wchar_t *volatile wv;
static volatile size_t cnt;

static void c_null_str(void)
{
    int a = 5, n; cnt = 4; v = NULL;
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    n = _snscanf(v, cnt, "%d", &a);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(a, 5);
}
static void c_null_format(void)
{
    int n; cnt = 4; v = NULL;
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    n = _snscanf("12", cnt, v);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
static void c_wide_null_str(void)
{
    int a = 5, n; cnt = 4; wv = NULL;
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    n = _snwscanf(wv, cnt, L"%d", &a);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(a, 5);
}
static void c_wide_null_format(void)
{
    int n; cnt = 4; wv = NULL;
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    n = _snwscanf(L"12", cnt, wv);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
/* each bad call runs in a child so that a CRT that crashes on NULL (instead of validating) cannot mask the others */
static void in_child(const char *name, void (*fn)(void))
{
    const char *c = tc_child_name();
    if (c && tc_strequ(c, name)) { tc_case_name = name; fn(); ExitProcess(tc_case_failed ? 1 : 0); }
}
#define SPAWN(name) do { DWORD code = 99; int ok = tc_spawn_self(#name, &code, 20000); CHECK(ok); CHECK_EQ_U(code, 0); } while (0)
TC_CASE(p_null_str) { SPAWN(null_str); }
TC_CASE(p_null_format) { SPAWN(null_format); }
TC_CASE(p_wide_null_str) { SPAWN(wide_null_str); }
TC_CASE(p_wide_null_format) { SPAWN(wide_null_format); }

int main(void)
{
    tc_begin();
    in_child("null_str", c_null_str); in_child("null_format", c_null_format);
    in_child("wide_null_str", c_wide_null_str); in_child("wide_null_format", c_wide_null_format);
    TC_RUN(p_null_str); TC_RUN(p_null_format); TC_RUN(p_wide_null_str); TC_RUN(p_wide_null_format);
    return tc_end();
}
