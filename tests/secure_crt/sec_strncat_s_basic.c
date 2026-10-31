/* TC
 * id:    sec_strncat_s_basic
 * area:  secure_crt
 * funcs: strncat_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strncat_s
 */
#include "tc.h"
#include <string.h>

static volatile const char *v_s = "XYZ";

TC_CASE(count_less_than_len) { char d[16] = "ab"; CHECK_EQ_I(strncat_s(d, sizeof d, (const char *)v_s, 2), 0); CHECK_STR(d, "abXY"); }
TC_CASE(count_greater_stops_at_nul) { char d[16] = "ab"; CHECK_EQ_I(strncat_s(d, sizeof d, (const char *)v_s, 100), 0); CHECK_STR(d, "abXYZ"); }
TC_CASE(count_zero_leaves_dest) { char d[16] = "ab"; CHECK_EQ_I(strncat_s(d, sizeof d, (const char *)v_s, 0), 0); CHECK_STR(d, "ab"); }
TC_CASE(exact_fit) { char d[6] = "ab"; CHECK_EQ_I(strncat_s(d, 6, (const char *)v_s, 3), 0); CHECK_STR(d, "abXYZ"); }

int main(void) { tc_begin(); TC_RUN(count_less_than_len); TC_RUN(count_greater_stops_at_nul); TC_RUN(count_zero_leaves_dest); TC_RUN(exact_fit); return tc_end(); }
