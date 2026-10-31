/* TC
 * id:    hp_aligned_free_null
 * area:  heap
 * funcs: _aligned_free
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _aligned_free ("If memblock is NULL, the pointer is ignored")
 * note:  the errno-unchanged expectation lives in hp_probe_aligned_free_null_errno (undocumented).
 */
#include "tc.h"
#include <malloc.h>

static void *volatile v_null = NULL;

TC_CASE(null_noop)
{
    _aligned_free(v_null);
    CHECK(1);
}
TC_CASE(null_twice)
{
    _aligned_free(v_null); _aligned_free(v_null);
    CHECK(1);
}
int main(void) { tc_begin(); TC_RUN(null_noop); TC_RUN(null_twice); return tc_end(); }
