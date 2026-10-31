/* TC
 * id:    err_neg_probe_wcserror_s_unknown
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: _wcserror_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs strerror_s, _strerror_s, _wcserror_s, __wcserror_s
 * note:  PROBE: _wcserror_s with an unknown error number (9999, -1) in a large buffer. Most likely the call succeeds (returns 0),
 *        stores a non-empty generic "Unknown error" text, and the invalid-parameter handler does not run.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(errno_t, _wcserror_s, (wchar_t *, size_t, int));

static void one(int code)
{
    wchar_t b[128]; errno_t r; volatile int c = code; size_t i;
    for (i = 0; i < 128; i++) b[i] = 0x7777;
    tc_iph_count = 0;
    r = tc_p__wcserror_s(b, 128, c);
    CHECK_EQ_I(r, 0);
    CHECK(b[0] != 0 && b[0] != 0x7777);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(unknown_9999) { one(9999); }
TC_CASE(unknown_minus1) { one(-1); }

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_wcserror_s);
    if (!tc_p__wcserror_s || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(unknown_9999); TC_RUN(unknown_minus1);
    errno = 0;
    return tc_end();
}
