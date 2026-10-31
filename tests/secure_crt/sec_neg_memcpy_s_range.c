/* TC
 * id:    sec_neg_memcpy_s_range
 * area:  secure_crt
 * funcs: memcpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memcpy_s (ERANGE)
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static volatile const char *v_s = "abcdef";
static char *volatile v_null;
static const char *volatile v_nulls;

static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
static char sbuf[16] = "abcdefghijklmno";

TC_CASE(count_gt_destsize_ERANGE) { char d[16]; setup(); CHECK_EQ_I(memcpy_s(d, 4, sbuf, 8), ERANGE); }
TC_CASE(count_destsize_plus_one_ERANGE) { char d[16]; setup(); CHECK_EQ_I(memcpy_s(d, 4, sbuf, 5), ERANGE); }
TC_CASE(errno_ERANGE) { char d[16]; setup(); memcpy_s(d, 4, sbuf, 8); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(handler_count_one) { char d[16]; setup(); memcpy_s(d, 4, sbuf, 8); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(destsize_zero_count_nonzero_is_error) { char d[16]; setup(); CHECK(memcpy_s(d, 0, sbuf, 4) != 0); CHECK_EQ_I(tc_iph_count, 1); }

int main(void) { tc_begin(); TC_RUN(count_gt_destsize_ERANGE); TC_RUN(count_destsize_plus_one_ERANGE); TC_RUN(errno_ERANGE); TC_RUN(handler_count_one); TC_RUN(destsize_zero_count_nonzero_is_error); return tc_end(); }
