/* TC
 * id:    sec_neg_memcpy_s_args
 * area:  secure_crt
 * funcs: memcpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs memcpy_s (EINVAL)
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static volatile const char *v_s = "abcdef";
static char *volatile v_null;
static const char *volatile v_nulls;

static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
static char sbuf[8] = "abcdefg";

TC_CASE(null_dest_count_nonzero_EINVAL) { setup(); CHECK_EQ_I(memcpy_s(v_null, 8, sbuf, 4), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_src_count_nonzero_EINVAL) { char d[8]; setup(); CHECK_EQ_I(memcpy_s(d, sizeof d, v_null, 4), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(handler_counts) { char d[8]; setup(); memcpy_s(v_null, 8, sbuf, 4); memcpy_s(d, sizeof d, v_null, 4); CHECK_EQ_I(tc_iph_count, 2); }
TC_CASE(errno_EINVAL) { char d[8]; setup(); memcpy_s(d, sizeof d, v_null, 4); CHECK_EQ_I(errno, EINVAL); errno = 0; memcpy_s(v_null, 8, sbuf, 4); CHECK_EQ_I(errno, EINVAL); }

int main(void) { tc_begin(); TC_RUN(null_dest_count_nonzero_EINVAL); TC_RUN(null_src_count_nonzero_EINVAL); TC_RUN(handler_counts); TC_RUN(errno_EINVAL); return tc_end(); }
