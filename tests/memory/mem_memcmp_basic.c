/* TC
 * id:    mem_memcmp_basic
 * area:  memory
 * funcs: memcmp
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.4.1; MS docs memcmp
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>

static volatile size_t v_n;
#define SGN(x) (((x) > 0) - ((x) < 0))
static int cmp(const void *a, const void *b, size_t n) { v_n = n; return memcmp(a, b, v_n); }

TC_CASE(equal_returns_0) { static const char a[] = "abcdef", b[] = "abcdef"; CHECK_EQ_I(cmp(a, b, 6), 0); }
TC_CASE(less_negative) { static const char a[] = "abcdef", b[] = "abcdeg"; CHECK_EQ_I(SGN(cmp(a, b, 6)), -1); }
TC_CASE(greater_positive) { static const char a[] = "abcdeg", b[] = "abcdef"; CHECK_EQ_I(SGN(cmp(a, b, 6)), 1); }
TC_CASE(first_byte_differs) { static const char a[] = "Xbcdef", b[] = "abcdef"; CHECK_EQ_I(SGN(cmp(a, b, 6)), -1); CHECK_EQ_I(SGN(cmp(b, a, 6)), 1); }
TC_CASE(last_byte_differs) { static const char a[] = "abcdez", b[] = "abcdef"; CHECK_EQ_I(SGN(cmp(a, b, 6)), 1); CHECK_EQ_I(SGN(cmp(b, a, 6)), -1); }

int main(void) { tc_begin(); TC_RUN(equal_returns_0); TC_RUN(less_negative); TC_RUN(greater_positive); TC_RUN(first_byte_differs); TC_RUN(last_byte_differs); return tc_end(); }
