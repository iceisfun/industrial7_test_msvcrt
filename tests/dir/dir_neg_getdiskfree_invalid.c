/* TC
 * id:    dir_neg_getdiskfree_invalid
 * area:  dir
 * kind:  negative
 * funcs: _getdiskfree _getdrives
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _getdiskfree ("returns zero if successful, otherwise a nonzero error value")
 * note:  Exact error code and the contents of the struct after failure are not asserted. An invalid-parameter handler is
 *        installed where available in case the CRT validates the drive number.
 */
#include "tc.h"
#include <direct.h>

TC_CASE(absent_drive)
{
    struct _diskfree_t df; unsigned long m = _getdrives(); int d, pick = 0;
    tc_iph_install();
    for (d = 26; d >= 1; d--) if (!((m >> (d - 1)) & 1)) { pick = d; break; }
    if (!pick) TC_SKIP("all drive letters in use");
    CHECK(_getdiskfree(pick, &df) != 0);
}
TC_CASE(drive_27)
{
    struct _diskfree_t df; volatile int d = 27;
    tc_iph_install();
    CHECK(_getdiskfree(d, &df) != 0);
}
int main(void) { tc_begin(); TC_RUN(absent_drive); TC_RUN(drive_27); return tc_end(); }
