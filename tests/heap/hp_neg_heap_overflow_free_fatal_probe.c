/* TC
 * id:    hp_neg_heap_overflow_free_fatal_probe
 * area:  heap
 * kind:  negative
 * funcs: malloc free
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   undefined behaviour per C11 7.22.3.3 / J.2; Windows heap integrity checks (STATUS_HEAP_CORRUPTION 0xC0000374)
 * note:  PROBING whether writing 256 bytes past a 32-byte block, then free() and another malloc(), is detected by the Windows 10
 *        heap in a child process (0xC0000374 or access violation) or survives (child returns 77). Exit code printed as TC INFO;
 *        only asserted that the child terminated by itself.
 */
#include "tc.h"
#include <stdlib.h>

static volatile size_t v_n = 32;

TC_CASE(overflow_child)
{
    DWORD code = 0; char b[32]; int ran = tc_spawn_self("ovf", &code, 20000);
    CHECK(ran);
    tc_hex(code, b); tc_write("TC INFO " TC_TEST_ID " exit="); tc_write(b); tc_write("\n");
}
TC_CASE(control_no_overflow)
{
    DWORD code = 0; int ran = tc_spawn_self("ok", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        volatile unsigned char *p = (volatile unsigned char *)malloc(v_n);
        size_t i, n = v_n + (c[0] == 'o' ? 256 : 0);
        void *q;
        for (i = 0; i < n; i++) p[i] = 0xCC;
        free((void *)p);
        q = malloc(v_n);
        free(q);
        return 77;
    }
    TC_RUN(overflow_child); TC_RUN(control_no_overflow);
    return tc_end();
}
