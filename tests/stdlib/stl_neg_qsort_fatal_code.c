/* TC
 * id:    stl_neg_qsort_fatal_code
 * area:  stdlib
 * kind:  negative
 * funcs: qsort
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs "Parameter validation"
 * note:  Probes the exact exit status of the ucrtbase default invalid-parameter path for qsort(NULL,1,1,cmp):
 *        expected fast-fail STATUS_STACK_BUFFER_OVERRUN 0xC0000409.
 */
#include "tc.h"
#include <stdlib.h>

static int __cdecl cmp(const void *a, const void *b) { return *(const unsigned char *)a - *(const unsigned char *)b; }

TC_CASE(exact_status)
{
    DWORD code = 0; int ran = tc_spawn_self("badqsort", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { tc_iph_default(); qsort(NULL, 1, 1, cmp); return 77; }
    TC_RUN(exact_status);
    return tc_end();
}
