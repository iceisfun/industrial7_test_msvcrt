/* TC
 * id:    gst_strtoll_l_neg_range_and_base
 * area:  conv
 * funcs: _strtoll_l _strtoull_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtoll, strtoull (ERANGE clamp; invalid base / NULL string: invalid parameter handler, EINVAL, returns 0)
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

#ifndef _strtoll_l
extern long long __cdecl _strtoll_l(const char *, char **, int, _locale_t);
extern unsigned long long __cdecl _strtoull_l(const char *, char **, int, _locale_t);
#endif
#define LLMIN (-9223372036854775807LL - 1)

static const char *volatile v;
static _locale_t loc;

TC_CASE(overflow)
{
    char *e;
    v = "9223372036854775808"; errno = 0; CHECK_EQ_I(_strtoll_l(v, &e, 10, loc), 9223372036854775807LL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 19);
    v = "-9223372036854775809"; errno = 0; CHECK_EQ_I(_strtoll_l(v, &e, 10, loc), LLMIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 20);
    v = "18446744073709551616"; errno = 0; CHECK_EQ_U(_strtoull_l(v, &e, 10, loc), 0xFFFFFFFFFFFFFFFFULL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v + 20);
    v = "0x1FFFFFFFFFFFFFFFF"; errno = 0; CHECK_EQ_U(_strtoull_l(v, &e, 16, loc), 0xFFFFFFFFFFFFFFFFULL); CHECK_EQ_I(errno, ERANGE);
    errno = 0;
}
TC_CASE(bad_base)
{
    char *e; static const int bases[] = { 1, 37 }; int i;
    v = "123";
    for (i = 0; i < 2; i++) {
        tc_iph_count = 0; errno = 0;
        CHECK_EQ_I(_strtoll_l(v, &e, bases[i], loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
        tc_iph_count = 0; errno = 0;
        CHECK_EQ_U(_strtoull_l(v, &e, bases[i], loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    }
}
TC_CASE(null_nptr)
{
    char *e = (char *)1; const char *volatile n = NULL;
    tc_iph_count = 0; errno = 0; CHECK_EQ_I(_strtoll_l(n, &e, 10, loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; CHECK_EQ_U(_strtoull_l(n, &e, 10, loc), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(no_digits)
{
    char *e;
    v = "hello"; e = (char *)1; CHECK_EQ_I(_strtoll_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v);
    e = (char *)1; CHECK_EQ_U(_strtoull_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v);
    v = ""; e = (char *)1; CHECK_EQ_I(_strtoll_l(v, &e, 10, loc), 0); CHECK_EQ_P(e, v);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::no_iph\n"); return 1; }
    loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL);
    TC_RUN(overflow); TC_RUN(bad_base); TC_RUN(null_nptr); TC_RUN(no_digits);
    _free_locale(loc);
    return tc_end();
}
