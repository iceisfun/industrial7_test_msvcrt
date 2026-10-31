/* TC
 * id:    nfx_neg_heapchk_after_corruption_probe
 * area:  negative_fatal
 * kind:  negative
 * funcs: _heapchk malloc
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _heapchk (returns _HEAPBADNODE etc. for a damaged heap); Windows heap manager (HeapValidate)
 * note:  Probes: after a 64-byte overrun of a 16-byte malloc block, _heapchk does not report a healthy heap. Accepted outcomes: child
 *        exit 104 (_heapchk returned _HEAPBADNODE, -4) or a fatal status: 0xC0000374 (STATUS_HEAP_CORRUPTION), 0xC0000409 (fastfail) or 0xC0000005 (access violation). Child exit 102 means
 *        _HEAPOK (corruption not detected) and fails the test; other _HEAP* values are reported as 100 + (-result).
 *        The corrupted block is never freed.
 */
#include "tc.h"
#include <malloc.h>

TC_CASE(overrun_detected)
{
    DWORD code = 0; int ran = tc_spawn_self("overrun", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 102);
    CHECK(code == 104 || code == 0xC0000374u || code == 0xC0000005u || code == 0xC0000409u);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        volatile char *p = (volatile char *)malloc(16); int i, r;
        if (!p) return 78;
        for (i = 0; i < 16 + 64; i++) p[i] = (char)0xEE;
        r = _heapchk();
        return 100 + (-r);
    }
    TC_RUN(overrun_detected);
    return tc_end();
}
