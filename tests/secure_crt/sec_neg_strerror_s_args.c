/* TC
 * id:    sec_neg_strerror_s_args
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: strerror_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs strerror_s: EINVAL if buffer is NULL or sizeInBytes is 0
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *se_t)(char *, size_t, int);
static se_t p_se;

TC_CASE(strerror_s_null_buffer) { int r; tc_iph_count = 0; r = p_se(NULL, 100, 22); CHECK_EQ_I(r, 22); CHECK(tc_iph_count >= 1); }
TC_CASE(strerror_s_size_zero) { char b[8] = "x"; int r; tc_iph_count = 0; r = p_se(b, 0, 22); CHECK_EQ_I(r, 22); CHECK(tc_iph_count >= 1); CHECK_EQ_I(b[0], 'x'); }
int main(void)
{
    tc_begin();
    p_se = (se_t)tc_crt_proc("strerror_s");
    CHECK(p_se); if (!p_se) return tc_end();
    tc_iph_install();
    TC_RUN(strerror_s_null_buffer); TC_RUN(strerror_s_size_zero);
    return tc_end();
}
