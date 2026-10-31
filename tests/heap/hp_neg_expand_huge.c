/* TC
 * id:    hp_neg_expand_huge
 * area:  heap
 * funcs: _expand malloc free
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _expand (ENOMEM)
 * note:  errno=ENOMEM only asserted on ucrtbase.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_n;

TC_CASE(expand_huge)
{
    unsigned char *p, *q; int i, bad = 0;
    p = (unsigned char *)malloc(48); CHECK(p != NULL); if (!p) return;
    for (i = 0; i < 48; i++) p[i] = (unsigned char)(i + 9);
    errno = 0; v_n = SIZE_MAX; q = (unsigned char *)_expand(p, v_n);
    CHECK(q == NULL); if (TC_AT_LEAST(140)) CHECK_EQ_I(errno, ENOMEM);
    errno = 0; v_n = SIZE_MAX - 0x1F; q = (unsigned char *)_expand(p, v_n);
    CHECK(q == NULL); if (TC_AT_LEAST(140)) CHECK_EQ_I(errno, ENOMEM);
    for (i = 0; i < 48; i++) if (p[i] != (unsigned char)(i + 9)) bad++;
    CHECK_EQ_I(bad, 0);
    CHECK(_msize(p) >= 48);
    free(p);
}
int main(void) { tc_begin(); TC_RUN(expand_huge); return tc_end(); }
