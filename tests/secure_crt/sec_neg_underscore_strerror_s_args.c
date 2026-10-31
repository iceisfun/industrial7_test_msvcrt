/* TC
 * id:    sec_neg_underscore_strerror_s_args
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: _strerror_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _strerror_s: EINVAL if buffer is NULL or sizeInBytes is 0
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *se_t)(char *, size_t, int);
typedef int (__cdecl *use_t)(char *, size_t, const char *);
static se_t p_se; static use_t p_use;

TC_CASE(underscore_null_buffer) { int r; if (!p_use) TC_SKIP("_strerror_s not exported"); tc_iph_count = 0; r = p_use(NULL, 100, "p"); CHECK_EQ_I(r, 22); CHECK(tc_iph_count >= 1); }
TC_CASE(underscore_size_zero) { char b[8] = "x"; int r; if (!p_use) TC_SKIP("_strerror_s not exported"); tc_iph_count = 0; r = p_use(b, 0, "p"); CHECK_EQ_I(r, 22); CHECK(tc_iph_count >= 1); CHECK_EQ_I(b[0], 'x'); }
int main(void)
{
    tc_begin();
    p_use = (use_t)tc_crt_proc("_strerror_s");
    CHECK(p_use); if (!p_use) return tc_end();
    tc_iph_install();
    TC_RUN(underscore_null_buffer); TC_RUN(underscore_size_zero);
    return tc_end();
}
