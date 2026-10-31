/* TC
 * id:    dir_getdiskfree_basic
 * area:  dir
 * funcs: _getdiskfree _getdrive
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _getdiskfree (struct _diskfree_t; drive 0 = default, 1 = A:; returns 0 on success)
 * note:  Skips when the current directory has no drive letter.
 */
#include "tc.h"
#include <direct.h>

static int has_drive(void) { char c[MAX_PATH]; return GetCurrentDirectoryA(MAX_PATH, c) && c[1] == ':'; }

TC_CASE(current_drive)
{
    struct _diskfree_t df; volatile int d = _getdrive();
    if (!has_drive()) TC_SKIP("no drive letter");
    CHECK_EQ_U(_getdiskfree(d, &df), 0);
    CHECK(df.sectors_per_cluster != 0);
    CHECK(df.bytes_per_sector != 0);
    CHECK(df.total_clusters != 0);
    CHECK(df.avail_clusters <= df.total_clusters);
}
TC_CASE(default_drive)
{
    struct _diskfree_t df; volatile int d = 0;
    if (!has_drive()) TC_SKIP("no drive letter");
    CHECK_EQ_U(_getdiskfree(d, &df), 0);
    CHECK(df.sectors_per_cluster != 0);
    CHECK(df.bytes_per_sector != 0);
    CHECK(df.avail_clusters <= df.total_clusters);
}
int main(void) { tc_begin(); TC_RUN(current_drive); TC_RUN(default_drive); return tc_end(); }
