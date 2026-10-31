/* TC
 * id:    dir_getcwd_alloc_small_maxlen
 * area:  dir
 * funcs: _getcwd _wgetcwd
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _getcwd: "If buffer is NULL, _getcwd allocates a buffer of at least maxlen size (more only if necessary)"
 * note:  The NULL-buffer form does NOT fail with ERANGE when maxlen is smaller than the cwd; the allocation grows as needed.
 *        (The negative ERANGE behaviour applies to caller-supplied buffers: see dir_neg_getcwd_exact_no_nul.)
 */
#include "tc.h"
#include <direct.h>
#include <stdlib.h>
#include <errno.h>

static volatile int v_n;
static size_t slen(const char *s) { size_t n = 0; while (s[n]) n++; return n; }

TC_CASE(null_maxlen_2)
{
    char ref[MAX_PATH]; char *p;
    GetCurrentDirectoryA(MAX_PATH, ref);
    p = _getcwd(NULL, 2 + v_n);
    CHECK(p != NULL);
    if (p) { CHECK_STR(p, ref); free(p); }
}
TC_CASE(null_maxlen_len_no_nul)
{
    char ref[MAX_PATH]; char *p;
    GetCurrentDirectoryA(MAX_PATH, ref);
    p = _getcwd(NULL, (int)slen(ref) + v_n);
    CHECK(p != NULL);
    if (p) { CHECK_STR(p, ref); free(p); }
}
TC_CASE(null_maxlen_len_plus1)
{
    char ref[MAX_PATH]; char *p;
    GetCurrentDirectoryA(MAX_PATH, ref);
    p = _getcwd(NULL, (int)slen(ref) + 1 + v_n);
    CHECK(p != NULL);
    if (p) { CHECK_STR(p, ref); free(p); }
}
TC_CASE(wide_null_maxlen_2)
{
    wchar_t ref[MAX_PATH]; wchar_t *p;
    GetCurrentDirectoryW(MAX_PATH, ref);
    p = _wgetcwd(NULL, 2 + v_n);
    CHECK(p != NULL);
    if (p) { CHECK_WSTR(p, ref); free(p); }
}

int main(void)
{
    tc_begin();
    TC_RUN(null_maxlen_2); TC_RUN(null_maxlen_len_no_nul); TC_RUN(null_maxlen_len_plus1); TC_RUN(wide_null_maxlen_2);
    return tc_end();
}
