/* TC
 * id:    sec_neg_itoa_s_errors
 * area:  secure_crt
 * funcs: _itoa_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itoa_s: buffer too small -> ERANGE, NULL buffer/zero size -> EINVAL; invalid-parameter handler invoked

 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
static volatile int v = 123456;
TC_CASE(buffer_too_small_erange)
{
    char b[4] = "zzz"; int r;
    tc_iph_count = 0; errno = 0;
    r = _itoa_s(v, b, 4, 10);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buffer_einval)
{
    int r; tc_iph_count = 0; errno = 0;
    r = _itoa_s(v, NULL, 10, 10);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(zero_size_einval)
{
    char b[4] = "zzz"; int r; tc_iph_count = 0; errno = 0;
    r = _itoa_s(5, b, 0, 10);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_STR(b, "zzz");
}
TC_CASE(exact_minus_one_fails)
{
    char b[3]; int r; tc_iph_count = 0;
    v = 123; r = _itoa_s(v, b, 3, 10);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    v = 123456;
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(buffer_too_small_erange); TC_RUN(null_buffer_einval); TC_RUN(zero_size_einval); TC_RUN(exact_minus_one_fails); return tc_end(); }
