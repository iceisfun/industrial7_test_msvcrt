/* TC
 * id:    str_strlen_basic
 * area:  string
 * funcs: strlen
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.6.3; MS docs strlen
 */
#include "tc.h"
#include <string.h>

TC_CASE(empty)  { volatile const char *s = ""; CHECK_EQ_U(strlen((const char *)s), 0); }
TC_CASE(simple) { volatile const char *s = "hello"; CHECK_EQ_U(strlen((const char *)s), 5); }
TC_CASE(embedded_nul) { static const char s[] = "ab\0cd"; CHECK_EQ_U(strlen(s), 2); }
TC_CASE(high_bytes) { static const char s[] = "\xff\x80\x01"; CHECK_EQ_U(strlen(s), 3); }

int main(void) { tc_begin(); TC_RUN(empty); TC_RUN(simple); TC_RUN(embedded_nul); TC_RUN(high_bytes); return tc_end(); }
