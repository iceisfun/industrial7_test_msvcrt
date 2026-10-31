/* TC
 * id:    sec_splitpath_s_basic
 * area:  secure_crt
 * funcs: _splitpath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _splitpath_s, _wsplitpath_s

 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
static char dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
static int sp(const char *p) { return _splitpath_s(p, dr, sizeof dr, di, sizeof di, fn, sizeof fn, ex, sizeof ex); }

TC_CASE(full_path_drive_dir_fname_ext)
{
    CHECK_EQ_I(sp("C:\\dir\\sub\\file.txt"), 0);
    CHECK_STR(dr, "C:"); CHECK_STR(di, "\\dir\\sub\\"); CHECK_STR(fn, "file"); CHECK_STR(ex, ".txt");
}
TC_CASE(forward_slashes)
{
    CHECK_EQ_I(sp("C:/a/b/c.txt"), 0);
    CHECK_STR(dr, "C:"); CHECK_STR(di, "/a/b/"); CHECK_STR(fn, "c"); CHECK_STR(ex, ".txt");
}
TC_CASE(no_drive)
{
    CHECK_EQ_I(sp("\\a\\b.c"), 0);
    CHECK_STR(dr, ""); CHECK_STR(di, "\\a\\"); CHECK_STR(fn, "b"); CHECK_STR(ex, ".c");
}
TC_CASE(no_ext)
{
    CHECK_EQ_I(sp("C:\\a\\b"), 0);
    CHECK_STR(dr, "C:"); CHECK_STR(di, "\\a\\"); CHECK_STR(fn, "b"); CHECK_STR(ex, "");
}
TC_CASE(filename_only)
{
    CHECK_EQ_I(sp("file.txt"), 0);
    CHECK_STR(dr, ""); CHECK_STR(di, ""); CHECK_STR(fn, "file"); CHECK_STR(ex, ".txt");
}
TC_CASE(trailing_backslash_dir_only)
{
    CHECK_EQ_I(sp("C:\\a\\b\\"), 0);
    CHECK_STR(dr, "C:"); CHECK_STR(di, "\\a\\b\\"); CHECK_STR(fn, ""); CHECK_STR(ex, "");
}
TC_CASE(null_components_with_zero_size)
{
    CHECK_EQ_I(_splitpath_s("C:\\a\\b.c", NULL, 0, NULL, 0, fn, sizeof fn, NULL, 0), 0);
    CHECK_STR(fn, "b");
    CHECK_EQ_I(_splitpath_s("C:\\a\\b.c", dr, sizeof dr, NULL, 0, NULL, 0, ex, sizeof ex), 0);
    CHECK_STR(dr, "C:"); CHECK_STR(ex, ".c");
}
int main(void) { tc_begin(); TC_RUN(full_path_drive_dir_fname_ext); TC_RUN(forward_slashes); TC_RUN(no_drive); TC_RUN(no_ext); TC_RUN(filename_only);
    TC_RUN(trailing_backslash_dir_only); TC_RUN(null_components_with_zero_size); return tc_end(); }
