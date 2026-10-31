/* TC
 * id:    gio_neg_probe_mktemp_s_buffer_state
 * area:  lowio
 * funcs:
 * procs: _mktemp_s
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mktemp_s (silent on buffer state after failure)
 * note:  PROBE: whether _mktemp_s sets template[0] to NUL on failure like many other secure-CRT functions, and
 *        that a zero-size call leaves the buffer untouched.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>

TC_DECL_PROC(int, _mktemp_s, (char *, size_t));

TC_CASE(size_too_small_buf0_is_nul)
{
    char b[16] = "abcXXXXXX";
    CHECK(tc_p__mktemp_s(b, 3) != 0);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(no_xs_buf0)
{
    char b[16] = "abc";
    CHECK(tc_p__mktemp_s(b, 4) != 0);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(size_zero_buf_untouched)
{
    char b[16] = "XXXXXX";
    CHECK(tc_p__mktemp_s(b, 0) != 0);
    CHECK_STR(b, "XXXXXX");
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_mktemp_s);
    CHECK(tc_p__mktemp_s);
    CHECK(tc_iph_install());
    if (tc_p__mktemp_s) { TC_RUN(size_too_small_buf0_is_nul); TC_RUN(no_xs_buf0); TC_RUN(size_zero_buf_untouched); }
    return tc_end();
}
