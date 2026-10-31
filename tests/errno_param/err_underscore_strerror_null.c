/* TC
 * id:    err_underscore_strerror_null
 * area:  errno_param
 * funcs: _strerror strerror
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _strerror: NULL strErrMsg gives the message for the current errno
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static volatile int v_e;
static char ref[260];
static void keep(int e) { const char *s; v_e = e; s = strerror(v_e); ref[0] = 0; if (s) { size_t n = strlen(s); if (n > 200) n = 200; memcpy(ref, s, n); ref[n] = 0; } }
static int starts(const char *s, const char *p) { return strncmp(s, p, strlen(p)) == 0; }

TC_CASE(enoent)
{
    const char *r; keep(ENOENT); errno = ENOENT; r = _strerror(NULL);
    CHECK(r != NULL); CHECK(ref[0] != 0);
    if (r) CHECK(starts(r, ref));
}
TC_CASE(follows_errno)
{
    const char *r; char a[260]; size_t n;
    keep(EINVAL); errno = EINVAL; r = _strerror(NULL);
    CHECK(r != NULL);
    if (r) { CHECK(starts(r, ref)); n = strlen(r); if (n > 255) n = 255; memcpy(a, r, n); a[n] = 0; }
    else a[0] = 0;
    keep(EACCES); errno = EACCES; r = _strerror(NULL);
    CHECK(r != NULL);
    if (r) { CHECK(starts(r, ref)); CHECK(strcmp(r, a) != 0); }
}
int main(void) { tc_begin(); TC_RUN(enoent); TC_RUN(follows_errno); return tc_end(); }
