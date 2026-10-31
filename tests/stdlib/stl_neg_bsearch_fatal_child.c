/* TC
 * id:    stl_neg_bsearch_fatal_child
 * area:  stdlib
 * kind:  negative
 * funcs: bsearch
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs "Parameter validation": default handler terminates; msvcrt.dll excluded (no validation)
 */
#include "tc.h"
#include <stdlib.h>

static int __cdecl cmp(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("badbsearch", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}
int main(void)
{
    int key = 1;
    tc_begin();
    if (tc_child_name()) { void *r = bsearch(&key, NULL, 3, sizeof(int), cmp); tc_write("POST-CALL-MARKER\n"); return r ? 1 : 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
