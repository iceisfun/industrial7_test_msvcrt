/* TC
 * id:    cv_neg_atoi_l_null_locale_probe
 * area:  conv
 * funcs: _atoi_l _atol_l _atoi64_l _atof_l _strtol_l _strtod_l
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _atoi_l family ("locale parameter"; NULL is believed to mean the current locale)
 * note:  PROBES: (1) a NULL _locale_t is accepted as "current locale" and yields the same value as the non-_l function with no invalid-parameter call;
 *        (2) _strtol_l(NULL, NULL end, 10, NULL) returns 0, sets EINVAL and fires the handler exactly once.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v_s;

TC_CASE(null_locale_accepted)
{
    int n0 = tc_iph_count; char *e;
    v_s = "  -123xyz";
    CHECK_EQ_I(_atoi_l(v_s, NULL), atoi(v_s));
    CHECK_EQ_I(_atol_l(v_s, NULL), atol(v_s));
    CHECK_EQ_I(_atoi64_l(v_s, NULL), _atoi64(v_s));
    CHECK_EQ_I(_strtol_l(v_s, &e, 10, NULL), -123);
    v_s = "2.5e2z";
    CHECK_BITS_D(_atof_l(v_s, NULL), 250.0);
    CHECK_BITS_D(_strtod_l(v_s, &e, NULL), 250.0);
    CHECK_EQ_I(tc_iph_count - n0, 0);
}
TC_CASE(strtol_l_null_string)
{
    int n0 = tc_iph_count; long r; errno = 0;
    r = _strtol_l(NULL, NULL, 10, NULL);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - n0, 1);
}

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(null_locale_accepted); TC_RUN(strtol_l_null_string);
    return tc_end();
}
