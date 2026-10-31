/* TC
 * id:    dir_splitpath_basic
 * area:  dir
 * funcs: _splitpath
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _splitpath, _wsplitpath (extension begins at the last '.' in the filename part)
 */
#include "tc.h"
#include <stdlib.h>

static void chk(const char *path, const char *d, const char *dir, const char *f, const char *e)
{
    char dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
    volatile const char *p = path;
    _splitpath((const char *)p, dr, di, fn, ex);
    CHECK_STR(dr, d); CHECK_STR(di, dir); CHECK_STR(fn, f); CHECK_STR(ex, e);
}
TC_CASE(full)        { chk("C:\\dir\\sub\\file.ext", "C:", "\\dir\\sub\\", "file", ".ext"); }
TC_CASE(name_only)   { chk("file", "", "", "file", ""); }
TC_CASE(dir_only)    { chk("dir\\", "", "dir\\", "", ""); }
TC_CASE(drive_rel)   { chk("C:file.txt", "C:", "", "file", ".txt"); }
TC_CASE(drive_root)  { chk("C:\\", "C:", "\\", "", ""); }
TC_CASE(multi_dot)   { chk("a.b.c", "", "", "a.b", ".c"); }
TC_CASE(hidden)      { chk(".hidden", "", "", "", ".hidden"); }
TC_CASE(trailing_dot){ chk("file.", "", "", "file", "."); }
int main(void)
{
    tc_begin(); TC_RUN(full); TC_RUN(name_only); TC_RUN(dir_only); TC_RUN(drive_rel); TC_RUN(drive_root);
    TC_RUN(multi_dot); TC_RUN(hidden); TC_RUN(trailing_dot);
    return tc_end();
}
