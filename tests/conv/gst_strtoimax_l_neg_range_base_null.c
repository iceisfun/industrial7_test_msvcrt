/* TC
 * id:    gst_strtoimax_l_neg_range_base_null
 * area:  conv
 * funcs: _strtoimax_l _strtoumax_l strtoumax _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtoimax/strtoumax (ERANGE clamp, invalid base / NULL: invalid parameter handler, EINVAL, returns 0); C11 7.22.1.4
 * note:  The "0x" with base 16 endptr question is in gst_strtoimax_l_neg_zero_x_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
extern intmax_t __cdecl _strtoimax_l(const char *, char **, int, _locale_t);
extern uintmax_t __cdecl _strtoumax_l(const char *, char **, int, _locale_t);

static const char *volatile v;
static _locale_t loc;

TC_CASE(overflow)
{
    char *e;
    v = "9223372036854775808"; errno = 0; CHECK_EQ_I(_strtoimax_l(v, &e, 10, loc), INTMAX_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 19);
    v = "-9223372036854775809"; errno = 0; CHECK_EQ_I(_strtoimax_l(v, &e, 10, loc), INTMAX_MIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 20);
    v = "18446744073709551616"; errno = 0; CHECK_EQ_U(strtoumax(v, &e, 10), UINTMAX_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 20);
    errno = 0; CHECK_EQ_U(_strtoumax_l(v, &e, 10, loc), UINTMAX_MAX); CHECK_EQ_I(errno, ERANGE);
    errno = 0;
}
TC_CASE(bad_base)
{
    char *e; static const int bases[] = { 1, 37 }; int i;
    v = "123";
    for (i = 0; i < 2; i++) {
        tc_iph_count = 0; errno = 0; CHECK_EQ_I(_strtoimax_l(v, &e, bases[i], loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
        tc_iph_count = 0; errno = 0; CHECK_EQ_U(_strtoumax_l(v, &e, bases[i], loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
        tc_iph_count = 0; errno = 0; CHECK_EQ_U(strtoumax(v, &e, bases[i]), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    }
}
TC_CASE(null_nptr)
{
    char *e = (char *)1; const char *volatile n = NULL;
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(_strtoimax_l(n, &e, 10, loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_EQ_U(_strtoumax_l(n, &e, 10, loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_EQ_U(strtoumax(n, &e, 10), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(garbage)
{
    char *e;
    v = "xyz"; e = (char *)1; CHECK_EQ_I(_strtoimax_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v);
    e = (char *)1; CHECK_EQ_U(_strtoumax_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v);
    e = (char *)1; CHECK_EQ_U(strtoumax(v, &e, 10), 0); CHECK_EQ_P(e, v);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::no_iph\n"); return 1; }
    loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL);
    TC_RUN(overflow); TC_RUN(bad_base); TC_RUN(null_nptr); TC_RUN(garbage);
    _free_locale(loc);
    return tc_end();
}
