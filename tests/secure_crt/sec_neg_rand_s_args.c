/* TC
 * id:    sec_neg_rand_s_args
 * area:  secure_crt
 * funcs: rand_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs rand_s: randomValue NULL -> EINVAL, invalid-parameter handler
 */
#define MINGW_HAS_SECURE_API 1
#define _CRT_RAND_S
#include "tc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
TC_CASE(null_EINVAL) { errno_t r; tc_iph_count = 0; errno = 0; r = rand_s(NULL); CHECK_EQ_I(r, EINVAL); }
TC_CASE(handler_count_one) { tc_iph_count = 0; rand_s(NULL); CHECK(tc_iph_count >= 1); }
TC_CASE(errno_EINVAL) { errno = 0; rand_s(NULL); CHECK_EQ_I(errno, EINVAL); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(null_EINVAL); TC_RUN(handler_count_one); TC_RUN(errno_EINVAL);
    return tc_end();
}
