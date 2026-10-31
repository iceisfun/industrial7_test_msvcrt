/* TC
 * id:    dir_getcwd_alloc
 * area:  dir
 * funcs: _getcwd _wgetcwd
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _getcwd, _wgetcwd
 * note:  buffer NULL: allocates at least maxlen; caller frees with free()
 */
#include "tc.h"
#include <direct.h>
#include <stdlib.h>
#include <errno.h>

static volatile int v_zero;

static void cur(char *b) { DWORD n = GetCurrentDirectoryA(MAX_PATH, b); if (!n || n >= MAX_PATH) b[0] = 0; }
static size_t slen(const char *s) { size_t n = 0; while (s[n]) n++; return n; }

TC_CASE(null_zero)
{
    char ref[MAX_PATH]; char *p; cur(ref);
    p = _getcwd(NULL, 0 + v_zero);
    CHECK(p != NULL);
    if (p) { CHECK_STR(p, ref); free(p); }
}
TC_CASE(null_300)
{
    char ref[MAX_PATH]; char *p; cur(ref);
    p = _getcwd(NULL, 300 + v_zero);
    CHECK(p != NULL);
    if (p) { CHECK_STR(p, ref); free(p); }
}
TC_CASE(buf_maxpath)
{
    char ref[MAX_PATH], buf[MAX_PATH]; char *p; cur(ref);
    p = _getcwd(buf, MAX_PATH + v_zero);
    CHECK_EQ_P(p, buf);
    CHECK_STR(buf, ref);
}
TC_CASE(wide_null)
{
    wchar_t ref[MAX_PATH]; wchar_t *p; DWORD n = GetCurrentDirectoryW(MAX_PATH, ref);
    CHECK(n > 0 && n < MAX_PATH);
    p = _wgetcwd(NULL, 0 + v_zero);
    CHECK(p != NULL);
    if (p) { CHECK_WSTR(p, ref); free(p); }
}
TC_CASE(exact_fit)
{
    char ref[MAX_PATH], buf[MAX_PATH]; char *p; size_t n; cur(ref); n = slen(ref);
    p = _getcwd(buf, (int)(n + 1));
    CHECK_EQ_P(p, buf);
    CHECK_STR(buf, ref);
}
int main(void)
{
    tc_begin();
    TC_RUN(null_zero); TC_RUN(null_300); TC_RUN(buf_maxpath); TC_RUN(wide_null); TC_RUN(exact_fit);
    return tc_end();
}
