/* TC
 * id:    nfx_neg_heap_double_free_probe
 * area:  negative_fatal
 * funcs: malloc free
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.3.3 (double free is undefined); Windows heap manager
 * note:  Probes that a double free() of a small block terminates the process with STATUS_HEAP_CORRUPTION (0xC0000374)
 *        on a default Windows 10 process heap. May silently succeed on some configurations (child then exits 77).
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(double_free_fatal)
{
    DWORD code = 0; int ran = tc_spawn_self("dfree", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0xC0000374u);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { void *volatile p = malloc(32); free(p); free(p); return 77; }
    TC_RUN(double_free_fatal);
    return tc_end();
}
