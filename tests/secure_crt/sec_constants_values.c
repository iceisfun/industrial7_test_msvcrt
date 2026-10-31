/* TC
 * id:    sec_constants_values
 * area:  secure_crt
 * funcs: strncpy_s strcpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  documented
 * ref:   MS errno.h (EINVAL 22, ERANGE 34, EILSEQ 42, STRUNCATE 80), crtdefs.h (_TRUNCATE = (size_t)-1), stdio.h L_tmpnam_s
 * note:  EINVAL is provoked with a NULL dest (not size 0): the docs table lists size 0 as ERANGE while the
 *        implementation is believed to return EINVAL (see sec_neg_strcpy_s_size0_probe).
 *        msvcr90 excluded because EILSEQ is not defined there. The runtime half checks the DLL returns these values.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <stdio.h>

static char *volatile v_nulld;
TC_CASE(EINVAL_22) { CHECK_EQ_I(EINVAL, 22); tc_iph_count = 0; CHECK_EQ_I(strcpy_s(v_nulld, 4, "a"), 22); }
TC_CASE(ERANGE_34) { char d[2]; CHECK_EQ_I(ERANGE, 34); CHECK_EQ_I(strcpy_s(d, sizeof d, "abc"), 34); }
TC_CASE(STRUNCATE_80) { char d[3]; CHECK_EQ_I(STRUNCATE, 80); CHECK_EQ_I(strncpy_s(d, sizeof d, "abcdef", _TRUNCATE), 80); CHECK_STR(d, "ab"); }
TC_CASE(EILSEQ_42) { CHECK_EQ_I(EILSEQ, 42); }
TC_CASE(TRUNCATE_is_size_t_max) { CHECK(_TRUNCATE == (size_t)-1); }
TC_CASE(L_tmpnam_s_positive) { CHECK(L_tmpnam_s > 0); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(EINVAL_22); TC_RUN(ERANGE_34); TC_RUN(STRUNCATE_80); TC_RUN(EILSEQ_42); TC_RUN(TRUNCATE_is_size_t_max); TC_RUN(L_tmpnam_s_positive);
    return tc_end();
}
