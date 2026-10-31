/* TC
 * id:    scn_neg_swscanf_null_params
 * area:  scanf
 * funcs: swscanf sscanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs scanf, scanf_l, wscanf, wscanf_l: parameter validation; MS docs Parameter Validation
 * note:  NULL input or NULL format string: invalid-parameter handler is invoked; function returns EOF and sets errno to EINVAL.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static volatile const wchar_t *vw;
static volatile const char *vn;
static const wchar_t *volatile nullw;
static const char *volatile nulln;

TC_CASE(swscanf_null_input) { int a = 9, r; tc_iph_count = 0; errno = 0; r = swscanf((const wchar_t *)nullw, L"%d", &a);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(a, 9); }
TC_CASE(swscanf_null_format) { int a = 9, r; vw = L"5"; tc_iph_count = 0; errno = 0; r = swscanf((const wchar_t *)vw, nullw, &a);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(a, 9); }
TC_CASE(sscanf_null_input) { int a = 9, r; tc_iph_count = 0; errno = 0; r = sscanf((const char *)nulln, "%d", &a);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(a, 9); }
TC_CASE(sscanf_null_format) { int a = 9, r; vn = "5"; tc_iph_count = 0; errno = 0; r = sscanf((const char *)vn, nulln, &a);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(a, 9); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::no_iph\n"); return 1; }
    TC_RUN(swscanf_null_input); TC_RUN(swscanf_null_format); TC_RUN(sscanf_null_input); TC_RUN(sscanf_null_format);
    return tc_end();
}
