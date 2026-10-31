/* TC
 * id:    loc_neg_mbstowcs_null_src
 * area:  locale
 * kind:  negative
 * funcs: mbstowcs
 * crt:   >=80
 * tier:  p1
 * conf:  observed
 * ref:   MS docs mbstowcs (parameter validation, VS2005+); MS docs Parameter Validation
 * note:  NULL source with a non-NULL dest -> invalid-parameter handler, (size_t)-1, EINVAL. mbstowcs(NULL,"abc",0)==3 is documented.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile p_abc = "abc", *volatile p_null = NULL;

TC_CASE(null_src_minus1) { wchar_t w[4]; size_t r; if (!tc_iph_install()) TC_SKIP("no IPH"); r = mbstowcs(w, p_null, 4); CHECK_EQ_U(r, (size_t)-1); }
TC_CASE(errno_einval_iph) { wchar_t w[4]; int before = tc_iph_count; if (!tc_iph_install()) TC_SKIP("no IPH"); errno = 0; mbstowcs(w, p_null, 4); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, before + 1); }
TC_CASE(null_dest_ok_zero_count) { int before = tc_iph_count; size_t r = mbstowcs(NULL, p_abc, 0); CHECK_EQ_U(r, 3); CHECK_EQ_I(tc_iph_count, before); }

int main(void) { tc_begin(); TC_RUN(null_src_minus1); TC_RUN(errno_einval_iph); TC_RUN(null_dest_ok_zero_count); return tc_end(); }
