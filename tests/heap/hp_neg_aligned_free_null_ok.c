/* TC
 * id:    hp_neg_aligned_free_null_ok
 * area:  heap
 * funcs: _aligned_free free
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_free; C free(NULL)
 * note:  free(NULL) and _aligned_free(NULL) must be harmless: no crash, no invalid-parameter call, errno unchanged.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

static void *volatile v_null;

TC_CASE(free_null)
{
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    free(v_null);
    CHECK_EQ_I(errno, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(aligned_free_null)
{
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    _aligned_free(v_null);
    CHECK_EQ_I(errno, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(free_null); TC_RUN(aligned_free_null); return tc_end(); }
