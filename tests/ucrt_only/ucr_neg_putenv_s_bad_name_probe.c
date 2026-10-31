/* TC
 * id:    ucr_neg_putenv_s_bad_name_probe
 * area:  ucrt_only
 * funcs: _putenv_s _wputenv_s
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _putenv_s
 * note:  PROBE: an empty name, or a name containing '=', makes _putenv_s/_wputenv_s return EINVAL (iph count not asserted).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(empty_name)   { volatile int r; r = _putenv_s("", "v"); CHECK_EQ_I(r, EINVAL); }
TC_CASE(wempty_name)  { volatile int r; r = _wputenv_s(L"", L"v"); CHECK_EQ_I(r, EINVAL); }
TC_CASE(eq_in_name)   { volatile int r; r = _putenv_s("TC_UCR_NB=B", "v"); CHECK_EQ_I(r, EINVAL); CHECK(getenv("TC_UCR_NB") == NULL); _putenv_s("TC_UCR_NB", ""); }
TC_CASE(weq_in_name)  { volatile int r; r = _wputenv_s(L"TC_UCR_NB=B", L"v"); CHECK_EQ_I(r, EINVAL); _wputenv_s(L"TC_UCR_NB", L""); }
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(empty_name); TC_RUN(wempty_name); TC_RUN(eq_in_name); TC_RUN(weq_in_name);
    return tc_end();
}
