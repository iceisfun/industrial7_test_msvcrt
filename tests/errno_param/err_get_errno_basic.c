/* TC
 * id:    err_get_errno_basic
 * area:  errno_param
 * funcs: _errno strtol
 * procs: _get_errno _set_errno
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _get_errno, _set_errno
 */
#include "tc.h"
#include <errno.h>
#include <stdlib.h>

typedef errno_t (__cdecl *geterrno_t)(int *);
static geterrno_t p_get;

TC_CASE(exported) { CHECK(p_get != NULL); }
TC_CASE(reads)
{
    int v = -1; errno_t r;
    if (!p_get) TC_SKIP("_get_errno missing");
    errno = EINVAL; r = p_get(&v); CHECK_EQ_I(r, 0); CHECK_EQ_I(v, EINVAL);
    errno = 0; v = -1; r = p_get(&v); CHECK_EQ_I(r, 0); CHECK_EQ_I(v, 0);
    errno = 0; { volatile const char *s = "99999999999999999999"; strtol((const char *)s, NULL, 10); }
    v = -1; r = p_get(&v); CHECK_EQ_I(r, 0); CHECK_EQ_I(v, ERANGE);
    CHECK_EQ_I(v, *_errno());
    errno = 0;
}
int main(void) { tc_begin(); p_get = (geterrno_t)tc_crt_proc("_get_errno"); TC_RUN(exported); TC_RUN(reads); return tc_end(); }
