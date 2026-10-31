/* TC
 * id:    hp_neg_free_non_heap_fatal_probe
 * area:  heap
 * kind:  negative
 * funcs: free malloc
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   undefined behaviour per C11 7.22.3.3
 * note:  PROBING free() of a static buffer and of an interior pointer (malloc(32)+8) in child processes: heap-corruption fast-fail
 *        / access violation or silent survival (77). Exit codes printed as TC INFO; only asserted that each child terminated.
 */
#include "tc.h"
#include <stdlib.h>

static volatile size_t v_n = 32;
static char sbuf[64];

static void report(const char *n, DWORD code)
{ char b[32]; tc_hex(code, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(n); tc_write("="); tc_write(b); tc_write("\n"); }

TC_CASE(static_address)
{
    DWORD code = 0; int ran = tc_spawn_self("static", &code, 20000);
    CHECK(ran); report("static_exit", code);
}
TC_CASE(interior_pointer)
{
    DWORD code = 0; int ran = tc_spawn_self("interior", &code, 20000);
    CHECK(ran); report("interior_exit", code);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 's') { char *volatile s = sbuf; free(s); }
        else { char *p = (char *)malloc(v_n); free(p + 8); }
        return 77;
    }
    TC_RUN(static_address); TC_RUN(interior_pointer);
    return tc_end();
}
