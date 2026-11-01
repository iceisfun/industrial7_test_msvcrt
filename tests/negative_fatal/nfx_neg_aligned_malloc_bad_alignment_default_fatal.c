/* TC
 * id:    nfx_neg_aligned_malloc_bad_alignment_default_fatal
 * area:  negative_fatal
 * kind:  negative
 * funcs: _aligned_malloc
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _aligned_malloc (alignment must be an integer power of two; invalid parameter handler is invoked), _set_invalid_parameter_handler (default handler terminates)
 * note:  Child calls _aligned_malloc(16, 3) with the default invalid-parameter handler: the process must terminate abnormally.
 *        Exact status is not asserted here (see the nfx_neg_iph_default_* tests); only: not 0, not the survival marker 77.
 */
#include "tc.h"
#include <malloc.h>

TC_CASE(non_power_of_two_alignment_is_fatal)
{
    DWORD code = 0; int ran = tc_spawn_self("badalign", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}
TC_CASE(power_of_two_alignment_is_ok)
{
    DWORD code = 99; int ran = tc_spawn_self("goodalign", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 0);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) {
        tc_iph_default();
        volatile size_t sz = 16, al = (c[0] == 'b') ? 3 : 16;
        void *p = _aligned_malloc(sz, al);
        if (c[0] == 'b') return 77;
        if (!p || ((ULONG_PTR)p & 15)) return 1;
        _aligned_free(p);
        return 0;
    }
    TC_RUN(non_power_of_two_alignment_is_fatal); TC_RUN(power_of_two_alignment_is_ok);
    return tc_end();
}
