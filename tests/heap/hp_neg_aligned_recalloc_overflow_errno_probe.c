/* TC
 * id:    hp_neg_aligned_recalloc_overflow_errno_probe
 * area:  heap
 * kind:  negative
 * funcs: _aligned_recalloc _aligned_free
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _recalloc (overflow of num*size sets ENOMEM); _aligned_recalloc docs are silent on errno
 * note:  PROBING: _aligned_recalloc(NULL, SIZE_MAX, 2, 16) returns NULL with errno == ENOMEM (by analogy with _recalloc/calloc).
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_num, v_size, v_al;

TC_CASE(overflow_errno)
{
    void *p;
    v_num = SIZE_MAX; v_size = 2; v_al = 16; errno = 0;
    p = _aligned_recalloc(NULL, v_num, v_size, v_al);
    CHECK(p == NULL);
    if (p) { _aligned_free(p); return; }
    CHECK_EQ_I(errno, ENOMEM);
}
int main(void) { tc_begin(); TC_RUN(overflow_errno); return tc_end(); }
