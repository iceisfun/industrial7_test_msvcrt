/* TC
 * id:    env_neg_putenv_empty_name
 * area:  env
 * kind:  negative
 * funcs: _putenv _wputenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   none (empty variable name is not tabulated in MS docs)
 * note:  PROBE: _putenv("=x"), _putenv("") and wide equivalents. Expect -1 / EINVAL; whether the invalid-parameter
 *        handler fires is not asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static char    *volatile v_eqx  = "=x";
static char    *volatile v_empty = "";
static wchar_t *volatile w_eqx  = L"=x";
static wchar_t *volatile w_empty = L"";

TC_CASE(putenv_eq_x_returns_minus1_EINVAL)
{
    tc_iph_install(); errno = 0;
    CHECK_EQ_I(_putenv(v_eqx), -1);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(putenv_empty_returns_minus1_EINVAL)
{
    tc_iph_install(); errno = 0;
    CHECK_EQ_I(_putenv(v_empty), -1);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(wide_equivalents)
{
    tc_iph_install(); errno = 0;
    CHECK_EQ_I(_wputenv(w_eqx), -1);
    CHECK_EQ_I(errno, EINVAL);
    errno = 0;
    CHECK_EQ_I(_wputenv(w_empty), -1);
    CHECK_EQ_I(errno, EINVAL);
}

int main(void)
{
    tc_begin();
    TC_RUN(putenv_eq_x_returns_minus1_EINVAL);
    TC_RUN(putenv_empty_returns_minus1_EINVAL);
    TC_RUN(wide_equivalents);
    return tc_end();
}
