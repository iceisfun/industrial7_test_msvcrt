/* TC
 * id:    dir_neg_chdrive_state_after_fail_probe
 * area:  dir
 * kind:  negative
 * funcs: _chdrive _getdrive _getdcwd _getcwd _chdir
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _chdrive, _chdir, _getdrive
 * note:  Probes that the current drive and current directory are unchanged after a failed _chdrive(27)
 *        (invalid parameter, -1) and after a failed _chdir to a missing directory. getdrive_range skips if
 *        _getdrive() returns 0 (documented for an undeterminable drive, e.g. cwd on a UNC share).
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static volatile int v_drive = 27;
static const char *volatile v_missing = "tc_dir_neg_chdrive_state_nope";

TC_CASE(chdrive_27)
{
    char a[MAX_PATH], b[MAX_PATH], da[MAX_PATH], db[MAX_PATH]; int d0, d1, r;
    d0 = _getdrive(); CHECK(_getcwd(a, MAX_PATH) != NULL); CHECK(_getdcwd(d0, da, MAX_PATH) != NULL);
    tc_iph_count = 0;
    r = _chdrive(v_drive);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
    d1 = _getdrive(); CHECK(_getcwd(b, MAX_PATH) != NULL); CHECK(_getdcwd(d0, db, MAX_PATH) != NULL);
    CHECK_EQ_I(d1, d0); CHECK_STR(b, a); CHECK_STR(db, da);
}
TC_CASE(chdir_missing)
{
    char a[MAX_PATH], b[MAX_PATH]; int d0, d1;
    d0 = _getdrive(); CHECK(_getcwd(a, MAX_PATH) != NULL);
    errno = 0;
    CHECK_EQ_I(_chdir(v_missing), -1);
    d1 = _getdrive(); CHECK(_getcwd(b, MAX_PATH) != NULL);
    CHECK_EQ_I(d1, d0); CHECK_STR(b, a);
}
TC_CASE(getdrive_range)
{
    int d = _getdrive();
    if (d == 0) { TC_SKIP("_getdrive returned 0 (e.g. UNC cwd)"); return; }
    CHECK(d >= 1 && d <= 26);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 1; }
    TC_RUN(chdrive_27); TC_RUN(chdir_missing); TC_RUN(getdrive_range);
    return tc_end();
}
