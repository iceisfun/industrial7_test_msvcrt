/* TC
 * id:    hp_neg_aligned_malloc_fatal_default
 * area:  heap
 * kind:  negative
 * funcs: _aligned_malloc _aligned_free
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   MS docs "Parameter validation" / _aligned_malloc: with the default invalid-parameter handler the process terminates
 * note:  Probes the exact termination status (0xC0000409 fast-fail expected on ucrtbase; docs only promise termination).
 *        The control child (valid alignment) must return normally, proving the harness works.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>

static volatile size_t v_n = 16, v_a;

TC_CASE(bad_alignment_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("bad", &code, 20000);
    CHECK(ran);
    CHECK(code != 0 && code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(control_valid_alignment)
{
    DWORD code = 0; int ran = tc_spawn_self("good", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        void *p;
        v_a = (c[0] == 'b') ? 3 : 16;
        p = _aligned_malloc(v_n, v_a);
        if (p) _aligned_free(p);
        return 77; /* reached only if the CRT did not terminate (or control) */
    }
    TC_RUN(bad_alignment_terminates); TC_RUN(control_valid_alignment);
    return tc_end();
}
