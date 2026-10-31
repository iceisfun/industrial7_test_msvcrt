/* TC
 * id:    hp_neg_recalloc_huge_keeps_block
 * area:  heap
 * funcs: _recalloc malloc free _msize
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _recalloc (returns NULL on failure, errno ENOMEM; original block not freed)
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_cnt = 1, v_huge = SIZE_MAX / 2;

TC_CASE(huge_keeps_block)
{
    unsigned char *p, *r; int i;
    p = (unsigned char *)malloc(32);
    CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 32; i++) p[i] = 0xA5;
    errno = 0;
    r = (unsigned char *)_recalloc(p, v_cnt, v_huge);
    CHECK(r == NULL);
    CHECK_EQ_I(errno, ENOMEM);
    if (r) { free(r); return; }
    for (i = 0; i < 32; i++) CHECK_EQ_I(p[i], 0xA5);
    CHECK(_msize(p) >= 32);
    free(p);
}
TC_CASE(null_huge)
{
    void *r;
    errno = 0;
    r = _recalloc(NULL, v_cnt, v_huge);
    CHECK(r == NULL);
    CHECK_EQ_I(errno, ENOMEM);
    if (r) free(r);
}
int main(void)
{
    tc_begin();
    TC_RUN(huge_keeps_block); TC_RUN(null_huge);
    return tc_end();
}
