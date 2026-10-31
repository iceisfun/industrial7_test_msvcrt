/* TC
 * id:    err_underscore_strerror_prefix
 * area:  errno_param
 * funcs: _strerror strerror
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _strerror: strErrMsg + ': ' + message + newline, strErrMsg up to 94 chars
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static volatile int v_e;

TC_CASE(abc_einval)
{
    char want[128]; const char *r, *t; size_t n;
    v_e = EINVAL; t = strerror(v_e); CHECK(t != NULL); if (!t) return;
    strcpy(want, "abc: "); strcat(want, t); strcat(want, "\n");
    errno = EINVAL; r = _strerror("abc");
    CHECK(r != NULL); if (!r) return;
    CHECK_STR(r, want);
    n = strlen(r); CHECK(n > 0 && r[n - 1] == '\n');
}
TC_CASE(prefix_94)
{
    char pre[100], want[400]; const char *r, *t; size_t n;
    memset(pre, 'p', 94); pre[94] = 0;
    v_e = ENOENT; t = strerror(v_e); CHECK(t != NULL); if (!t) return;
    strcpy(want, pre); strcat(want, ": "); strcat(want, t); strcat(want, "\n");
    errno = ENOENT; r = _strerror(pre);
    CHECK(r != NULL); if (!r) return;
    n = strlen(r); CHECK(n > 0 && r[n - 1] == '\n');
    CHECK(strncmp(r, pre, 94) == 0);
    CHECK_STR(r, want);
}
int main(void) { tc_begin(); TC_RUN(abc_einval); TC_RUN(prefix_94); return tc_end(); }
