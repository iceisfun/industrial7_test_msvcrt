/* TC
 * id:    dir_neg_chdrive_invalid
 * area:  dir
 * kind:  negative
 * funcs: _chdrive _getdrive _getdrives
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _chdrive (drive 1..26; returns -1 on failure)
 * note:  Only the -1 return is asserted (errno EACCES is probed in dir_neg_probe_chdrive_iph). For a well-formed but absent drive only the -1 return is
 *        asserted. See dir_neg_probe_chdrive_iph for iph counting of the range check.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

TC_CASE(out_of_range)
{
    static const int bad[] = { 0, 27, -5 }; int i;
    int d0 = _getdrive();
    tc_iph_install();
    for (i = 0; i < 3; i++) {
        volatile int v = bad[i]; int r;
        r = _chdrive(v);
        CHECK_EQ_I(r, -1);
    }
    CHECK_EQ_I(_getdrive(), d0);
}
TC_CASE(absent_drive)
{
    unsigned long m = _getdrives(); int d, pick = 0, d0 = _getdrive();
    tc_iph_install();
    for (d = 26; d >= 1; d--) if (!((m >> (d - 1)) & 1)) { pick = d; break; }
    if (!pick) TC_SKIP("all drive letters in use");
    CHECK_EQ_I(_chdrive(pick), -1);
    CHECK_EQ_I(_getdrive(), d0);
}
int main(void) { tc_begin(); TC_RUN(out_of_range); TC_RUN(absent_drive); return tc_end(); }
