/* TC
 * id:    sec_neg_strcpy_s_size0_probe
 * area:  secure_crt
 * kind:  negative
 * funcs: strcpy_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs strcpy_s (error table lists dest_size 0 as ERANGE with dest[0] set to 0)
 * note:  PROBE: with dest_size == 0 the docs table says ERANGE and dest[0]=0, but the implementation is believed to
 *        validate (dest != NULL && size > 0) first, returning EINVAL and leaving dest untouched. Asserts the
 *        believed-real behaviour (EINVAL, dest unchanged, errno EINVAL, one handler call).
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static volatile size_t v_zero;
TC_CASE(size_zero_returns_EINVAL_dest_untouched)
{
    char d[4] = "xyz"; int r; tc_iph_count = 0; errno = 0;
    r = strcpy_s(d, v_zero, "a");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_STR(d, "xyz");
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(size_zero_returns_EINVAL_dest_untouched);
    return tc_end();
}
