/* TC
 * id:    hp_neg_aligned_free_foreign_ptr_probe
 * area:  heap
 * kind:  negative
 * funcs: _aligned_free malloc free
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _aligned_free (only for blocks from _aligned_malloc); _aligned_free(NULL) is a no-op
 * note:  PROBING _aligned_free(malloc(64)) in a child: the CRT reads a hidden back-pointer before the block, so the outcome is a
 *        crash, heap corruption, or silent survival (77). Exit code printed as TC INFO; only asserted that the child terminated.
 *        The parent's control _aligned_free(NULL) is documented to be fine.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>

static volatile size_t v_n = 64;

TC_CASE(foreign_child)
{
    DWORD code = 0; char b[32]; int ran = tc_spawn_self("foreign", &code, 20000);
    CHECK(ran);
    tc_hex(code, b); tc_write("TC INFO " TC_TEST_ID " exit="); tc_write(b); tc_write("\n");
}
TC_CASE(null_control)
{
    _aligned_free(NULL);
    CHECK(1);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        void *p = malloc(v_n);
        _aligned_free(p);
        return 77;
    }
    TC_RUN(foreign_child); TC_RUN(null_control);
    return tc_end();
}
