/* TC
 * id:    pff_neg_snprintf_s_zero_size_float
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s
 * procs: _snwprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _snprintf_s: sizeOfBuffer must be > 0
 * note:  PROBES: _snprintf_s / _snwprintf_s with sizeOfBuffer==0 (and with NULL buffer + size 0 + _TRUNCATE) are treated as
 *        invalid parameters: return -1, errno EINVAL, handler count 1, and a non-NULL buffer is left untouched.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

TC_DECL_PROC(int, _snwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, ...));

#define EXPECT_FAIL(expr) do { int r_; tc_iph_count = 0; errno = 0; r_ = (expr); \
    CHECK_EQ_I(r_, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(null_size0_truncate)
{
    char * volatile nb = NULL; volatile double v = 1.0;
    EXPECT_FAIL(_snprintf_s(nb, 0, _TRUNCATE, "%f", v));
}
TC_CASE(buf_size0)
{
    char b[8]; volatile size_t z = 0; volatile double v = 1.0; int i;
    for (i = 0; i < 8; i++) b[i] = 'x';
    EXPECT_FAIL(_snprintf_s(b, z, 0, "%f", v));
    for (i = 0; i < 8; i++) CHECK(b[i] == 'x');
}
TC_CASE(wide_size0)
{
    wchar_t b[8]; volatile size_t z = 0; int i;
    if (!tc_p__snwprintf_s) TC_SKIP("_snwprintf_s not exported");
    for (i = 0; i < 8; i++) b[i] = L'x';
    EXPECT_FAIL(tc_p__snwprintf_s(b, z, 0, L"%f", 1.0));
    for (i = 0; i < 8; i++) CHECK(b[i] == L'x');
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_snwprintf_s);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_size0_truncate); TC_RUN(buf_size0); TC_RUN(wide_size0);
    return tc_end();
}
