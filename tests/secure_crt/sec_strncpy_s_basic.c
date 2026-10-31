/* TC
 * id:    sec_strncpy_s_basic
 * area:  secure_crt
 * funcs: strncpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strncpy_s
 */
#include "tc.h"
#include <string.h>

static volatile const char *v_s = "abcdef";

TC_CASE(count_less_than_len) { char d[16]; memset(d, 'x', sizeof d); CHECK_EQ_I(strncpy_s(d, sizeof d, (const char *)v_s, 3), 0); CHECK_STR(d, "abc"); }
TC_CASE(count_equals_len) { char d[16]; CHECK_EQ_I(strncpy_s(d, sizeof d, (const char *)v_s, 6), 0); CHECK_STR(d, "abcdef"); }
TC_CASE(count_greater_than_len_stops_at_nul) { char d[16]; memset(d, 'x', sizeof d); CHECK_EQ_I(strncpy_s(d, sizeof d, (const char *)v_s, 100), 0); CHECK_STR(d, "abcdef"); }
TC_CASE(dest_always_terminated) { char d[8]; memset(d, 'x', sizeof d); CHECK_EQ_I(strncpy_s(d, sizeof d, (const char *)v_s, 4), 0); CHECK_EQ_I(d[4], 0); CHECK_EQ_I(strlen(d), 4); }
TC_CASE(iph_not_called) { char d[8]; tc_iph_install(); tc_iph_count = 0; strncpy_s(d, sizeof d, (const char *)v_s, 3); strncpy_s(d, sizeof d, (const char *)v_s, 0); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); TC_RUN(count_less_than_len); TC_RUN(count_equals_len); TC_RUN(count_greater_than_len_stops_at_nul); TC_RUN(dest_always_terminated); TC_RUN(iph_not_called); return tc_end(); }
