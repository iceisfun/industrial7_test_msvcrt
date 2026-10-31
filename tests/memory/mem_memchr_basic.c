/* TC
 * id:    mem_memchr_basic
 * area:  memory
 * funcs: memchr
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.5.1; MS docs memchr
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>

static volatile size_t v_n;
static volatile int v_c;
static const char *find(const char *s, int c, size_t n) { v_c = c; v_n = n; return (const char *)memchr(s, v_c, v_n); }
static const char S[] = "hello world";

TC_CASE(found_middle) { CHECK_EQ_P(find(S, 'o', 11), S + 4); }
TC_CASE(found_first) { CHECK_EQ_P(find(S, 'h', 11), S); }
TC_CASE(found_last) { CHECK_EQ_P(find(S, 'd', 11), S + 10); }
TC_CASE(not_found_returns_null)
{
    CHECK_EQ_P(find(S, 'z', 11), NULL);
    CHECK_EQ_P(find(S, 'd', 10), NULL);  /* match lies beyond n */
    CHECK_EQ_P(find(S, 'h', 0), NULL);   /* n == 0 */
}

int main(void) { tc_begin(); TC_RUN(found_middle); TC_RUN(found_first); TC_RUN(found_last); TC_RUN(not_found_returns_null); return tc_end(); }
