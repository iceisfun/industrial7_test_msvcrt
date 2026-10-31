/* TC
 * id:    pff_neg_swprintf_s_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf_s
 * procs: _snwprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs swprintf_s, _snwprintf_s: NULL buffer or NULL format -> invalid parameter handler, -1, errno EINVAL
 * note:  Buffer state and zero-size behaviour are in pff_neg_probe_vsprintf_s_null_fmt_buf0 / pff_neg_snprintf_s_zero_size_float.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

TC_DECL_PROC(int, _snwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, ...));

#define EXPECT_FAIL(expr) do { int r_; tc_iph_count = 0; errno = 0; r_ = (expr); \
    CHECK_EQ_I(r_, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(swprintf_s_null)
{
    wchar_t b[10]; wchar_t * volatile nb = NULL; const wchar_t * volatile nf = NULL;
    EXPECT_FAIL(swprintf_s(nb, 10, L"%f", 1.0));
    EXPECT_FAIL(swprintf_s(b, 10, nf));
}
TC_CASE(snwprintf_s_null)
{
    wchar_t b[10]; wchar_t * volatile nb = NULL; const wchar_t * volatile nf = NULL;
    if (!tc_p__snwprintf_s) TC_SKIP("_snwprintf_s not exported");
    EXPECT_FAIL(tc_p__snwprintf_s(nb, 10, 5, L"%f", 1.0));
    EXPECT_FAIL(tc_p__snwprintf_s(b, 10, 5, nf));
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_snwprintf_s);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(swprintf_s_null); TC_RUN(snwprintf_s_null);
    return tc_end();
}
