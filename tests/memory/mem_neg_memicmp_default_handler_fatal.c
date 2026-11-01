/* TC
 * id:    mem_neg_memicmp_default_handler_fatal
 * area:  memory
 * kind:  negative
 * funcs: _memicmp
 * crt:   >=100
 * tier:  p1
 * conf:  inferred
 * ref:   MS docs "Parameter validation": default invalid-parameter handler terminates the process (that _memicmp validates NULL is inferred)
 * note:  Child process; asserts abnormal termination only (code != 0 and != 77).
 */
#include "tc.h"
#include <string.h>

TC_CASE(child_terminates_abnormally)
{
    DWORD code = 0; int ran = tc_spawn_self("memicmp_null", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
int main(void)
{
    static char buf[8] = "abcdefg";
    static void *volatile v_null = NULL;
    static volatile size_t v_n = 4;
    tc_begin();
    if (tc_child_name()) { tc_iph_default(); (void)_memicmp(v_null, buf, v_n); return 77; }
    TC_RUN(child_terminates_abnormally);
    return tc_end();
}
