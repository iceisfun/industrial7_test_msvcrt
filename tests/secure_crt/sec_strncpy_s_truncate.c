/* TC
 * id:    sec_strncpy_s_truncate
 * area:  secure_crt
 * funcs: strncpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strncpy_s (_TRUNCATE), errno.h STRUNCATE
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static volatile const char *v_s = "abcdef";

TC_CASE(truncate_fits_returns_0) { char d[16]; CHECK_EQ_I(strncpy_s(d, sizeof d, (const char *)v_s, (size_t)-1), 0); CHECK_STR(d, "abcdef"); }
TC_CASE(truncate_exceeds_returns_80) { char d[4]; memset(d, 'x', sizeof d); CHECK_EQ_I(strncpy_s(d, sizeof d, (const char *)v_s, (size_t)-1), 80); }
TC_CASE(buffer_content_abc_terminated) { char d[4]; memset(d, 'x', sizeof d); strncpy_s(d, sizeof d, (const char *)v_s, (size_t)-1); CHECK_STR(d, "abc"); CHECK_EQ_I(d[3], 0); }
TC_CASE(strunc_literal_is_80) { CHECK_EQ_I(STRUNCATE, 80); CHECK_EQ_U(_TRUNCATE, (size_t)-1); }
TC_CASE(exact_fit_with_truncate_returns_0) { char d[7]; CHECK_EQ_I(strncpy_s(d, 7, (const char *)v_s, (size_t)-1), 0); CHECK_STR(d, "abcdef"); }
TC_CASE(iph_not_called) { char d[4]; tc_iph_install(); tc_iph_count = 0; strncpy_s(d, sizeof d, (const char *)v_s, (size_t)-1); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); TC_RUN(truncate_fits_returns_0); TC_RUN(truncate_exceeds_returns_80); TC_RUN(buffer_content_abc_terminated); TC_RUN(strunc_literal_is_80); TC_RUN(exact_fit_with_truncate_returns_0); TC_RUN(iph_not_called); return tc_end(); }
