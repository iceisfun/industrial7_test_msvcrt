/* TC
 * id:    gst_strtoi64_l_neg_invalid_input
 * area:  conv
 * funcs: _strtoi64_l _strtoui64_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtoll ("returns 0 if no conversion"; base outside 2..36 or NULL string: invalid parameter handler, errno EINVAL); Parameter validation
 * note:  errno after a plain no-conversion is intentionally not asserted (see probe tests).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

static const char *volatile v;
static _locale_t loc;

TC_CASE(no_conversion)
{
    char *e;
    v = "";    e = (char *)1; CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v);
    v = "abc"; e = (char *)1; CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v);
    v = "abc"; e = (char *)1; CHECK_EQ_U(_strtoui64_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v);
    v = "+";   e = (char *)1; CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v);
    v = "-";   e = (char *)1; CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v);
    v = "0x";  CHECK_EQ_I(_strtoi64_l(v, &e, 16, loc), 0);
}
TC_CASE(whitespace_only)
{
    char *e;
    v = " \t\n "; e = (char *)1; CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v);
    e = (char *)1; CHECK_EQ_U(_strtoui64_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v);
}
TC_CASE(bad_base)
{
    char *e; static const int bases[] = { 1, 37, -1, 99 }; int i;
    v = "123";
    for (i = 0; i < 4; i++) {
        tc_iph_count = 0; errno = 0; e = (char *)1;
        CHECK_EQ_I(_strtoi64_l(v, &e, bases[i], loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
        tc_iph_count = 0; errno = 0;
        CHECK_EQ_U(_strtoui64_l(v, &e, bases[i], loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    }
}
TC_CASE(null_string)
{
    char *e = (char *)1;
    const char *volatile n = NULL;
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_strtoi64_l(n, &e, 10, loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_U(_strtoui64_l(n, &e, 10, loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(unsigned_negation_wraps)
{
    char *e;
    v = "-1"; errno = 0; CHECK_EQ_U(_strtoui64_l(v, &e, 10, loc), 0xFFFFFFFFFFFFFFFFULL); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, v + 2);
    v = "-2"; errno = 0; CHECK_EQ_U(_strtoui64_l(v, &e, 10, loc), 0xFFFFFFFFFFFFFFFEULL); CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::no_iph\n"); return 1; }
    loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL);
    TC_RUN(no_conversion); TC_RUN(whitespace_only); TC_RUN(bad_base); TC_RUN(null_string); TC_RUN(unsigned_negation_wraps);
    _free_locale(loc);
    return tc_end();
}
