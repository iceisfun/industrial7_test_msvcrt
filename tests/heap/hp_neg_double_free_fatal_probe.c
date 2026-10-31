/* TC
 * id:    hp_neg_double_free_fatal_probe
 * area:  heap
 * kind:  negative
 * funcs: malloc free
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   MS docs free ("Attempting to free an invalid pointer ... may cause errors"); undefined behaviour per C11 7.22.3.3
 * note:  PROBING what the Windows 10 heap does on free(p); free(p) in a child process: heap-corruption fast-fail
 *        (0xC0000374 STATUS_HEAP_CORRUPTION) or no detection (child returns 77). The exit code is printed as TC INFO; only asserted
 *        that the child terminated by itself (no hang).
 */
#include "tc.h"
#include <stdlib.h>

static volatile size_t v_n = 64;

TC_CASE(double_free_child)
{
    DWORD code = 0; char b[32]; int ran = tc_spawn_self("dbl", &code, 20000);
    CHECK(ran);
    tc_hex(code, b); tc_write("TC INFO " TC_TEST_ID " exit="); tc_write(b); tc_write("\n");
}
TC_CASE(control_single_free)
{
    DWORD code = 0; int ran = tc_spawn_self("ok", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        void *p = malloc(v_n);
        free(p);
        if (c[0] == 'd') free(p);
        return 77;
    }
    TC_RUN(double_free_child); TC_RUN(control_single_free);
    return tc_end();
}
