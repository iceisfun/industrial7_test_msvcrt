/* TC
 * id:    stl_neg_qsort_fatal_child
 * area:  stdlib
 * kind:  negative
 * funcs: qsort
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs "Parameter validation": default invalid parameter handler terminates the process
 * note:  Child process; exact termination status is probed separately in stl_neg_qsort_fatal_code.
 */
#include "tc.h"
#include <stdlib.h>
#include <stdio.h>

static int __cdecl cmp(const void *a, const void *b) { return *(const unsigned char *)a - *(const unsigned char *)b; }

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("badqsort", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);   /* 77 = qsort returned normally */
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { tc_iph_default(); qsort(NULL, 1, 1, cmp); tc_write("POST-CALL-MARKER\n"); return 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
