/* TC
 * id:    gst_strtoll_l_basic
 * area:  conv
 * funcs: _strtoll_l _strtoull_l _strtoi64_l _strtoui64_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strtoll, _strtoll_l, strtoull, _strtoull_l; C11 7.22.1.4
 * note: results are compared with _strtoi64_l.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

#ifndef _strtoll_l
extern long long __cdecl _strtoll_l(const char *, char **, int, _locale_t);
extern unsigned long long __cdecl _strtoull_l(const char *, char **, int, _locale_t);
#endif
#ifndef _strtoi64_l
extern __int64 __cdecl _strtoi64_l(const char *, char **, int, _locale_t);
extern unsigned __int64 __cdecl _strtoui64_l(const char *, char **, int, _locale_t);
#endif
#define LLMIN (-9223372036854775807LL - 1)

static const char *volatile v;
static _locale_t loc;

TC_CASE(parse_forms)
{
    char *e;
    v = "  -1234567890123"; CHECK_EQ_I(_strtoll_l(v, &e, 10, loc), -1234567890123LL); CHECK_EQ_P(e, v + 16);
    v = "0xdeadBEEF"; CHECK_EQ_I(_strtoll_l(v, &e, 16, loc), 0xdeadbeefLL); CHECK_EQ_P(e, v + 10);
    v = "0777"; CHECK_EQ_I(_strtoll_l(v, &e, 8, loc), 511);
    v = "0777"; CHECK_EQ_I(_strtoll_l(v, &e, 0, loc), 511);
    v = "0x10"; CHECK_EQ_U(_strtoull_l(v, &e, 0, loc), 16);
    v = "42";   CHECK_EQ_U(_strtoull_l(v, &e, 0, loc), 42);
    v = "zz";   CHECK_EQ_U(_strtoull_l(v, &e, 36, loc), 1295);
}
TC_CASE(limits)
{
    char *e;
    v = "9223372036854775807";  CHECK_EQ_I(_strtoll_l(v, &e, 10, loc), 9223372036854775807LL); CHECK_EQ_P(e, v + 19);
    v = "-9223372036854775808"; CHECK_EQ_I(_strtoll_l(v, &e, 10, loc), LLMIN); CHECK_EQ_P(e, v + 20);
    v = "18446744073709551615"; CHECK_EQ_U(_strtoull_l(v, &e, 10, loc), 0xFFFFFFFFFFFFFFFFULL); CHECK_EQ_P(e, v + 20);
}
TC_CASE(endptr)
{
    char *e;
    v = "55 rest"; CHECK_EQ_I(_strtoll_l(v, &e, 10, loc), 55); CHECK_EQ_P(e, v + 2);
    v = "12xyz";   CHECK_EQ_U(_strtoull_l(v, &e, 10, loc), 12); CHECK_EQ_P(e, v + 2);
}
TC_CASE(same_as_strtoi64_l)
{
    static const char *const tab[] = { "0", "-1", "  77z", "0x7fffffffffffffff", "-0x10", "4294967296", "9223372036854775807",
        "-9223372036854775808", "0777", "12abc", "", "xyz", "+5" };
    static const int bases[] = { 10, 16, 0, 8 };
    unsigned i, b;
    for (i = 0; i < sizeof tab / sizeof tab[0]; i++)
        for (b = 0; b < 4; b++) {
            char *e1, *e2; long long a, c; unsigned long long ua, uc;
            v = tab[i];
            a = _strtoll_l(v, &e1, bases[b], loc); c = _strtoi64_l(v, &e2, bases[b], loc);
            CHECK_EQ_I(a, c); CHECK_EQ_P(e1, e2);
            ua = _strtoull_l(v, &e1, bases[b], loc); uc = _strtoui64_l(v, &e2, bases[b], loc);
            CHECK_EQ_U(ua, uc); CHECK_EQ_P(e1, e2);
        }
}
TC_CASE(whitespace_chars)
{
    char *e;
    v = "\t\n\v\f\r 99x"; CHECK_EQ_I(_strtoll_l(v, &e, 10, loc), 99); CHECK_EQ_P(e, v + 8);
    CHECK_EQ_U(_strtoull_l(v, &e, 10, loc), 99);
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL);
    TC_RUN(parse_forms); TC_RUN(limits); TC_RUN(endptr); TC_RUN(same_as_strtoi64_l); TC_RUN(whitespace_chars);
    _free_locale(loc);
    return tc_end();
}
