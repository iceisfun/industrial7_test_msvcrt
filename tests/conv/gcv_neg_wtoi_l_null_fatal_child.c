/* TC
 * id:    gcv_neg_wtoi_l_null_fatal_child
 * area:  conv
 * funcs: _wtoi_l _wtoi64_l
 * crt:   >=100
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation (default invalid parameter handler terminates)
 * note:  Only asserts the child never reaches return 77; exact exit code differs per CRT.
 */
#include "tc.h"
#include <stdlib.h>

static int child_main(const char *n)
{
    const wchar_t *volatile s = NULL;
    if (n[0] == 'a') _wtoi_l(s, NULL); else _wtoi64_l(s, NULL);
    return 77;
}
TC_CASE(wtoi_l_terminates) { DWORD code = 0; CHECK(tc_spawn_self("a", &code, 20000)); CHECK(code != 77); CHECK(code != 0xdead0001u); }
TC_CASE(wtoi64_l_terminates) { DWORD code = 0; CHECK(tc_spawn_self("b", &code, 20000)); CHECK(code != 77); CHECK(code != 0xdead0001u); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(wtoi_l_terminates); TC_RUN(wtoi64_l_terminates);
    return tc_end();
}
