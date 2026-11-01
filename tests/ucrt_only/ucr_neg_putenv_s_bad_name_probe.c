/* TC
 * id:    ucr_neg_putenv_s_bad_name_probe
 * area:  ucrt_only
 * funcs: _putenv_s _wputenv_s
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _putenv_s
 * note:  Win10 22H2 (observed): _putenv_s/_wputenv_s return EINVAL for an empty name, but an '=' inside the name is NOT rejected (returns 0 and stores
 *        "NAME=B=v"; getenv("NAME") then yields "B=v"). The old probe guessed EINVAL for both. Handler count not asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(empty_name)   { volatile int r; r = _putenv_s("", "v"); CHECK_EQ_I(r, EINVAL); }
TC_CASE(wempty_name)  { volatile int r; r = _wputenv_s(L"", L"v"); CHECK_EQ_I(r, EINVAL); }
/* observed on Win10 22H2: '=' inside the name is NOT rejected: the entry "TC_UCR_NB=B=v" is stored (so getenv("TC_UCR_NB") sees "B=v") */
TC_CASE(eq_in_name_not_rejected)  { volatile int r; const char *g; r = _putenv_s("TC_UCR_NB=B", "v"); CHECK_EQ_I(r, 0); g = getenv("TC_UCR_NB"); CHECK_STR(g, "B=v"); }
TC_CASE(weq_in_name_not_rejected) { volatile int r; r = _wputenv_s(L"TC_UCR_NB2=B", L"v"); CHECK_EQ_I(r, 0); CHECK_WSTR(_wgetenv(L"TC_UCR_NB2"), L"B=v"); }
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(empty_name); TC_RUN(wempty_name); TC_RUN(eq_in_name_not_rejected); TC_RUN(weq_in_name_not_rejected);
    return tc_end();
}
