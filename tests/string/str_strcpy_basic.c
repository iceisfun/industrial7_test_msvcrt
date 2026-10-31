/* TC
 * id:    str_strcpy_basic
 * area:  string
 * funcs: strcpy
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.24.2.3; MS docs strcpy
 */
#include "tc.h"
#include <string.h>

#define SGN(x) ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)
static char buf[32];
static void fill(void) { memset(buf, 'Z', sizeof buf); }

TC_CASE(copy_short) { volatile const char *s = "hello"; fill(); strcpy(buf, (const char *)s); CHECK_STR(buf, "hello"); CHECK_EQ_I(buf[5], 0); CHECK_EQ_I(buf[6], 'Z'); }
TC_CASE(copy_empty) { volatile const char *s = ""; fill(); strcpy(buf, (const char *)s); CHECK_EQ_I(buf[0], 0); CHECK_EQ_I(buf[1], 'Z'); }
TC_CASE(returns_dest) { volatile const char *s = "abc"; fill(); CHECK_EQ_P(strcpy(buf, (const char *)s), buf); }
TC_CASE(embedded_nul) { static const char s[] = "ab\0cd"; fill(); strcpy(buf, s); CHECK_EQ_I(buf[0], 'a'); CHECK_EQ_I(buf[1], 'b'); CHECK_EQ_I(buf[2], 0); CHECK_EQ_I(buf[3], 'Z'); }
TC_CASE(high_bit) { static const char s[] = "\x80\xff\xe9"; fill(); strcpy(buf, s); CHECK_MEM(buf, "\x80\xff\xe9\0Z", 5); }

int main(void) { tc_begin(); TC_RUN(copy_short); TC_RUN(copy_empty); TC_RUN(returns_dest); TC_RUN(embedded_nul); TC_RUN(high_bit); return tc_end(); }
