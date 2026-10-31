/* TC
 * id:    ct_neg_create_locale_errno_probe
 * area:  ctype
 * funcs: _errno
 * procs: _create_locale _free_locale
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _create_locale
 * note:  PROBE: errno is left unchanged (0) by a failed _create_locale with a well-formed but unknown locale name
 *        (docs say nothing about errno; a bad name is not an invalid parameter). Most likely value, not EINVAL.
 */
#include "tc.h"
#include <errno.h>

typedef void *(__cdecl *cl_t)(int, const char *);
typedef void (__cdecl *fl_t)(void *);
static cl_t p_cl; static fl_t p_fl;

TC_CASE(errno_unchanged_on_bad_name)
{
    void *l; errno = 0; l = p_cl(0, "no_such_locale_xyz");
    CHECK(l == NULL); CHECK_EQ_I(errno, 0); if (l) p_fl(l);
}
int main(void)
{
    tc_begin();
    p_cl = (cl_t)tc_crt_proc("_create_locale"); p_fl = (fl_t)tc_crt_proc("_free_locale");
    if (!p_cl || !p_fl) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(errno_unchanged_on_bad_name);
    return tc_end();
}
