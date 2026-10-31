/* TC
 * id:    exp_neg_o_strcpy_s_family_errors
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: _o_strcpy_s _o_strcat_s _o_wcscpy_s _o_wcscat_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs strcpy_s, strcat_s, wcscpy_s (EINVAL/ERANGE, dest[0] set to 0, invalid parameter handler invoked)
 * note:  The _o_ exports are the API-set (no-locale) forms of the secure string functions and must keep the secure-CRT error contract. conf is inferred because the _o_ exports themselves are undocumented; the contract asserted is the documented one of the public functions.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *cpy_t)(char *, size_t, const char *);
typedef int (__cdecl *wcpy_t)(wchar_t *, size_t, const wchar_t *);
static cpy_t p_cpy, p_cat; static wcpy_t p_wcpy, p_wcat;

TC_CASE(cpy_null_and_zero)
{
    char buf[8]; int before;
    if (!p_cpy) { CHECK(p_cpy != NULL); return; }
    tc_iph_count = 0;
    CHECK_EQ_I(p_cpy(NULL, 8, "a"), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    buf[0] = 'x'; before = tc_iph_count;
    CHECK_EQ_I(p_cpy(buf, 0, "a"), EINVAL);
    CHECK_EQ_I(tc_iph_count, before + 1);
    buf[0] = 'x'; before = tc_iph_count;
    CHECK_EQ_I(p_cpy(buf, 8, NULL), EINVAL);
    CHECK_EQ_I(buf[0], 0);
    CHECK_EQ_I(tc_iph_count, before + 1);
}
TC_CASE(cpy_range)
{
    char buf[8]; int before = tc_iph_count;
    if (!p_cpy) { CHECK(p_cpy != NULL); return; }
    buf[0] = 'x';
    CHECK_EQ_I(p_cpy(buf, 2, "abc"), ERANGE);
    CHECK_EQ_I(buf[0], 0);
    CHECK_EQ_I(tc_iph_count, before + 1);
}
TC_CASE(cpy_success_no_handler)
{
    char buf[8]; int before = tc_iph_count;
    if (!p_cpy) { CHECK(p_cpy != NULL); return; }
    CHECK_EQ_I(p_cpy(buf, 8, "abc"), 0);
    CHECK_STR(buf, "abc");
    CHECK_EQ_I(tc_iph_count, before);
}
TC_CASE(cat_errors)
{
    char buf[4]; int before = tc_iph_count;
    if (!p_cat) { CHECK(p_cat != NULL); return; }
    buf[0] = 'a'; buf[1] = 'b'; buf[2] = 'c'; buf[3] = 'd';  /* no terminator within size */
    CHECK_EQ_I(p_cat(buf, 4, "x"), EINVAL);
    CHECK_EQ_I(tc_iph_count, before + 1);
    before = tc_iph_count;
    buf[0] = 'x'; buf[1] = 'y'; buf[2] = 0;
    CHECK_EQ_I(p_cat(buf, 4, "abc"), ERANGE);
    CHECK_EQ_I(buf[0], 0);
    CHECK_EQ_I(tc_iph_count, before + 1);
}
TC_CASE(wcs_errors)
{
    wchar_t buf[8]; int before = tc_iph_count;
    if (!p_wcpy || !p_wcat) { CHECK(p_wcpy && p_wcat); return; }
    CHECK_EQ_I(p_wcpy(NULL, 8, L"a"), EINVAL);
    CHECK_EQ_I(tc_iph_count, before + 1);
    before = tc_iph_count; buf[0] = L'x';
    CHECK_EQ_I(p_wcpy(buf, 2, L"abc"), ERANGE);
    CHECK_EQ_I(buf[0], 0);
    CHECK_EQ_I(tc_iph_count, before + 1);
    before = tc_iph_count; buf[0] = L'x'; buf[1] = L'y'; buf[2] = 0;
    CHECK_EQ_I(p_wcat(buf, 4, L"abc"), ERANGE);
    CHECK_EQ_I(tc_iph_count, before + 1);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    p_cpy = (cpy_t)tc_crt_proc("_o_strcpy_s"); p_cat = (cpy_t)tc_crt_proc("_o_strcat_s");
    p_wcpy = (wcpy_t)tc_crt_proc("_o_wcscpy_s"); p_wcat = (wcpy_t)tc_crt_proc("_o_wcscat_s");
    TC_RUN(cpy_null_and_zero); TC_RUN(cpy_range); TC_RUN(cpy_success_no_handler);
    TC_RUN(cat_errors); TC_RUN(wcs_errors);
    return tc_end();
}
