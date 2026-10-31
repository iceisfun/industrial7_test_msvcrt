/* TC
 * id:    sec_splitpath_s_dotfile_probe
 * area:  secure_crt
 * funcs: _splitpath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _splitpath_s (silent on these)
 * note:  PROBE: the CRT splits ext at the LAST '.' of the last component, even a leading one (.bashrc -> fname empty, ext .bashrc); and for UNC paths drive is empty; an empty path returns 0 with all four components empty (drive is only 'X:').
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
static char dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
static int sp(const char *p) { return _splitpath_s(p, dr, sizeof dr, di, sizeof di, fn, sizeof fn, ex, sizeof ex); }
TC_CASE(dotfile_fname_empty_ext_dotname) { CHECK_EQ_I(sp("C:\\d\\.bashrc"), 0); CHECK_STR(fn, ""); CHECK_STR(ex, ".bashrc"); }
TC_CASE(multiple_dots_ext_is_last) { CHECK_EQ_I(sp("a.b.c.txt"), 0); CHECK_STR(fn, "a.b.c"); CHECK_STR(ex, ".txt"); }
TC_CASE(trailing_dot_ext_is_dot) { CHECK_EQ_I(sp("name."), 0); CHECK_STR(fn, "name"); CHECK_STR(ex, "."); }
TC_CASE(unc_path_drive_empty)
{
    CHECK_EQ_I(sp("\\\\server\\share\\dir\\f.txt"), 0);
    CHECK_STR(dr, ""); CHECK_STR(di, "\\\\server\\share\\dir\\"); CHECK_STR(fn, "f"); CHECK_STR(ex, ".txt");
}
TC_CASE(empty_path_all_empty)
{
    dr[0] = di[0] = fn[0] = ex[0] = 'x';
    CHECK_EQ_I(sp(""), 0);
    CHECK_STR(dr, ""); CHECK_STR(di, ""); CHECK_STR(fn, ""); CHECK_STR(ex, "");
}
int main(void) { tc_begin(); TC_RUN(dotfile_fname_empty_ext_dotname); TC_RUN(multiple_dots_ext_is_last); TC_RUN(trailing_dot_ext_is_dot); TC_RUN(unc_path_drive_empty); TC_RUN(empty_path_all_empty); return tc_end(); }
