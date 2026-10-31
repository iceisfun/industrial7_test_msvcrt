/* TC
 * id:    sec_neg_strncat_s_args
 * area:  secure_crt
 * funcs: strncat_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncat_s (EINVAL)
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static volatile const char *v_s = "abcdef";
static char *volatile v_null;
static const char *volatile v_nulls;

static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(null_dest_EINVAL) { setup(); CHECK_EQ_I(strncat_s(v_null, 8, (const char *)v_s, 3), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size_zero_EINVAL) { char d[8] = "ab"; setup(); CHECK_EQ_I(strncat_s(d, 0, (const char *)v_s, 3), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_src_nonzero_count_EINVAL) { char d[8] = "ab"; setup(); CHECK_EQ_I(strncat_s(d, sizeof d, v_nulls, 3), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(dest_unterminated_EINVAL) { char d[4]; memset(d, 'x', sizeof d); setup(); CHECK_EQ_I(strncat_s(d, sizeof d, "y", 1), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(handler_counts) { char d[8] = "ab"; setup(); strncat_s(v_null, 8, "x", 1); strncat_s(d, 0, "x", 1); strncat_s(d, sizeof d, v_nulls, 1); CHECK_EQ_I(tc_iph_count, 3); }

int main(void) { tc_begin(); TC_RUN(null_dest_EINVAL); TC_RUN(size_zero_EINVAL); TC_RUN(null_src_nonzero_count_EINVAL); TC_RUN(dest_unterminated_EINVAL); TC_RUN(handler_counts); return tc_end(); }
