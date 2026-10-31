/* TC
 * id:    nfx_neg_heap_overrun_free_probe
 * area:  negative_fatal
 * funcs: malloc free
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.3 (writing outside the allocation is undefined); Windows heap manager
 * note:  Probes that overwriting 16 bytes past the end of a heap block is detected when the block is freed and
 *        terminates with STATUS_HEAP_CORRUPTION (0xC0000374). Detection is not guaranteed (child exits 77 if not).
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

static int overrun(size_t n)
{
    volatile char *p = (volatile char *)malloc(n); size_t i;
    if (!p) return 78;
    for (i = 0; i < n + 16; i++) p[i] = (char)0xEE;
    free((void *)p);
    return 77;
}
TC_CASE(small_block)
{
    DWORD code = 0; int ran = tc_spawn_self("small", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0xC0000374u);
}
TC_CASE(page_block)
{
    DWORD code = 0; int ran = tc_spawn_self("page", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0xC0000374u);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return overrun(c[0] == 's' ? 32 : 0x1000);
    TC_RUN(small_block); TC_RUN(page_block);
    return tc_end();
}
