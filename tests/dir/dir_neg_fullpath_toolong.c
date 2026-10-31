/* TC
 * id:    dir_neg_fullpath_toolong
 * area:  dir
 * kind:  negative
 * funcs: _fullpath _wfullpath
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _fullpath, _wfullpath ("if the length of the absolute path exceeds maxLength, returns NULL and sets errno to ERANGE")
 * note:  The NULL-buffer (alloc mode) case with a path longer than MAX_PATH is a lenient probe: it may return NULL or the
 *        full string; only the content is asserted when non-NULL. No assertion on NULL-versus-success.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static char lp[400]; static wchar_t wlp[400];
static void mk(void)
{
    int i = 0, c;
    for (c = 0; c < 3; c++) {
        int n = c == 2 ? 98 : 100, k;
        for (k = 0; k < n; k++) { lp[i] = (char)('a' + c); wlp[i] = (wchar_t)('a' + c); i++; }
        if (c < 2) { lp[i] = '\\'; wlp[i] = L'\\'; i++; }
    }
    lp[i] = 0; wlp[i] = 0;
}
static int prefix(const char *s, const char *p) { while (*p) { if (*s++ != *p++) return 0; } return 1; }
static int wprefix(const wchar_t *s, const wchar_t *p) { while (*p) { if (*s++ != *p++) return 0; } return 1; }

TC_CASE(erange_small_buffer)
{
    char buf[MAX_PATH]; char *r; volatile const char *p = lp;
    errno = 0;
    r = _fullpath(buf, (const char *)p, MAX_PATH);
    CHECK(r == NULL);
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(erange_small_buffer_wide)
{
    wchar_t buf[MAX_PATH]; wchar_t *r; volatile const wchar_t *p = wlp;
    errno = 0;
    r = _wfullpath(buf, (const wchar_t *)p, MAX_PATH);
    CHECK(r == NULL);
    CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(alloc_mode_lenient)
{
    volatile const char *p = lp; char cwd[MAX_PATH]; char *r;
    GetCurrentDirectoryA(MAX_PATH, cwd);
    r = _fullpath(NULL, (const char *)p, 0);
    if (r) { CHECK(prefix(r, cwd)); free(r); }
}
TC_CASE(big_buffer_control)
{
    static char buf[1000]; char cwd[MAX_PATH]; char *r; volatile const char *p = lp;
    GetCurrentDirectoryA(MAX_PATH, cwd);
    r = _fullpath(buf, (const char *)p, 1000);
    CHECK(r == buf);
    if (r) CHECK(prefix(buf, cwd));
}
TC_CASE(big_buffer_control_wide)
{
    static wchar_t buf[1000]; wchar_t cwd[MAX_PATH]; wchar_t *r; volatile const wchar_t *p = wlp;
    GetCurrentDirectoryW(MAX_PATH, cwd);
    r = _wfullpath(buf, (const wchar_t *)p, 1000);
    CHECK(r == buf);
    if (r) CHECK(wprefix(buf, cwd));
}
int main(void)
{
    tc_begin(); mk();
    TC_RUN(erange_small_buffer); TC_RUN(erange_small_buffer_wide); TC_RUN(alloc_mode_lenient);
    TC_RUN(big_buffer_control); TC_RUN(big_buffer_control_wide);
    return tc_end();
}
