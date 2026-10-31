/* TC
 * id:    sec_neg_mbscpy_s_state
 * area:  secure_crt
 * funcs: _mbscpy_s _mbscat_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbscpy_s, _mbscat_s (parameter validation table; dest[0] reset to 0 on error)
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
typedef const unsigned char *cu;
static cu volatile v_s, v_null;
static volatile size_t v0 = 0;

TC_CASE(cpy_size0_EINVAL_untouched) { unsigned char d[4] = "zz"; v_s = (cu)"abc"; RESET();
    CHECK_EQ_I(_mbscpy_s(d, v0, v_s), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_STR((char *)d, "zz"); }
TC_CASE(cpy_erange_errno_and_d0) { unsigned char d[3] = "zz"; v_s = (cu)"abc"; RESET();
    CHECK_EQ_I(_mbscpy_s(d, 3, v_s), ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(d[0], 0); }
TC_CASE(cpy_null_src_d0) { unsigned char d[4] = "zz"; RESET();
    CHECK_EQ_I(_mbscpy_s(d, 4, v_null), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(d[0], 0); }
TC_CASE(cpy_exact_fit) { unsigned char d[4]; v_s = (cu)"abc"; RESET();
    CHECK_EQ_I(_mbscpy_s(d, 4, v_s), 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0); CHECK_STR((char *)d, "abc"); }
TC_CASE(cat_size0_EINVAL) { unsigned char d[4] = "a"; v_s = (cu)"b"; RESET();
    CHECK_EQ_I(_mbscat_s(d, v0, v_s), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(cat_exact_fit) { unsigned char d[4] = "a"; v_s = (cu)"bc"; RESET();
    CHECK_EQ_I(_mbscat_s(d, 4, v_s), 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR((char *)d, "abc"); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(cpy_size0_EINVAL_untouched); TC_RUN(cpy_erange_errno_and_d0); TC_RUN(cpy_null_src_d0);
    TC_RUN(cpy_exact_fit); TC_RUN(cat_size0_EINVAL); TC_RUN(cat_exact_fit); return tc_end(); }
