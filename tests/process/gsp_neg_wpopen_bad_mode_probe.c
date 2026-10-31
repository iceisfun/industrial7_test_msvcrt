/* TC
 * id:    gsp_neg_wpopen_bad_mode_probe
 * area:  process
 * funcs: _wpopen
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _popen, _wpopen: mode is "r" or "w", optionally with "t" or "b"
 * note:  PROBE: rejection of mode strings outside that grammar (NULL, EINVAL, invalid-parameter handler once) is expected but
 *        not spelled out. Strings: "x", "", "rw". The command is never run if rejection works; if a CRT accepted a mode
 *        the returned stream is closed with pclose (command is a quick `cmd /c exit 0`).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const wchar_t *volatile wmode;

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
TC_CASE(mode_rw)    { bad(L"rw"); }
int main(void) { tc_begin(); TC_RUN(mode_x); TC_RUN(mode_empty); TC_RUN(mode_rw); return tc_end(); }
