/* TC
 * id:    cv_neg_wcstol_bad_base_null
 * area:  conv
 * funcs: wcstol wcstoul _wcstoi64
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstol / strtol: invalid base or NULL string -> errno EINVAL, invalid parameter handler invoked, returns 0
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>

static const wchar_t *volatile vs;

TC_CASE(wcstol_base1) { long v; vs = L"12"; tc_iph_count = 0; errno = 0; v = wcstol(vs, NULL, 1); CHECK_EQ_I(v, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wcstol_base37) { long v; vs = L"12"; tc_iph_count = 0; errno = 0; v = wcstol(vs, NULL, 37); CHECK_EQ_I(v, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wcstol_negbase) { long v; vs = L"12"; tc_iph_count = 0; errno = 0; v = wcstol(vs, NULL, -5); CHECK_EQ_I(v, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wcstol_null) { long v; tc_iph_count = 0; errno = 0; vs = NULL; v = wcstol(vs, NULL, 10); CHECK_EQ_I(v, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wcstoul_base99) { unsigned long v; vs = L"12"; tc_iph_count = 0; errno = 0; v = wcstoul(vs, NULL, 99); CHECK_EQ_U(v, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(i64_null) { __int64 v; tc_iph_count = 0; errno = 0; vs = NULL; v = _wcstoi64(vs, NULL, 10); CHECK_EQ_I(v, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(valid_bases_no_iph)
{
    tc_iph_count = 0; vs = L"11"; errno = 0;
    CHECK_EQ_I(wcstol(vs, NULL, 2), 3); CHECK_EQ_I(wcstol(vs, NULL, 36), 37); CHECK_EQ_I(wcstol(vs, NULL, 0), 11);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(wcstol_base1); TC_RUN(wcstol_base37); TC_RUN(wcstol_negbase); TC_RUN(wcstol_null); TC_RUN(wcstoul_base99); TC_RUN(i64_null);
    TC_RUN(valid_bases_no_iph);
    return tc_end();
}
