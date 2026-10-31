/* TC
 * id:    dir_splitpath_unc_probe
 * area:  dir
 * funcs: _splitpath
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _splitpath (only "X:" is recognised as a drive; no UNC handling documented)
 * note:  Probes that _splitpath does not treat a UNC server\share prefix, nor the \\?\ prefix, as a drive: the whole
 *        leading text up to the last separator is returned in dir. Probe-only test.
 */
#include "tc.h"
#include <stdlib.h>

static const char *vs(const char *s) { static volatile const char *v; v = s; return (const char *)v; }

TC_CASE(unc)
{
    char dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
    _splitpath(vs("\\\\srv\\share\\d\\f.txt"), dr, di, fn, ex);
    CHECK_STR(dr, ""); CHECK_STR(di, "\\\\srv\\share\\d\\"); CHECK_STR(fn, "f"); CHECK_STR(ex, ".txt");
}
TC_CASE(extended_prefix)
{
    char dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
    _splitpath(vs("\\\\?\\C:\\f.txt"), dr, di, fn, ex);
    CHECK_STR(dr, ""); CHECK_STR(di, "\\\\?\\C:\\"); CHECK_STR(fn, "f"); CHECK_STR(ex, ".txt");
}
int main(void) { tc_begin(); TC_RUN(unc); TC_RUN(extended_prefix); return tc_end(); }
