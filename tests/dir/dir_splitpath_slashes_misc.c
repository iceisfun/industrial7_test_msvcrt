/* TC
 * id:    dir_splitpath_slashes_misc
 * area:  dir
 * funcs: _splitpath
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _splitpath (forward slash and backslash both separators; output args may be NULL; _MAX_* sizes)
 */
#include "tc.h"
#include <stdlib.h>

static char big[1024];
static const char *vs(const char *s) { static volatile const char *v; v = s; return (const char *)v; }

TC_CASE(forward)
{
    char dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
    _splitpath(vs("/usr/local/x.c"), dr, di, fn, ex);
    CHECK_STR(dr, ""); CHECK_STR(di, "/usr/local/"); CHECK_STR(fn, "x"); CHECK_STR(ex, ".c");
}
TC_CASE(mixed)
{
    char dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
    _splitpath(vs("C:/a\\b.c"), dr, di, fn, ex);
    CHECK_STR(dr, "C:"); CHECK_STR(di, "/a\\"); CHECK_STR(fn, "b"); CHECK_STR(ex, ".c");
}
TC_CASE(null_outputs)
{
    char a[_MAX_DRIVE], b[_MAX_DIR], c[_MAX_FNAME], d[_MAX_EXT];
    const char *p = vs("C:\\d\\f.e");
    _splitpath(p, NULL, b, c, d);   CHECK_STR(b, "\\d\\"); CHECK_STR(c, "f"); CHECK_STR(d, ".e");
    _splitpath(p, a, NULL, c, d);   CHECK_STR(a, "C:"); CHECK_STR(c, "f"); CHECK_STR(d, ".e");
    _splitpath(p, a, b, NULL, d);   CHECK_STR(a, "C:"); CHECK_STR(b, "\\d\\"); CHECK_STR(d, ".e");
    _splitpath(p, a, b, c, NULL);   CHECK_STR(a, "C:"); CHECK_STR(b, "\\d\\"); CHECK_STR(c, "f");
}
TC_CASE(max_sizes)
{
    char dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
    char wdir[_MAX_DIR], wfn[_MAX_FNAME];
    int i, n = 0;
    big[n++] = 'C'; big[n++] = ':';
    wdir[0] = '\\'; big[n++] = '\\';
    for (i = 0; i < 253; i++) { big[n++] = 'd'; wdir[1 + i] = 'd'; }
    big[n++] = '\\'; wdir[254] = '\\'; wdir[255] = 0;
    for (i = 0; i < 255; i++) { big[n++] = 'f'; wfn[i] = 'f'; }
    wfn[255] = 0;
    big[n++] = '.'; big[n++] = 'e'; big[n++] = 'x'; big[n++] = 't'; big[n] = 0;
    _splitpath(big, dr, di, fn, ex);
    CHECK_STR(dr, "C:"); CHECK_STR(di, wdir); CHECK_STR(fn, wfn); CHECK_STR(ex, ".ext");
}
TC_CASE(empty)
{
    char dr[_MAX_DRIVE], di[_MAX_DIR], fn[_MAX_FNAME], ex[_MAX_EXT];
    dr[0] = di[0] = fn[0] = ex[0] = 'X';
    _splitpath(vs(""), dr, di, fn, ex);
    CHECK_STR(dr, ""); CHECK_STR(di, ""); CHECK_STR(fn, ""); CHECK_STR(ex, "");
}
int main(void)
{
    tc_begin(); TC_RUN(forward); TC_RUN(mixed); TC_RUN(null_outputs); TC_RUN(max_sizes); TC_RUN(empty);
    return tc_end();
}
