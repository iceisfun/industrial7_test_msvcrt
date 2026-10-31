/* TC
 * id:    dir_fullpath_basic
 * area:  dir
 * funcs: _fullpath _getcwd _getdrive
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _fullpath
 * note:  Drive letter taken from _getdrive so the test is machine independent.
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
TC_CASE(relative_file) { char w[MAX_PATH]; under_cwd(w, "file.txt"); chk("file.txt", w); }
TC_CASE(dot_dotdot) { char w[MAX_PATH]; under_cwd(w, "b"); chk(".\\a\\..\\b", w); }
TC_CASE(two_up) { char w[MAX_PATH]; under_cwd(w, "c"); chk("a\\b\\..\\..\\c", w); }
TC_CASE(forward_slashes)
{
    char in[] = "X:/x/y", want[] = "X:\\x\\y"; in[0] = want[0] = drv();
    chk(in, want);
}
TC_CASE(abs_dot)
{
    char in[] = "X:\\a\\.\\b", want[] = "X:\\a\\b"; in[0] = want[0] = drv();
    chk(in, want);
}
TC_CASE(null_buffer_allocs)
{
    char w[MAX_PATH]; char *p; under_cwd(w, "zz");
    p = _fullpath(NULL, "zz", 0 + v_zero);
    CHECK(p != NULL);
    if (p) { CHECK_STR(p, w); free(p); }
}
TC_CASE(trailing_backslash)
{
    char in[] = "X:\\a\\b\\", want[] = "X:\\a\\b\\"; in[0] = want[0] = drv();
    chk(in, want);
}
int main(void)
{
    tc_begin();
    TC_RUN(relative_file); TC_RUN(dot_dotdot); TC_RUN(two_up); TC_RUN(forward_slashes); TC_RUN(abs_dot);
    TC_RUN(null_buffer_allocs); TC_RUN(trailing_backslash);
    return tc_end();
}
