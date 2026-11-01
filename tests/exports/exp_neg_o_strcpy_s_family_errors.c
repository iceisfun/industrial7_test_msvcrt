/* TC
 * id:    exp_neg_o_strcpy_s_family_errors
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: _o_strcpy_s _o_strcat_s _o_wcscpy_s _o_wcscat_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs strcpy_s, strcat_s, wcscpy_s (EINVAL/ERANGE, dest[0] set to 0, invalid parameter handler invoked)
 * note:  The _o_ exports are the API-set (no-locale) forms of the secure string functions and must keep the secure-CRT error contract. conf is inferred because the _o_ exports themselves are undocumented; the contract asserted is the documented one of the public functions.
 * note:  DUAL STATE (Win10 22H2 19045 ucrtbase; analysis: every _o_ export runs inside __crt_state_management::wrapped_invoke, which switches to the
 *        separate OS-state errno and invalid-parameter handler; audit probe: _o__read(9999) -> app errno 0 / *_o__errno()=9 / OS handler 1). So the APP-state
 *        errno and handler (errno, _set_invalid_parameter_handler) stay untouched (asserted == 0 here) while the error contract is visible in the OS state
 *        (_o__errno, _o__set_invalid_parameter_handler), which is asserted with the plain-function values. Deterministic.
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
    tc_iph_count = 0; tc_o_reset();
    CHECK_EQ_I(p_cpy(NULL, 8, "a"), EINVAL);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(tc_oiph_count, 1); CHECK_EQ_I(tc_o_errno(), EINVAL);
    buf[0] = 'x'; before = tc_iph_count; tc_o_reset();
    CHECK_EQ_I(p_cpy(buf, 0, "a"), EINVAL);
    CHECK_EQ_I(tc_iph_count, before);
    CHECK_EQ_I(tc_oiph_count, 1);
    buf[0] = 'x'; before = tc_iph_count; tc_o_reset();
    CHECK_EQ_I(p_cpy(buf, 8, NULL), EINVAL);
    CHECK_EQ_I(buf[0], 0);
    CHECK_EQ_I(tc_iph_count, before);
    CHECK_EQ_I(tc_oiph_count, 1);
}
TC_CASE(cpy_range)
{
    char buf[8]; int before = tc_iph_count;
    if (!p_cpy) { CHECK(p_cpy != NULL); return; }
    buf[0] = 'x'; tc_o_reset();
    CHECK_EQ_I(p_cpy(buf, 2, "abc"), ERANGE);
    CHECK_EQ_I(buf[0], 0);
    CHECK_EQ_I(tc_iph_count, before);
    CHECK_EQ_I(tc_oiph_count, 1); CHECK_EQ_I(tc_o_errno(), ERANGE);
}
TC_CASE(cpy_success_no_handler)
{
    char buf[8]; int before = tc_iph_count;
    if (!p_cpy) { CHECK(p_cpy != NULL); return; }
    tc_o_reset();
    CHECK_EQ_I(p_cpy(buf, 8, "abc"), 0);
    CHECK_STR(buf, "abc");
    CHECK_EQ_I(tc_iph_count, before); CHECK_EQ_I(tc_oiph_count, 0);
}
TC_CASE(cat_errors)
{
    char buf[4]; int before = tc_iph_count;
    if (!p_cat) { CHECK(p_cat != NULL); return; }
    buf[0] = 'a'; buf[1] = 'b'; buf[2] = 'c'; buf[3] = 'd';  /* no terminator within size */
    tc_o_reset();
    CHECK_EQ_I(p_cat(buf, 4, "x"), EINVAL);
    CHECK_EQ_I(tc_iph_count, before); CHECK_EQ_I(tc_oiph_count, 1);
    before = tc_iph_count; tc_o_reset();
    buf[0] = 'x'; buf[1] = 'y'; buf[2] = 0;
    CHECK_EQ_I(p_cat(buf, 4, "abc"), ERANGE);
    CHECK_EQ_I(buf[0], 0);
    CHECK_EQ_I(tc_iph_count, before); CHECK_EQ_I(tc_oiph_count, 1);
}
TC_CASE(wcs_errors)
{
    wchar_t buf[8]; int before = tc_iph_count;
    if (!p_wcpy || !p_wcat) { CHECK(p_wcpy && p_wcat); return; }
    tc_o_reset();
    CHECK_EQ_I(p_wcpy(NULL, 8, L"a"), EINVAL);
    CHECK_EQ_I(tc_iph_count, before); CHECK_EQ_I(tc_oiph_count, 1);
    before = tc_iph_count; buf[0] = L'x'; tc_o_reset();
    CHECK_EQ_I(p_wcpy(buf, 2, L"abc"), ERANGE);
    CHECK_EQ_I(buf[0], 0);
    CHECK_EQ_I(tc_iph_count, before); CHECK_EQ_I(tc_oiph_count, 1);
    before = tc_iph_count; buf[0] = L'x'; buf[1] = L'y'; buf[2] = 0; tc_o_reset();
    CHECK_EQ_I(p_wcat(buf, 4, L"abc"), ERANGE);
    CHECK_EQ_I(tc_iph_count, before); CHECK_EQ_I(tc_oiph_count, 1);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install() && tc_oiph_install());
    p_cpy = (cpy_t)tc_crt_proc("_o_strcpy_s"); p_cat = (cpy_t)tc_crt_proc("_o_strcat_s");
    p_wcpy = (wcpy_t)tc_crt_proc("_o_wcscpy_s"); p_wcat = (wcpy_t)tc_crt_proc("_o_wcscat_s");
    TC_RUN(cpy_null_and_zero); TC_RUN(cpy_range); TC_RUN(cpy_success_no_handler);
    TC_RUN(cat_errors); TC_RUN(wcs_errors);
    return tc_end();
}
