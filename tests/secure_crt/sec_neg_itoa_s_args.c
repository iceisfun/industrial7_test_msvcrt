/* TC
 * id:    sec_neg_itoa_s_args
 * area:  secure_crt
 * funcs: _itoa_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itoa_s, _itow_s (radix must be 2..36)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
static volatile int v_radix;
static volatile size_t v_sz;

TC_CASE(null_buffer_EINVAL) { v_radix = 10; RESET(); CHECK_EQ_I(_itoa_s(5, NULL, 8, v_radix), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size_zero_EINVAL) { char b[8] = "zz"; v_radix = 10; v_sz = 0; RESET(); CHECK_EQ_I(_itoa_s(5, b, v_sz, v_radix), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(radix_1_EINVAL) { char b[8]; v_radix = 1; RESET(); CHECK_EQ_I(_itoa_s(5, b, 8, v_radix), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(radix_37_EINVAL) { char b[8]; v_radix = 37; RESET(); CHECK_EQ_I(_itoa_s(5, b, 8, v_radix), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(radix_0_EINVAL) { char b[8]; v_radix = 0; RESET(); CHECK_EQ_I(_itoa_s(5, b, 8, v_radix), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(handler_counts) { char b[8]; v_radix = 10; RESET(); _itoa_s(5, NULL, 8, v_radix); _itoa_s(5, b, 0, v_radix); _itoa_s(5, b, 8, 1);
    CHECK_EQ_I(tc_iph_count, 3); RESET(); CHECK_EQ_I(_itoa_s(123, b, 8, v_radix), 0); CHECK_STR(b, "123"); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(null_buffer_EINVAL); TC_RUN(size_zero_EINVAL); TC_RUN(radix_1_EINVAL); TC_RUN(radix_37_EINVAL); TC_RUN(radix_0_EINVAL); TC_RUN(handler_counts); return tc_end(); }
