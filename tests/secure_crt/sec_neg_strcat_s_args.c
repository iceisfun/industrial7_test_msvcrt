/* TC
 * id:    sec_neg_strcat_s_args
 * area:  secure_crt
 * funcs: strcat_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcat_s (EINVAL)
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static volatile const char *v_s = "abcdef";
static char *volatile v_null;
static const char *volatile v_nulls;

static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(null_dest_EINVAL) { setup(); CHECK_EQ_I(strcat_s(v_null, 8, (const char *)v_s), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size_zero_EINVAL) { char d[8] = "ab"; setup(); CHECK_EQ_I(strcat_s(d, 0, (const char *)v_s), EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_STR(d, "ab"); }
TC_CASE(null_src_EINVAL) { char d[8] = "ab"; setup(); CHECK_EQ_I(strcat_s(d, sizeof d, v_nulls), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(dest_not_terminated_within_size_EINVAL) { char d[4]; memset(d, 'x', sizeof d); setup(); CHECK_EQ_I(strcat_s(d, sizeof d, "y"), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(handler_counts) { char d[8] = "ab"; setup(); strcat_s(v_null, 8, "x"); strcat_s(d, 0, "x"); strcat_s(d, sizeof d, v_nulls); CHECK_EQ_I(tc_iph_count, 3); }

int main(void) { tc_begin(); TC_RUN(null_dest_EINVAL); TC_RUN(size_zero_EINVAL); TC_RUN(null_src_EINVAL); TC_RUN(dest_not_terminated_within_size_EINVAL); TC_RUN(handler_counts); return tc_end(); }
