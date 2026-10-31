/* TC
 * id:    dir_getdcwd_alloc_small_maxlen
 * area:  dir
 * funcs: _getdcwd _wgetdcwd _getdrive _getcwd
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _getdcwd: "If buffer is NULL, _getdcwd allocates a buffer of at least maxlen size (more only if necessary)"
 * note:  NULL buffer with maxlen smaller than the path succeeds with a heap string (caller frees); no ERANGE.
 */
#include "tc.h"
#include <direct.h>
#include <stdlib.h>
#include <errno.h>

static volatile int v_n;
static size_t slen(const char *s) { size_t n = 0; while (s[n]) n++; return n; }

TC_CASE(null_small)
{
    char ref[MAX_PATH]; char *p;
    _getcwd(ref, MAX_PATH);
    p = _getdcwd(0 + v_n, NULL, 2 + v_n);
    CHECK(p != NULL);
    if (p) { CHECK_STR(p, ref); free(p); }
}
TC_CASE(null_exact_no_nul)
{
    char ref[MAX_PATH]; char *p;
    _getcwd(ref, MAX_PATH);
    p = _getdcwd(_getdrive(), NULL, (int)slen(ref) + v_n);
    CHECK(p != NULL);
    if (p) { CHECK_STR(p, ref); free(p); }
}
TC_CASE(null_len_plus1_control)
{
    char ref[MAX_PATH]; char *p;
    _getcwd(ref, MAX_PATH);
    p = _getdcwd(_getdrive(), NULL, (int)slen(ref) + 1 + v_n);
    CHECK(p != NULL);
    if (p) { CHECK_STR(p, ref); free(p); }
}
TC_CASE(wide_null_small)
{
    wchar_t ref[MAX_PATH]; wchar_t *p;
    GetCurrentDirectoryW(MAX_PATH, ref);
    p = _wgetdcwd(0 + v_n, NULL, 2 + v_n);
    CHECK(p != NULL);
    if (p) { CHECK_WSTR(p, ref); free(p); }
}

int main(void)
{
    tc_begin();
    TC_RUN(null_small); TC_RUN(null_exact_no_nul); TC_RUN(null_len_plus1_control); TC_RUN(wide_null_small);
    return tc_end();
}
