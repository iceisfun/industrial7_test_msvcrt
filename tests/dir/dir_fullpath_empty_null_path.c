/* TC
 * id:    dir_fullpath_empty_null_path
 * area:  dir
 * funcs: _fullpath _getcwd
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _fullpath; empty path handled as getcwd in the CRT implementation (not clearly documented)
 * note:  NULL path is probed separately in dir_probe_fullpath_null_path
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

TC_CASE(empty_is_cwd)
{
    char out[MAX_PATH], c[MAX_PATH]; char *p;
    _getcwd(c, MAX_PATH);
    p = _fullpath(out, "", MAX_PATH);
    CHECK_EQ_P(p, out);
    if (p) CHECK_STR(out, c);
}
int main(void) { tc_begin(); TC_RUN(empty_is_cwd); return tc_end(); }
