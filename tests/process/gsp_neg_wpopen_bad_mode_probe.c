/* TC
 * id:    gsp_neg_wpopen_bad_mode_probe
 * area:  process
 * funcs: _wpopen
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _popen, _wpopen: mode is "r" or "w", optionally with "t" or "b"
 * note:  OBSERVED on Win10 22H2 (19045): _wpopen with mode "x" or "" returns NULL/EINVAL/one handler call, but mode "rw" is
 *        ACCEPTED (non-NULL stream, pclose == 0): the CRT only validates the first character plus the t/b/ flags it knows, not
 *        a contradictory r+w. The earlier expectation that "rw" is rejected was wrong. Stable.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const wchar_t *volatile wmode;

static void accepted(const wchar_t *m)
{
    FILE *volatile f;
    wmode = m; errno = 0;
    f = wpopen(L"cmd /c exit 0", wmode);
    CHECK(f != NULL);                       /* observed: accepted (only the first char selects r/w, the rest is scanned leniently) */
    if (f) CHECK_EQ_I(pclose(f), 0);
}
static void bad(const wchar_t *m)
{
    FILE *volatile f;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0; errno = 0; wmode = m;
    f = wpopen(L"cmd /c exit 0", wmode);
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (f) pclose(f);
}
TC_CASE(mode_x)     { bad(L"x"); }
TC_CASE(mode_empty) { bad(L""); }
TC_CASE(mode_rw_accepted) { accepted(L"rw"); }
int main(void) { tc_begin(); TC_RUN(mode_x); TC_RUN(mode_empty); TC_RUN(mode_rw_accepted); return tc_end(); }
