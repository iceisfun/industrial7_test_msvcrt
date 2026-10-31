/* TC
 * id:    gcv_imaxabs_basic
 * area:  conv
 * funcs:
 * procs: imaxabs
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C99 7.8.2.1; MS docs imaxabs
 * note:  Not exported by msvcr110 and older (see exp_neg_probe_pre120_c99_stdlib_absent).
 */
#include "tc.h"
#include <inttypes.h>
#include <stdint.h>

typedef intmax_t (__cdecl *imaxabs_t)(intmax_t);
static imaxabs_t p_imaxabs;
static volatile intmax_t v;

TC_CASE(small) { v = 0; CHECK_EQ_I(p_imaxabs(v), 0); v = 1; CHECK_EQ_I(p_imaxabs(v), 1); v = -1; CHECK_EQ_I(p_imaxabs(v), 1); }
TC_CASE(mid) { v = 123456789012LL; CHECK_EQ_I(p_imaxabs(v), 123456789012LL); v = -123456789012LL; CHECK_EQ_I(p_imaxabs(v), 123456789012LL); }
TC_CASE(max) { v = INTMAX_MAX; CHECK_EQ_I(p_imaxabs(v), INTMAX_MAX); v = -INTMAX_MAX; CHECK_EQ_I(p_imaxabs(v), INTMAX_MAX); }
TC_CASE(wide) { v = -0x100000001LL; CHECK_EQ_I(p_imaxabs(v), 0x100000001LL); v = 0x100000001LL; CHECK_EQ_I(p_imaxabs(v), 0x100000001LL); }

int main(void) { tc_begin(); p_imaxabs = (imaxabs_t)tc_crt_proc("imaxabs"); if (!p_imaxabs) { tc_write("TC FAIL " TC_TEST_ID "::imaxabs not exported\n"); return 1; } TC_RUN(small); TC_RUN(mid); TC_RUN(max); TC_RUN(wide); return tc_end(); }
