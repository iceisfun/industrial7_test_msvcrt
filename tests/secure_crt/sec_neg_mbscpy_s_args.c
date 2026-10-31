/* TC
 * id:    sec_neg_mbscpy_s_args
 * area:  secure_crt
 * funcs: _mbscpy_s _mbscat_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbscpy_s, _mbscpy_s_l, _mbscat_s
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
static volatile const unsigned char *v_s;

TC_CASE(mbscpy_null_dest_EINVAL) { v_s = (const unsigned char *)"abc"; RESET();
    CHECK_EQ_I(_mbscpy_s(NULL, 8, (const unsigned char *)v_s), EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(mbscpy_null_src_EINVAL) { unsigned char d[8] = "zzz"; v_s = NULL; RESET();
    CHECK_EQ_I(_mbscpy_s(d, 8, (const unsigned char *)v_s), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(d[0], 0); }
TC_CASE(mbscpy_small_ERANGE) { unsigned char d[3] = "zz"; v_s = (const unsigned char *)"abc"; RESET();
    CHECK_EQ_I(_mbscpy_s(d, 3, (const unsigned char *)v_s), ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(d[0], 0); }
TC_CASE(mbscat_unterminated_EINVAL) { unsigned char d[4] = {'a', 'b', 'c', 'd'}; v_s = (const unsigned char *)"x"; RESET();
    CHECK_EQ_I(_mbscat_s(d, 4, (const unsigned char *)v_s), EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(mbscat_small_ERANGE) { unsigned char d[4] = "ab"; v_s = (const unsigned char *)"cde"; RESET();
    CHECK_EQ_I(_mbscat_s(d, 4, (const unsigned char *)v_s), ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(d[0], 0); }
TC_CASE(handler_counts) { unsigned char d[3] = "zz", e[4] = {'a', 'b', 'c', 'd'}; v_s = (const unsigned char *)"abc"; RESET();
    _mbscpy_s(NULL, 8, (const unsigned char *)v_s); _mbscpy_s(d, 3, (const unsigned char *)v_s); _mbscat_s(e, 4, (const unsigned char *)v_s);
    CHECK_EQ_I(tc_iph_count, 3); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(mbscpy_null_dest_EINVAL); TC_RUN(mbscpy_null_src_EINVAL); TC_RUN(mbscpy_small_ERANGE); TC_RUN(mbscat_unterminated_EINVAL); TC_RUN(mbscat_small_ERANGE); TC_RUN(handler_counts); return tc_end(); }
