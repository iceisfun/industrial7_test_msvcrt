/* TC
 * id:    tm_neg_strdate_strtime_s_probe_size8
 * area:  time
 * funcs:
 * procs: _strdate_s _strtime_s _wstrdate_s _wstrtime_s
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strdate_s
 * note:  PROBES which error code is returned for a too-small (size 8) buffer: ERANGE (believed, the "buffer too small"
 *        convention, and what the old CRT sources do) rather than EINVAL (which the MS docs error table may list for
 *        0 < size < 9). Real Windows decides. Only the code is asserted; failure itself is asserted in the *_s_args tests.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

TC_DECL_PROC(int, _strdate_s, (char *, size_t));
TC_DECL_PROC(int, _strtime_s, (char *, size_t));
TC_DECL_PROC(int, _wstrdate_s, (wchar_t *, size_t));
TC_DECL_PROC(int, _wstrtime_s, (wchar_t *, size_t));

TC_CASE(strdate_s) { char b[16]; if (!tc_p__strdate_s) TC_SKIP("missing"); CHECK_EQ_I(tc_p__strdate_s(b, 8), ERANGE); }
TC_CASE(strtime_s) { char b[16]; if (!tc_p__strtime_s) TC_SKIP("missing"); CHECK_EQ_I(tc_p__strtime_s(b, 8), ERANGE); }
TC_CASE(wstrdate_s) { wchar_t b[16]; if (!tc_p__wstrdate_s) TC_SKIP("missing"); CHECK_EQ_I(tc_p__wstrdate_s(b, 8), ERANGE); }
TC_CASE(wstrtime_s) { wchar_t b[16]; if (!tc_p__wstrtime_s) TC_SKIP("missing"); CHECK_EQ_I(tc_p__wstrtime_s(b, 8), ERANGE); }
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_LOAD_PROC(_strdate_s); TC_LOAD_PROC(_strtime_s); TC_LOAD_PROC(_wstrdate_s); TC_LOAD_PROC(_wstrtime_s);
    TC_RUN(strdate_s); TC_RUN(strtime_s); TC_RUN(wstrdate_s); TC_RUN(wstrtime_s);
    return tc_end();
}
