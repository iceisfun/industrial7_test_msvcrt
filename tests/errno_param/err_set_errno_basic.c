/* TC
 * id:    err_set_errno_basic
 * area:  errno_param
 * funcs: _errno
 * procs: _set_errno _get_errno
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_errno, _get_errno
 */
#include "tc.h"
#include <errno.h>

typedef errno_t (__cdecl *fn_t)(int);
typedef errno_t (__cdecl *gfn_t)(int *);
static fn_t p_set; static gfn_t p_get;

TC_CASE(exported) { CHECK(p_set != NULL); CHECK(p_get != NULL); }
TC_CASE(sets)
{
    int v = -1;
    if (!p_set || !p_get) TC_SKIP("missing");
    errno = 0;
    CHECK_EQ_I(p_set(ERANGE), 0); CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(p_get(&v), 0); CHECK_EQ_I(v, ERANGE);
    CHECK_EQ_I(p_set(0), 0); CHECK_EQ_I(errno, 0);
    errno = EINVAL; CHECK_EQ_I(p_set(EDOM), 0); CHECK_EQ_I(errno, EDOM);
    errno = 0;
}
int main(void) { tc_begin(); p_set = (fn_t)tc_crt_proc("_set_errno"); p_get = (gfn_t)tc_crt_proc("_get_errno"); TC_RUN(exported); TC_RUN(sets); return tc_end(); }
