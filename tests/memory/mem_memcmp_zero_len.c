/* TC
 * id:    mem_memcmp_zero_len
 * area:  memory
 * funcs: memcmp
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.4.1 (n=0 compares no bytes); MS docs memcmp
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>

static volatile size_t v_zero = 0;

TC_CASE(zero_len_diff_buffers_returns_0) { static const char a[] = "aaaa", b[] = "zzzz"; CHECK_EQ_I(memcmp(a, b, v_zero), 0); CHECK_EQ_I(memcmp(b, a, v_zero), 0); }
TC_CASE(zero_len_same) { static const char a[] = "abc"; CHECK_EQ_I(memcmp(a, a, v_zero), 0); }

int main(void) { tc_begin(); TC_RUN(zero_len_diff_buffers_returns_0); TC_RUN(zero_len_same); return tc_end(); }
