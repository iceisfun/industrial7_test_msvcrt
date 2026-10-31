/* TC
 * id:    str_neg_strlwr_s_l_null_locale_probe
 * area:  string
 * funcs:
 * procs: _strlwr_s_l _strupr_s_l _strlwr_l _strupr_l _strnicmp_l
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strlwr_l/_strupr_l/_strnicmp_l (locale parameter; NULL not discussed)
 * note:  PROBE: a NULL _locale_t passed to the _l string functions means "use the current locale" (UCRT _LocaleUpdate treats a null locale pointer as the global locale) and succeeds with no handler call. Alternative on real Windows: EINVAL via the invalid-parameter handler (INT_MAX from _strnicmp_l, NULL from _strlwr_l). Test runs in the C locale.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <limits.h>
TC_DECL_PROC(int, _strlwr_s_l, (char *, size_t, void *));
TC_DECL_PROC(int, _strupr_s_l, (char *, size_t, void *));
TC_DECL_PROC(char *, _strlwr_l, (char *, void *));
TC_DECL_PROC(char *, _strupr_l, (char *, void *));
TC_DECL_PROC(int, _strnicmp_l, (const char *, const char *, size_t, void *));

TC_CASE(lwr_s_l)
{
    char b[8]; strcpy(b, "AbC"); tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(tc_p__strlwr_s_l(b, sizeof b, NULL), 0); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(upr_s_l)
{
    char b[8]; strcpy(b, "AbC"); tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(tc_p__strupr_s_l(b, sizeof b, NULL), 0); CHECK_STR(b, "ABC"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(lwr_l)
{
    char b[8]; strcpy(b, "AbC"); tc_iph_count = 0; errno = 0;
    CHECK(tc_p__strlwr_l(b, NULL) == b); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(upr_l)
{
    char b[8]; strcpy(b, "AbC"); tc_iph_count = 0; errno = 0;
    CHECK(tc_p__strupr_l(b, NULL) == b); CHECK_STR(b, "ABC"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(nicmp_l)
{
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(tc_p__strnicmp_l("A", "a", 1, NULL), 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_LOAD_PROC(_strlwr_s_l); TC_LOAD_PROC(_strupr_s_l); TC_LOAD_PROC(_strlwr_l); TC_LOAD_PROC(_strupr_l); TC_LOAD_PROC(_strnicmp_l);
    if (!tc_p__strlwr_s_l || !tc_p__strupr_s_l || !tc_p__strlwr_l || !tc_p__strupr_l || !tc_p__strnicmp_l)
    { tc_write("TC FAIL " TC_TEST_ID "::__import missing _l function\n"); return 1; }
    TC_RUN(lwr_s_l); TC_RUN(upr_s_l); TC_RUN(lwr_l); TC_RUN(upr_l); TC_RUN(nicmp_l);
    return tc_end();
}
