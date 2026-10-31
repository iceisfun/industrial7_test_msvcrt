/* TC
 * id:    str_strncat_basic
 * area:  string
 * funcs: strncat
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.24.3.2; MS docs strncat
 */
#include "tc.h"
#include <string.h>

static char buf[32];
static void fill(void) { memset(buf, 'Z', sizeof buf); strcpy(buf, "ab"); }

TC_CASE(n_gt_len) { volatile const char *s = "cde"; fill(); strncat(buf, (const char *)s, 10); CHECK_STR(buf, "abcde"); CHECK_EQ_I(buf[6], 'Z'); }
TC_CASE(n_lt_len) { volatile const char *s = "cdefg"; fill(); strncat(buf, (const char *)s, 2); CHECK_STR(buf, "abcd"); CHECK_EQ_I(buf[5], 'Z'); }
TC_CASE(n_zero) { volatile const char *s = "cde"; fill(); strncat(buf, (const char *)s, 0); CHECK_STR(buf, "ab"); CHECK_EQ_I(buf[3], 'Z'); }
TC_CASE(always_terminates) { volatile const char *s = "cdefg"; fill(); buf[4] = 'Q'; strncat(buf, (const char *)s, 3); CHECK_EQ_I(buf[5], 0); CHECK_STR(buf, "abcde"); }
TC_CASE(returns_dest) { volatile const char *s = "c"; fill(); CHECK_EQ_P(strncat(buf, (const char *)s, 1), buf); }

int main(void) { tc_begin(); TC_RUN(n_gt_len); TC_RUN(n_lt_len); TC_RUN(n_zero); TC_RUN(always_terminates); TC_RUN(returns_dest); return tc_end(); }
