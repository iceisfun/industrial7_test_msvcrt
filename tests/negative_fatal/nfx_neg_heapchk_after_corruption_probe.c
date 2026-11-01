/* TC
 * id:    nfx_neg_heapchk_after_corruption_probe
 * area:  negative_fatal
 * kind:  negative
 * funcs: _heapchk malloc
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _heapchk (only "minimal consistency" checks; _HEAPBADNODE etc. for a damaged heap); Windows heap manager (HeapValidate)
 * note:  Class G (demoted to probe, outside the pass/fail bar). Observed on Win10 22H2 19045 ucrtbase: after overrunning a
 *        16-byte malloc block by 64 bytes, _heapchk() returned _HEAPOK (-2, child exit 102). A stub returning _HEAPOK would
 *        pass an exact-102 assertion, so the outcome is only RECORDED here (INFO line), not asserted. The result depends on
 *        heap layout (a 16-byte block is served by the low-fragmentation heap, whose user-block overruns HeapValidate does
 *        not see; a backend-chunk header overrun would be detected). Missing datum: the detection outcome for a non-LFH
 *        backend block on real Windows. The test only asserts that the child ran and did not fail setup. The corrupted block
 *        is never freed.
 */
#include "tc.h"
#include <malloc.h>

TC_CASE(overrun_recorded)
{
    DWORD code = 0; int ran = tc_spawn_self("overrun", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 78);
    if (code == 102) tc_write("TC INFO " TC_TEST_ID ": _heapchk returned _HEAPOK after the overrun (not detected; LFH block)\n");
    else if (code == 104) tc_write("TC INFO " TC_TEST_ID ": _heapchk returned _HEAPBADNODE after the overrun\n");
    else tc_write("TC INFO " TC_TEST_ID ": child ended with a non-_HEAPOK/_HEAPBADNODE status (fatal or other _HEAP* value)\n");
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
    TC_RUN(overrun_recorded);
    return tc_end();
}
