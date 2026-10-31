/* TC
 * id:    str_strcat_basic
 * area:  string
 * funcs: strcat
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.24.3.1; MS docs strcat
 */
#include "tc.h"
#include <string.h>

static char buf[32];
static void fill(void) { memset(buf, 'Z', sizeof buf); }

TC_CASE(append) { volatile const char *s = "def"; fill(); strcpy(buf, "abc"); strcat(buf, (const char *)s); CHECK_STR(buf, "abcdef"); CHECK_EQ_I(buf[7], 'Z'); }
TC_CASE(append_empty) { volatile const char *s = ""; fill(); strcpy(buf, "abc"); strcat(buf, (const char *)s); CHECK_STR(buf, "abc"); CHECK_EQ_I(buf[4], 'Z'); }
TC_CASE(to_empty_dest) { volatile const char *s = "xyz"; fill(); buf[0] = 0; strcat(buf, (const char *)s); CHECK_STR(buf, "xyz"); }
TC_CASE(returns_dest) { volatile const char *s = "q"; fill(); buf[0] = 'a'; buf[1] = 0; CHECK_EQ_P(strcat(buf, (const char *)s), buf); }
TC_CASE(chained) { volatile const char *a = "one", *b = "-", *c = "two"; fill(); buf[0] = 0; strcat(strcat(strcat(buf, (const char *)a), (const char *)b), (const char *)c); CHECK_STR(buf, "one-two"); }

int main(void) { tc_begin(); TC_RUN(append); TC_RUN(append_empty); TC_RUN(to_empty_dest); TC_RUN(returns_dest); TC_RUN(chained); return tc_end(); }
