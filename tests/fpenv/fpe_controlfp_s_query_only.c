/* TC
 * id:    fpe_controlfp_s_query_only
 * area:  fpenv
 * funcs: _controlfp
 * procs: _controlfp_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _controlfp_s

 */
#include "tc.h"
#include <float.h>
#include <errno.h>
TC_DECL_PROC(int, _controlfp_s, (unsigned int *, unsigned int, unsigned int));

TC_CASE(query_matches)
{
    unsigned int cur = 0xdeadbeef;
    CHECK_EQ_I(tc_p__controlfp_s(&cur, 0, 0), 0);
    CHECK_EQ_U(cur, _controlfp(0, 0));
}
TC_CASE(query_is_stable)
{
    unsigned int a = 1, b = 2;
    CHECK_EQ_I(tc_p__controlfp_s(&a, 0, 0), 0);
    CHECK_EQ_I(tc_p__controlfp_s(&b, 0, 0), 0);
    CHECK_EQ_U(a, b);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_controlfp_s); if (!tc_p__controlfp_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; } TC_RUN(query_matches); TC_RUN(query_is_stable); return tc_end(); }
