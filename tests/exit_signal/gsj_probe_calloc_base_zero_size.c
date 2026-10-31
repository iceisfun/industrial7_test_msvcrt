/* TC
 * id:    gsj_probe_calloc_base_zero_size
 * area:  exit_signal
 * funcs:
 * procs: _calloc_base _free_base
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs calloc (zero-size behaviour not relied on)
 * note:  PROBING that _calloc_base with a zero count or zero size returns a non-NULL, freeable pointer (as ucrt malloc(0) does).
 *        C11 7.22.3 leaves this implementation-defined, so it is isolated from the documented zero-fill tests.
 */
#include "tc.h"

TC_DECL_PROC(void *, _calloc_base, (size_t, size_t));
TC_DECL_PROC(void, _free_base, (void *));
static volatile size_t v_n, v_s;

TC_CASE(zero_count)
{
    void *a; if (!tc_p__calloc_base) TC_SKIP("no _calloc_base");
    v_n = 0; v_s = 10; a = tc_p__calloc_base(v_n, v_s);
    CHECK(a != NULL);
    tc_p__free_base(a);
}
TC_CASE(zero_size)
{
    void *b; if (!tc_p__calloc_base) TC_SKIP("no _calloc_base");
    v_n = 10; v_s = 0; b = tc_p__calloc_base(v_n, v_s);
    CHECK(b != NULL);
    tc_p__free_base(b);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_calloc_base); TC_LOAD_PROC(_free_base);
    TC_RUN(zero_count); TC_RUN(zero_size);
    return tc_end();
}
