/* TC
 * id:    hp_recalloc_grow_zero
 * area:  heap
 * funcs: _recalloc _msize free
 * procs: 
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _recalloc
 * note:  msvcr80/msvcrt lack _recalloc. Only bytes at or beyond the old block _msize are asserted zero (the CRT zeroes the growth
 *        beyond the old usable size; the slack inside the old block was never requested).
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

static volatile size_t v_n, v_s;

TC_CASE(null_like_calloc)
{
    unsigned char *p; size_t i, nz = 0;
    v_n = 4; v_s = 8; p = (unsigned char *)_recalloc(NULL, v_n, v_s);
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 32; i++) nz += p[i] != 0;
    CHECK_EQ_U(nz, 0);
    free(p);
}
TC_CASE(grow_zero_fills)
{
    unsigned char *p, *q; size_t i, om, bad = 0, nz = 0;
    v_n = 16; v_s = 1; p = (unsigned char *)_recalloc(NULL, v_n, v_s);
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 16; i++) p[i] = 0xAA;
    om = _msize(p);   /* bytes in [16,om) were never requested: not asserted */
    v_n = 64; q = (unsigned char *)_recalloc(p, v_n, v_s);
    CHECK(q != NULL);
    if (!q) { free(p); return; }
    for (i = 0; i < 16; i++) bad += q[i] != 0xAA;
    for (i = (om > 16 ? om : 16); i < 64; i++) nz += q[i] != 0;
    CHECK_EQ_U(bad, 0);
    CHECK_EQ_U(nz, 0);
    free(q);
}
TC_CASE(shrink_keeps_prefix)
{
    unsigned char *p, *q; size_t i, bad = 0;
    v_n = 64; v_s = 1; p = (unsigned char *)_recalloc(NULL, v_n, v_s);
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 64; i++) p[i] = (unsigned char)(i + 1);
    v_n = 8; q = (unsigned char *)_recalloc(p, v_n, v_s);
    CHECK(q != NULL);
    if (!q) { free(p); return; }
    for (i = 0; i < 8; i++) bad += q[i] != (unsigned char)(i + 1);
    CHECK_EQ_U(bad, 0);
    free(q);
}
int main(void) { tc_begin(); TC_RUN(null_like_calloc); TC_RUN(grow_zero_fills); TC_RUN(shrink_keeps_prefix); return tc_end(); }
