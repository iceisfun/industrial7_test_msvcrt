/* TC
 * id:    sec_neg_rand_s_null
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: rand_s _errno
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs rand_s: EINVAL and invalid-parameter handler if randomValue is NULL
 */
#include "tc.h"

typedef int (__cdecl *rand_s_t)(unsigned int *);
typedef int *(__cdecl *errno_fn)(void);
static rand_s_t p_rand_s; static errno_fn p_errno;

TC_CASE(null_pointer_einval)
{
    int r; *p_errno() = 0; tc_iph_count = 0;
    r = p_rand_s(NULL);
    CHECK_EQ_I(r, 22); CHECK_EQ_I(*p_errno(), 22); CHECK(tc_iph_count >= 1);
}
TC_CASE(valid_call_still_works_after_error) { unsigned int v; CHECK_EQ_I(p_rand_s(&v), 0); }
int main(void)
{
    tc_begin();
    p_rand_s = (rand_s_t)tc_crt_proc("rand_s"); p_errno = (errno_fn)tc_crt_proc("_errno");
    CHECK(p_rand_s && p_errno); if (!(p_rand_s && p_errno)) return tc_end();
    tc_iph_install();
    TC_RUN(null_pointer_einval); TC_RUN(valid_call_still_works_after_error);
    return tc_end();
}
