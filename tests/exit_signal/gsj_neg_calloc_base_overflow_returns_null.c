/* TC
 * id:    gsj_neg_calloc_base_overflow_returns_null
 * area:  exit_signal
 * kind:  negative
 * funcs:
 * procs: _calloc_base _free_base
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs calloc (returns NULL on failure); C11 7.22.3.2 (cannot allocate an object larger than SIZE_MAX)
 * note:  a wrapped count*size would yield a small successful allocation, which must not happen.
 */
#include "tc.h"

TC_DECL_PROC(void *, _calloc_base, (size_t, size_t));
TC_DECL_PROC(void, _free_base, (void *));
static volatile size_t v_a, v_b;

static void run(size_t a, size_t b)
{
    void *p;
    if (!tc_p__calloc_base) TC_SKIP("no _calloc_base");
    v_a = a; v_b = b; p = tc_p__calloc_base(v_a, v_b);
    CHECK(p == NULL);
    if (p) tc_p__free_base(p);
}
TC_CASE(half_plus_one_times_two) { run((size_t)-1 / 2 + 1, 2); }
TC_CASE(max_times_max) { run((size_t)-1, (size_t)-1); }
TC_CASE(two_times_half_plus_one) { run(2, (size_t)-1 / 2 + 1); }
TC_CASE(one_times_max) { run(1, (size_t)-1); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_calloc_base); TC_LOAD_PROC(_free_base);
    TC_RUN(half_plus_one_times_two); TC_RUN(max_times_max); TC_RUN(two_times_half_plus_one); TC_RUN(one_times_max);
    return tc_end();
}
