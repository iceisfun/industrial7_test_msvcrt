/* TC
 * id:    str_strncpy_basic
 * area:  string
 * funcs: strncpy
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.24.2.4; MS docs strncpy
 */
#include "tc.h"
#include <string.h>

static char buf[32];
static void fill(void) { memset(buf, 'Z', sizeof buf); }

TC_CASE(pads) { volatile const char *s = "abc"; fill(); strncpy(buf, (const char *)s, 8); CHECK_MEM(buf, "abc\0\0\0\0\0Z", 9); }
TC_CASE(n_eq_len) { volatile const char *s = "abc"; fill(); strncpy(buf, (const char *)s, 3); CHECK_MEM(buf, "abcZ", 4); }
TC_CASE(truncates) { volatile const char *s = "abcdef"; fill(); strncpy(buf, (const char *)s, 3); CHECK_MEM(buf, "abcZ", 4); }
TC_CASE(n_zero) { volatile const char *s = "abc"; fill(); CHECK_EQ_P(strncpy(buf, (const char *)s, 0), buf); CHECK_EQ_I(buf[0], 'Z'); }
TC_CASE(returns_dest) { volatile const char *s = "abc"; fill(); CHECK_EQ_P(strncpy(buf, (const char *)s, 5), buf); }
TC_CASE(empty_src_all_nul) { volatile const char *s = ""; fill(); strncpy(buf, (const char *)s, 4); CHECK_MEM(buf, "\0\0\0\0Z", 5); }

int main(void) { tc_begin(); TC_RUN(pads); TC_RUN(n_eq_len); TC_RUN(truncates); TC_RUN(n_zero); TC_RUN(returns_dest); TC_RUN(empty_src_all_nul); return tc_end(); }
