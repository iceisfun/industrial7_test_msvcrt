/* TC
 * id:    gst_strto_neg_errno_no_digits_probe
 * area:  conv
 * funcs: _strtoi64_l _create_locale _free_locale
 * procs: _strtof_l
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.4 (errno unspecified for no-conversion); MS docs strtol (EINVAL listed for invalid input)
 * note:  Probes errno after a no-digits conversion. Asserted guess: errno is left unchanged (preset sentinel 12345 survives) for both _strtoi64_l("abc") and _strtof_l("abc"). Real Windows may instead set EINVAL.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

TC_DECL_PROC(float, _strtof_l, (const char *, char **, _locale_t));
static _locale_t loc;
static const char *volatile v;

TC_CASE(strtoi64_l_abc) { char *e; errno = 12345; v = "abc"; (void)_strtoi64_l(v, &e, 10, loc); CHECK_EQ_I(errno, 12345); }
TC_CASE(strtof_l_abc)
{
    char *e;
    if (!tc_p__strtof_l) TC_SKIP("_strtof_l missing");
    errno = 12345; v = "abc"; (void)tc_p__strtof_l(v, &e, loc); CHECK_EQ_I(errno, 12345);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_strtof_l);
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(strtoi64_l_abc); TC_RUN(strtof_l_abc);
    _free_locale(loc);
    return tc_end();
}
