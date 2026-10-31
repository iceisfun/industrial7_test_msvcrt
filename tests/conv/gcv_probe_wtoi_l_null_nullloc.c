/* TC
 * id:    gcv_probe_wtoi_l_null_nullloc
 * area:  conv
 * funcs: _wtoi_l _wtol_l _wtoi64_l
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _wtoi, _wtoi_l (NULL str: invalid parameter handler, EINVAL, 0)
 * note:  PROBE: NULL string AND NULL locale: parameter validation runs (handler once, EINVAL, 0) rather than a crash from dereferencing the NULL locale.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const wchar_t *volatile NS = NULL;
#define EXPECT_FAIL(call, n) do { errno = 0; CHECK_EQ_I((call), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, (n)); } while (0)

TC_CASE(null_locale)
{
    tc_iph_count = 0;
    EXPECT_FAIL(_wtoi_l(NS, NULL), 1);
    EXPECT_FAIL(_wtol_l(NS, NULL), 2);
    EXPECT_FAIL(_wtoi64_l(NS, NULL), 3);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null_locale);
    return tc_end();
}
