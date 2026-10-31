/* TC
 * id:    ucr_neg_wcstol_bad_args
 * area:  ucrt_only
 * funcs: wcstol wcstoul
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstol, wcstoul: invalid parameter handler invoked, errno EINVAL, return 0
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>

static const wchar_t *volatile w;
TC_CASE(base1) { wchar_t *e; w = L"10"; tc_iph_count = 0; errno = 0;
  CHECK_EQ_I(wcstol((const wchar_t *)w, &e, 1), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
  tc_iph_count = 0; errno = 0; CHECK_EQ_U(wcstoul((const wchar_t *)w, &e, 1), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(base37) { wchar_t *e; w = L"10"; tc_iph_count = 0; errno = 0;
  CHECK_EQ_I(wcstol((const wchar_t *)w, &e, 37), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
  tc_iph_count = 0; errno = 0; CHECK_EQ_U(wcstoul((const wchar_t *)w, &e, 37), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_string) { wchar_t *e; tc_iph_count = 0; errno = 0;
  CHECK_EQ_I(wcstol(NULL, &e, 10), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
  tc_iph_count = 0; errno = 0; CHECK_EQ_U(wcstoul(NULL, &e, 10), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 2; TC_RUN(base1); TC_RUN(base37); TC_RUN(null_string); return tc_end(); }
