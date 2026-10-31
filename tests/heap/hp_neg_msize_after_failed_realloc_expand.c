/* TC
 * id:    hp_neg_msize_after_failed_realloc_expand
 * area:  heap
 * funcs: _msize realloc _expand malloc free
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   C11 7.22.3.5 (failed realloc leaves old object unchanged); MS docs realloc, _expand (original block unchanged on failure)
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_48 = 48, v_huge = SIZE_MAX / 2;

TC_CASE(failed_realloc)
{
    unsigned char *p, *r; size_t s0; int i;
    p = (unsigned char *)malloc(v_48);
    CHECK(p != NULL); if (!p) return;
    for (i = 0; i < 48; i++) p[i] = (unsigned char)(i ^ 0x5a);
    s0 = _msize(p);
    CHECK(s0 >= 48);
    r = (unsigned char *)realloc(p, v_huge);
    CHECK(r == NULL);
    if (r) { free(r); return; }
    CHECK_EQ_U(_msize(p), s0);
    for (i = 0; i < 48; i++) CHECK_EQ_I(p[i], i ^ 0x5a);
    free(p);
}
TC_CASE(failed_expand)
{
    unsigned char *p, *r; size_t s0; int i;
    p = (unsigned char *)malloc(v_48);
    CHECK(p != NULL); if (!p) return;
    for (i = 0; i < 48; i++) p[i] = (unsigned char)(i ^ 0x3c);
    s0 = _msize(p);
    r = (unsigned char *)_expand(p, v_huge);
    CHECK(r == NULL);
    if (r) { free(r); return; }
    CHECK_EQ_U(_msize(p), s0);
    for (i = 0; i < 48; i++) CHECK_EQ_I(p[i], i ^ 0x3c);
    free(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(failed_realloc); TC_RUN(failed_expand);
    return tc_end();
}
