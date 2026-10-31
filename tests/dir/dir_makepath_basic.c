/* TC
 * id:    dir_makepath_basic
 * area:  dir
 * funcs: _makepath
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _makepath, _wmakepath (inserts ':' after drive, '\' after dir and '.' before ext where missing)
 */
#include "tc.h"
#include <stdlib.h>

static const char *vs(const char *s) { static volatile const char *v; v = s; return s ? (const char *)v : NULL; }
static char *mk(char *b, const char *d, const char *di, const char *f, const char *e)
{ _makepath(b, d, di, f, e); return b; }

TC_CASE(drive_colon)   { char b[_MAX_PATH]; CHECK_STR(mk(b, vs("C"), vs("\\dir\\"), vs("file"), vs(".ext")), "C:\\dir\\file.ext"); }
TC_CASE(drive_with_colon) { char b[_MAX_PATH]; CHECK_STR(mk(b, vs("C:"), vs("\\dir\\"), vs("file"), vs(".ext")), "C:\\dir\\file.ext"); }
TC_CASE(dir_slash)     { char b[_MAX_PATH]; CHECK_STR(mk(b, vs("C:"), vs("\\dir"), vs("file"), vs(".ext")), "C:\\dir\\file.ext"); }
TC_CASE(ext_dot)       { char b[_MAX_PATH]; CHECK_STR(mk(b, vs("C:"), vs("\\dir\\"), vs("file"), vs("ext")), "C:\\dir\\file.ext"); }
TC_CASE(all_null)      { char b[_MAX_PATH]; b[0] = 'X'; CHECK_STR(mk(b, NULL, NULL, NULL, NULL), ""); }
TC_CASE(no_drive_dir)
{
    char b[_MAX_PATH];
    CHECK_STR(mk(b, NULL, NULL, vs("file"), vs(".ext")), "file.ext");
    CHECK_STR(mk(b, vs(""), vs(""), vs("file"), vs("ext")), "file.ext");
}
TC_CASE(long_259)
{
    char b[_MAX_PATH], dir[110], fn[160], want[_MAX_PATH]; int i, n = 0;
    dir[0] = '\\'; for (i = 1; i <= 100; i++) dir[i] = 'd'; dir[101] = '\\'; dir[102] = 0;
    for (i = 0; i < 151; i++) fn[i] = 'f'; fn[151] = 0;
    want[n++] = 'C'; want[n++] = ':';
    for (i = 0; dir[i]; i++) want[n++] = dir[i];
    for (i = 0; fn[i]; i++) want[n++] = fn[i];
    want[n++] = '.'; want[n++] = 'e'; want[n++] = 'x'; want[n++] = 't'; want[n] = 0;
    CHECK_EQ_I(n, 259);
    CHECK_STR(mk(b, vs("C:"), dir, fn, vs(".ext")), want);
}
int main(void)
{
    tc_begin(); TC_RUN(drive_colon); TC_RUN(drive_with_colon); TC_RUN(dir_slash); TC_RUN(ext_dot);
    TC_RUN(all_null); TC_RUN(no_drive_dir); TC_RUN(long_259);
    return tc_end();
}
