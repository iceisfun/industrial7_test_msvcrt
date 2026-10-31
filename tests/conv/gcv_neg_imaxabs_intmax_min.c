/* TC
 * id:    gcv_neg_imaxabs_intmax_min
 * area:  conv
 * funcs:
 * procs: imaxabs
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C99 7.8.2.1 (result undefined when not representable)
 * note:  PROBING: imaxabs(INTMAX_MIN) returns INTMAX_MIN (two's complement wrap), without crash and without
 *        invoking the invalid-parameter handler.
 */
#include "tc.h"
#include <inttypes.h>
#include <stdint.h>

typedef intmax_t (__cdecl *imaxabs_t)(intmax_t);
static imaxabs_t p_imaxabs;
static volatile intmax_t v;

TC_CASE(min_wraps)
{
    intmax_t r;
    tc_iph_install(); tc_iph_count = 0;
    v = INTMAX_MIN; r = p_imaxabs(v);
    CHECK_EQ_I(r, INTMAX_MIN);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void) { tc_begin(); p_imaxabs = (imaxabs_t)tc_crt_proc("imaxabs"); if (!p_imaxabs) { tc_write("TC FAIL " TC_TEST_ID "::imaxabs not exported\n"); return 1; } TC_RUN(min_wraps); return tc_end(); }
