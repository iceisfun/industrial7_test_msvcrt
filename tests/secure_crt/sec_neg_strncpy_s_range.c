/* TC
 * id:    sec_neg_strncpy_s_range
 * area:  secure_crt
 * funcs: strncpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncpy_s (ERANGE, dest[0] set to 0)
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static volatile const char *v_s = "abcdef";
static char *volatile v_null;
static const char *volatile v_nulls;

static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(count_ge_size_src_long_ERANGE) { char d[4]; memset(d, 'x', sizeof d); setup(); CHECK_EQ_I(strncpy_s(d, sizeof d, (const char *)v_s, 10), ERANGE); }
TC_CASE(count_equals_srclen_dest_too_small_ERANGE) { char d[4]; memset(d, 'x', sizeof d); setup(); CHECK_EQ_I(strncpy_s(d, sizeof d, (const char *)v_s, 6), ERANGE); }
TC_CASE(dest_emptied) { char d[4]; memset(d, 'x', sizeof d); setup(); strncpy_s(d, sizeof d, (const char *)v_s, 6); CHECK_EQ_I(d[0], 0); }
TC_CASE(errno_ERANGE) { char d[4]; memset(d, 'x', sizeof d); setup(); strncpy_s(d, sizeof d, (const char *)v_s, 6); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(handler_count_one) { char d[4]; memset(d, 'x', sizeof d); setup(); strncpy_s(d, sizeof d, (const char *)v_s, 6); CHECK_EQ_I(tc_iph_count, 1); }

int main(void) { tc_begin(); TC_RUN(count_ge_size_src_long_ERANGE); TC_RUN(count_equals_srclen_dest_too_small_ERANGE); TC_RUN(dest_emptied); TC_RUN(errno_ERANGE); TC_RUN(handler_count_one); return tc_end(); }
