/* TC
 * id:    sec_neg_tmpnam_s_args
 * area:  secure_crt
 * kind:  negative
 * funcs:
 * procs: tmpnam_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs tmpnam_s: EINVAL if str is NULL (invalid-parameter handler invoked)
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *tmpnam_s_t)(char *, size_t);
static tmpnam_s_t p_tmpnam_s;

TC_CASE(null_buffer) { int r; tc_iph_count = 0; r = p_tmpnam_s(NULL, 260); CHECK_EQ_I(r, 22); CHECK(tc_iph_count >= 1); }
int main(void)
{
    tc_begin();
    p_tmpnam_s = (tmpnam_s_t)tc_crt_proc("tmpnam_s");
    CHECK(p_tmpnam_s); if (!p_tmpnam_s) return tc_end();
    tc_iph_install();
    TC_RUN(null_buffer);
    return tc_end();
}
