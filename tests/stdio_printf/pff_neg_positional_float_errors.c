/* TC
 * id:    pff_neg_positional_float_errors
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_p _vsprintf_p
 * procs: _swprintf_p
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "printf_p positional parameters" (_sprintf_p, _vsprintf_p, _swprintf_p): positions are 1-based; invalid format -> invalid parameter handler
 * note:  PROBES that malformed positional formats with %f fail with -1, handler count 1, errno EINVAL: a gap (%2$f without %1$),
 *        position 0, mixing positional and sequential specs; too-small buffer (-1, handler 1, buf[0]==0, errno nonzero); NULL buffer.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

TC_DECL_PROC(int, _swprintf_p, (wchar_t *, size_t, const wchar_t *, ...));

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
    if (!tc_p__swprintf_p) TC_SKIP("_swprintf_p not exported");
    EXPECT_FAIL(tc_p__swprintf_p(b, 32, f, 1.0));
}
TC_CASE(too_small)
{
    char b[16]; const char * volatile f = "%1$f"; int r, i;
    for (i = 0; i < 16; i++) b[i] = 'x';
    tc_iph_count = 0; errno = 0;
    r = _sprintf_p(b, 4, f, 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK(errno != 0); CHECK_EQ_I(b[0], 0);
}
TC_CASE(null_buffer)
{
    char * volatile nb = NULL; const char * volatile f = "%1$f";
    EXPECT_FAIL(_sprintf_p(nb, 32, f, 1.0));
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_swprintf_p);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(missing_position_1); TC_RUN(position_zero); TC_RUN(mixed_positional); TC_RUN(vsprintf_p_gap);
    TC_RUN(wide_position_zero); TC_RUN(too_small); TC_RUN(null_buffer);
    return tc_end();
}
