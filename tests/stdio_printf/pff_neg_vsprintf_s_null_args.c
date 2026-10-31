/* TC
 * id:    pff_neg_vsprintf_s_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: vsprintf_s _vsnprintf_s vswprintf_s
 * procs: _vsnwprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vsprintf_s, _vsnprintf_s, vswprintf_s, _vsnwprintf_s: NULL buffer or NULL format -> invalid parameter handler, -1, errno EINVAL
 * note:  Return value, errno and handler count only; the buffer state after failure is in pff_neg_probe_vsprintf_s_null_fmt_buf0.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

TC_DECL_PROC(int, _vsnwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, va_list));

static int c_vs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vsprintf_s(b, n, f, ap); va_end(ap); return r; }
static int c_vsn(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnprintf_s(b, n, c, f, ap); va_end(ap); return r; }
static int c_vsw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vswprintf_s(b, n, f, ap); va_end(ap); return r; }
static int c_vsnw(wchar_t *b, size_t n, size_t c, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = tc_p__vsnwprintf_s(b, n, c, f, ap); va_end(ap); return r; }

#define EXPECT_FAIL(expr) do { int r_; tc_iph_count = 0; errno = 0; r_ = (expr); \
    CHECK_EQ_I(r_, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(vsprintf_s_null)
{
    char b[16]; char * volatile nb = NULL; const char * volatile nf = NULL;
    EXPECT_FAIL(c_vs(nb, 16, "%f", 1.0));
    EXPECT_FAIL(c_vs(b, 16, nf, 1.0));
}
TC_CASE(vsnprintf_s_null)
{
    char b[16]; char * volatile nb = NULL; const char * volatile nf = NULL;
    EXPECT_FAIL(c_vsn(nb, 16, _TRUNCATE, "%e", 1.0));
    EXPECT_FAIL(c_vsn(b, 16, _TRUNCATE, nf, 1.0));
}
TC_CASE(vswprintf_s_null)
{
    wchar_t b[16]; wchar_t * volatile nb = NULL; const wchar_t * volatile nf = NULL;
    EXPECT_FAIL(c_vsw(nb, 16, L"%g", 1.0));
    EXPECT_FAIL(c_vsw(b, 16, nf, 1.0));
}
TC_CASE(vsnwprintf_s_null)
{
    wchar_t b[16]; wchar_t * volatile nb = NULL; const wchar_t * volatile nf = NULL;
    if (!tc_p__vsnwprintf_s) TC_SKIP("_vsnwprintf_s not exported");
    EXPECT_FAIL(c_vsnw(nb, 16, _TRUNCATE, L"%f", 1.0));
    EXPECT_FAIL(c_vsnw(b, 16, _TRUNCATE, nf, 1.0));
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_vsnwprintf_s);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(vsprintf_s_null); TC_RUN(vsnprintf_s_null); TC_RUN(vswprintf_s_null); TC_RUN(vsnwprintf_s_null);
    return tc_end();
}
