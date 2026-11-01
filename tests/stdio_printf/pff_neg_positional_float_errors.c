/* TC
 * id:    pff_neg_positional_float_errors
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_p _vsprintf_p __stdio_common_vswprintf_p
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs "printf_p positional parameters" (_sprintf_p, _vsprintf_p, _swprintf_p): positions are 1-based; invalid format -> invalid parameter handler
 * note:  Malformed positional formats with %f fail with -1, handler count 1, errno EINVAL (gap, position 0, mixed, NULL buffer; narrow and, via the
 *        __stdio_common_vswprintf_p worker, wide; _swprintf_p itself is not exported from ucrtbase, so the worker is called directly).
 *        too_small, observed on Windows 10 22H2: _sprintf_p(b,4,"%1$f",1.0) returns -2 (the raw worker value, NOT -1), does not call the handler,
 *        leaves errno 0 and writes the truncated, NUL-terminated "1.0". -2 is the worker status as seen through the mingw wrapper (no -1 normalisation; MSVC SDK inline wrappers may map it to -1 [unverified]). Differs from the documented -1 / handler / buf[0]==0 secure-style failure.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

static int vwp(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf_p(0, b, n, f, NULL, ap); va_end(ap); return r; }

#define EXPECT_FAIL(expr) do { int r_; tc_iph_count = 0; errno = 0; r_ = (expr); \
    CHECK_EQ_I(r_, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

static int vp(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsprintf_p(b, n, f, ap); va_end(ap); return r; }

TC_CASE(missing_position_1)  { char b[32]; const char * volatile f = "%2$f"; EXPECT_FAIL(_sprintf_p(b, 32, f, 1.0)); }
TC_CASE(position_zero)       { char b[32]; const char * volatile f = "%0$f"; EXPECT_FAIL(_sprintf_p(b, 32, f, 1.0)); }
TC_CASE(mixed_positional)    { char b[32]; const char * volatile f = "%1$f %f"; EXPECT_FAIL(_sprintf_p(b, 32, f, 1.0)); }
TC_CASE(vsprintf_p_gap)      { char b[32]; const char * volatile f = "%2$e"; EXPECT_FAIL(vp(b, 32, f, 1.0, 2.0)); }
TC_CASE(wide_position_zero)
{
    wchar_t b[32]; const wchar_t * volatile f = L"%0$f";
    EXPECT_FAIL(vwp(b, 32, f, 1.0));
}
TC_CASE(too_small)
{
    char b[16]; const char * volatile f = "%1$f"; int r, i;
    for (i = 0; i < 16; i++) b[i] = 'x';
    tc_iph_count = 0; errno = 0;
    r = _sprintf_p(b, 4, f, 1.0);
    CHECK_EQ_I(r, -2); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); CHECK_STR(b, "1.0");
}
TC_CASE(null_buffer)
{
    char * volatile nb = NULL; const char * volatile f = "%1$f";
    EXPECT_FAIL(_sprintf_p(nb, 32, f, 1.0));
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(missing_position_1); TC_RUN(position_zero); TC_RUN(mixed_positional); TC_RUN(vsprintf_p_gap);
    TC_RUN(wide_position_zero); TC_RUN(too_small); TC_RUN(null_buffer);
    return tc_end();
}
