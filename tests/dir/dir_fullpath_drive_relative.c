/* TC
 * id:    dir_fullpath_drive_relative
 * area:  dir
 * funcs: _fullpath _getdcwd _getdrive
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   GetFullPathName semantics, wrapped by _fullpath
 * note:  Assumes no =X: environment override differing from the process cwd.
 */
#include "tc.h"
#include <direct.h>
#include <stdlib.h>
#include <errno.h>

static volatile int v_zero;
static size_t slen(const char *s) { size_t n = 0; while (s[n]) n++; return n; }
static void cat(char *d, const char *a, const char *b) { size_t n = slen(a), i = 0; for (i = 0; i < n; i++) d[i] = a[i]; for (i = 0; b[i]; i++) d[n + i] = b[i]; d[n + i] = 0; }
/* cwd + "\\" + tail (no extra backslash when cwd is a root) */
static void under_cwd(char *d, const char *tail)
{
    char c[MAX_PATH]; size_t n; _getcwd(c, MAX_PATH); n = slen(c);
    cat(d, c, (n && c[n - 1] == 0x5c) ? "" : "\\");
    cat(d, d, tail);
}
static char drv(void) { return (char)(0x40 + _getdrive()); }

TC_CASE(drive_colon_file)
{
    char in[] = "X:file", out[MAX_PATH], d[MAX_PATH], w[MAX_PATH]; char *p;
    in[0] = drv();
    CHECK(_getdcwd(_getdrive(), d, MAX_PATH) != NULL);
    cat(w, d, (d[slen(d) - 1] == 0x5c) ? "file" : "\\file");
    p = _fullpath(out, in, MAX_PATH);
    CHECK_EQ_P(p, out);
    if (p) CHECK_STR(out, w);
}
TC_CASE(root_relative)
{
    char out[MAX_PATH], w[] = "X:\\file"; char *p; w[0] = drv();
    p = _fullpath(out, "\\file", MAX_PATH);
    CHECK_EQ_P(p, out);
    if (p) CHECK_STR(out, w);
}
int main(void) { tc_begin(); TC_RUN(drive_colon_file); TC_RUN(root_relative); return tc_end(); }
