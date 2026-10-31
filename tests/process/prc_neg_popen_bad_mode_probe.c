/* TC
 * id:    prc_neg_popen_bad_mode_probe
 * area:  process
 * funcs: _popen
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _popen (only r/w with t/b described)
 * note:  PROBE: whether _popen validates the mode string and rejects "x" and "" with NULL/EINVAL. Not documented explicitly.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
static const char *volatile vmode;
static void bad(const char *m)
{
    FILE *f;
    errno = 0; vmode = m;
    f = _popen("cmd /c exit 0", vmode);
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EINVAL);
    if (f) _pclose(f);
}
TC_CASE(mode_x)  { tc_iph_install(); bad("x"); }
TC_CASE(mode_empty) { tc_iph_install(); bad(""); }
int main(void) { tc_begin(); TC_RUN(mode_x); TC_RUN(mode_empty); return tc_end(); }
