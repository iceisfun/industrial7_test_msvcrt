/* TC
 * id:    gst_strtoi64_l_neg_null_locale_probe
 * area:  conv
 * funcs: _strtoi64_l _strtoui64_l
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtoll, _strtoll_l ("locale" parameter; NULL believed to mean current locale)
 * note:  PROBES: a NULL _locale_t is accepted as the current locale (value parsed, errno unchanged, no invalid-parameter call).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v = "123";

TC_CASE(null_locale_signed)
{
    char *e = NULL; long long r; errno = 0x33; tc_iph_count = 0;
    r = _strtoi64_l(v, &e, 10, NULL);
    CHECK_EQ_I(r, 123); CHECK_EQ_P(e, v + 3); CHECK_EQ_I(errno, 0x33); CHECK_EQ_I(tc_iph_count, 0);
    errno = 0;
}
TC_CASE(null_locale_unsigned)
{
    char *e = NULL; unsigned long long r; errno = 0x33; tc_iph_count = 0;
    r = _strtoui64_l(v, &e, 10, NULL);
    CHECK_EQ_U(r, 123); CHECK_EQ_P(e, v + 3); CHECK_EQ_I(errno, 0x33); CHECK_EQ_I(tc_iph_count, 0);
    errno = 0;
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(null_locale_signed); TC_RUN(null_locale_unsigned);
    return tc_end();
}
