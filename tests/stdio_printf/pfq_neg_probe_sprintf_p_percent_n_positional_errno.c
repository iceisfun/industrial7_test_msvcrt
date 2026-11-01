/* TC
 * id:    pfq_neg_probe_sprintf_p_percent_n_positional_errno
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _sprintf_p, printf_s (failure sets errno)
 * note:  OBSERVED for case too_small on ucrtbase (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "too_small errno != 0"; zzp_probe2 "_sprintf_p small %1$s: r=-2 errno=0 iph=0"):
 *        _sprintf_p into a too-small buffer returns -2, leaves errno unchanged and does not call the handler; the other three failure classes
 *        (NULL buffer, NULL format, position gap) still set errno (EINVAL, handler once) as originally probed. Old expectation kept for pre-UCRT
 *        CRTs (unverified). Stable.
 *        NOTE: -2 is the raw ucrtbase __stdio_common_v*printf_p export result (observed, not documented); MSVC headers map any negative result to -1.
 */
#include "tc.h"
#include "pfq_shim.h"
#include <errno.h>

static char * volatile Nbuf = NULL;
static const char * volatile Nfmt = NULL;
static const char * volatile Fgap = "%1$d%3$d", * volatile Fbig = "%1$s", * volatile Fok = "%1$d";

TC_CASE(null_buf)
{
    int r; errno = 0; tc_iph_count = 0;
    r = pq_sprintf_p(Nbuf, 16, Fok, 1);
    CHECK(r < 0); CHECK(errno != 0);
}
TC_CASE(null_fmt)
{
    char b[16]; int r; errno = 0; tc_iph_count = 0;
    r = pq_sprintf_p(b, sizeof b, Nfmt, 1);
    CHECK(r < 0); CHECK(errno != 0);
}
TC_CASE(gap)
{
    char b[16]; int r; errno = 0; tc_iph_count = 0;
    r = pq_sprintf_p(b, sizeof b, Fgap, 1, 2, 3);
    CHECK(r < 0); CHECK(errno != 0);
}
TC_CASE(too_small)
{
    char b[4]; int r; errno = 0; tc_iph_count = 0;
    r = pq_sprintf_p(b, sizeof b, Fbig, "abcdefgh");
#if TC_CRT_VER >= 140
    CHECK_EQ_I(r, -2); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);   /* ucrtbase observed: legacy truncation result, errno untouched */
#else
    CHECK(r < 0); CHECK(errno != 0);
#endif
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_buf); TC_RUN(null_fmt); TC_RUN(gap); TC_RUN(too_small);
    return tc_end();
}
