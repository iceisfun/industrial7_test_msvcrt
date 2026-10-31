/* TC
 * id:    pfx_neg_fprintf_l_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: fopen fclose
 * procs: _fprintf_l _fwprintf_l _vfprintf_l
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf, _fprintf_l, fwprintf, _fwprintf_l (parameter validation: NULL stream/format -> invalid parameter handler, EINVAL, -1)
 * note:  Resolved through procs: mingw import libs are incomplete for these, and ucrtbase does not export them (see pfx_neg_stdio_common_vfprintf_null_args).
 *        A NULL locale argument means the current locale.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define FN "tc_pfx_neg_fprintf_l_null_args.tmp"
static FILE *volatile nul;
static const char *volatile nfmt;
static const wchar_t *volatile nwfmt;

TC_DECL_PROC(int, _fprintf_l, (FILE *, const char *, void *, ...));
TC_DECL_PROC(int, _fwprintf_l, (FILE *, const wchar_t *, void *, ...));
TC_DECL_PROC(int, _vfprintf_l, (FILE *, const char *, void *, va_list));

static int vl(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = tc_p__vfprintf_l(f, fmt, NULL, ap); va_end(ap); return r; }

#define EXPECT_BAD(call) do { int r_; tc_iph_count = 0; errno = 0; r_ = (call); \
    CHECK_EQ_I(r_, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(null_stream) { EXPECT_BAD(tc_p__fprintf_l(nul, "x", NULL)); }
TC_CASE(null_format)
{
    FILE *f = fopen(FN, "w"); long sz;
    CHECK(f != NULL); if (!f) return;
    EXPECT_BAD(tc_p__fprintf_l(f, nfmt, NULL));
    fflush(f); sz = ftell(f); CHECK_EQ_I(sz, 0);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(wide_null_stream) { EXPECT_BAD(tc_p__fwprintf_l(nul, L"x", NULL)); }
TC_CASE(wide_null_format)
{
    FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    EXPECT_BAD(tc_p__fwprintf_l(f, nwfmt, NULL));
    fclose(f); DeleteFileA(FN);
}
TC_CASE(v_null_stream) { EXPECT_BAD(vl(nul, "x")); }
TC_CASE(v_null_format)
{
    FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    EXPECT_BAD(vl(f, nfmt));
    fclose(f); DeleteFileA(FN);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_fprintf_l); TC_LOAD_PROC(_fwprintf_l); TC_LOAD_PROC(_vfprintf_l);
    if (!tc_p__fprintf_l || !tc_p__fwprintf_l || !tc_p__vfprintf_l) { tc_write("TC FAIL procs missing\n"); return 1; }
    if (!tc_iph_install()) { tc_write("TC FAIL no iph\n"); return 1; }
    TC_RUN(null_stream); TC_RUN(null_format); TC_RUN(wide_null_stream); TC_RUN(wide_null_format);
    TC_RUN(v_null_stream); TC_RUN(v_null_format);
    return tc_end();
}
