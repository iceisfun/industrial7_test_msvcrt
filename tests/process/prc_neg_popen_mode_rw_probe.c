/* TC
 * id:    prc_neg_popen_mode_rw_probe
 * area:  process
 * funcs: _popen
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _popen (only r/w with t/b described)
 * note:  Observed on Win10 22H2 19045 ucrtbase: _popen with mode "rw" calls the invalid-parameter handler once but still
 *        returns a usable stream (not NULL, not rejected as the old guess had it). The stream is closed with _pclose.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
static const char *volatile vmode;
TC_CASE(mode_rw)
{
    FILE *f;
    tc_iph_install(); tc_iph_count = 0; errno = 0; vmode = "rw";
    f = _popen("cmd /c exit 0", vmode);
    CHECK(f != NULL);              /* observed: "rw" is NOT rejected (the first letter decides, trailing chars are ignored) */
    CHECK_EQ_I(tc_iph_count, 1);   /* observed: the handler IS called once, yet a stream is still returned */
    CHECK_EQ_I(errno, EINVAL);
    if (f) CHECK_EQ_I(_pclose(f), 0);
}
int main(void) { tc_begin(); TC_RUN(mode_rw); return tc_end(); }
