/* TC
 * id:    dir_fullpath_dotdot_root
 * area:  dir
 * funcs: _fullpath _getdrive
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   GetFullPathName semantics, wrapped by _fullpath
 * note:  Nonexistent paths; drive letter from _getdrive.
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

static void chk(const char *in, const char *want)
{
    char out[MAX_PATH]; char *p = _fullpath(out, in, MAX_PATH);
    CHECK_EQ_P(p, out);
    if (p) CHECK_STR(out, want);
}
TC_CASE(above_root) { char i[] = "X:\\..\\..\\x", w[] = "X:\\x"; i[0] = w[0] = drv(); chk(i, w); }
TC_CASE(to_root) { char i[] = "X:\\a\\..\\..", w[] = "X:\\"; i[0] = w[0] = drv(); chk(i, w); }
TC_CASE(unc) { chk("\\\\srv\\share\\a\\..\\b", "\\\\srv\\share\\b"); }
TC_CASE(trailing_dot) { char i[] = "X:\\a\\b\\.", w[] = "X:\\a\\b"; i[0] = w[0] = drv(); chk(i, w); }
int main(void)
{
    tc_begin();
    TC_RUN(above_root); TC_RUN(to_root); TC_RUN(unc); TC_RUN(trailing_dot);
    return tc_end();
}
