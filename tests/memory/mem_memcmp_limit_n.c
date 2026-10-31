/* TC
 * id:    mem_memcmp_limit_n
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

TC_CASE(diff_after_n_ignored) { static const char a[] = "abcdXf", b[] = "abcdYf"; CHECK_EQ_I(cmp(a, b, 4), 0); CHECK_EQ_I(SGN(cmp(a, b, 5)), -1); }
TC_CASE(diff_at_n_minus_1_seen) { static const char a[] = "abcZ", b[] = "abcA"; CHECK_EQ_I(SGN(cmp(a, b, 4)), 1); CHECK_EQ_I(cmp(a, b, 3), 0); }
TC_CASE(embedded_nul_does_not_stop)
{
    static const unsigned char a[] = {'a', 0, 'b', 0, 'c'}, b[] = {'a', 0, 'b', 0, 'd'};
    CHECK_EQ_I(cmp(a, b, 4), 0);
    CHECK_EQ_I(SGN(cmp(a, b, 5)), -1);
}

int main(void) { tc_begin(); TC_RUN(diff_after_n_ignored); TC_RUN(diff_at_n_minus_1_seen); TC_RUN(embedded_nul_does_not_stop); return tc_end(); }
