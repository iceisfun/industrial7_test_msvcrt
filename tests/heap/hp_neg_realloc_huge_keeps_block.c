/* TC
 * id:    hp_neg_realloc_huge_keeps_block
 * area:  heap
 * funcs: realloc malloc free
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.3.5; MS docs realloc (ENOMEM, original block not freed)
 * note:  errno asserted only on ucrtbase.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

static volatile size_t v_n;

TC_CASE(keeps_block)
{
    unsigned char *p, *q; size_t i; int bad = 0;
    v_n = 64; p = (unsigned char *)malloc(v_n);
    CHECK(p != NULL); if (!p) return;
    for (i = 0; i < 64; i++) p[i] = (unsigned char)(i ^ 0x5c);
    errno = 0; v_n = SIZE_MAX; q = (unsigned char *)realloc(p, v_n);
    CHECK(q == NULL); if (TC_AT_LEAST(140)) CHECK_EQ_I(errno, ENOMEM);
    if (q) p = q;
    for (i = 0; i < 64; i++) if (p[i] != (unsigned char)(i ^ 0x5c)) bad++;
    CHECK_EQ_I(bad, 0);
    errno = 0; v_n = SIZE_MAX - 100; q = (unsigned char *)realloc(p, v_n);
    CHECK(q == NULL); if (TC_AT_LEAST(140)) CHECK_EQ_I(errno, ENOMEM);
    if (q) p = q;
    for (i = 0; i < 64; i++) if (p[i] != (unsigned char)(i ^ 0x5c)) bad++;
    CHECK_EQ_I(bad, 0);
    p[63] = 1;   /* still writable */
    free(p);
}
int main(void) { tc_begin(); TC_RUN(keeps_block); return tc_end(); }
