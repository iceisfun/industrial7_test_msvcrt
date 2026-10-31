/* TC
 * id:    gcv_neg_atoll_l_null_fatal_child
 * area:  conv
 * funcs: _atoll_l
 * crt:   >=120
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation (default invalid parameter handler terminates)
 * note:  Exact termination code differs per CRT; only asserts the child never reaches return 77.
 */
#include "tc.h"
#include <stdlib.h>

static int child_main(const char *n)
{
    volatile const char *s = NULL;
    (void)n;
    _atoll_l((const char *)s, NULL);
    return 77;
}
TC_CASE(default_handler_terminates)
{
    DWORD code = 0; int ok = tc_spawn_self("null", &code, 20000);
    CHECK(ok);
    CHECK(code != 77);
    CHECK(code != 0xdead0001u);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(default_handler_terminates);
    return tc_end();
}
