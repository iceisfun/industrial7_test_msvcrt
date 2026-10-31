/* TC
 * id:    hp_neg_huge_alloc_fails
 * area:  heap
 * kind:  negative
 * funcs: malloc calloc realloc free
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs malloc, calloc, realloc (return NULL and set errno to ENOMEM when the request cannot be satisfied or exceeds _HEAP_MAXREQ)
 * note:  errno is only asserted for ucrtbase; older CRTs may leave errno untouched on the size-limit path (see hp_neg_probe_huge_alloc_errno_old). A failed realloc must
 *        leave the original block valid.
 */
#include "tc.h"
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

TC_CASE(malloc_max)
{
    volatile size_t n = SIZE_MAX; void *volatile p;
    errno = 0; p = malloc(n);
    CHECK(p == NULL);
    if (TC_AT_LEAST(140)) CHECK_EQ_I(errno, ENOMEM);
    free(p);
}
TC_CASE(calloc_overflow)
{
    volatile size_t a = SIZE_MAX / 2, b = 4; void *volatile p;
    errno = 0; p = calloc(a, b);
    CHECK(p == NULL);
    if (TC_AT_LEAST(140)) CHECK_EQ_I(errno, ENOMEM);
    free(p);
}
TC_CASE(realloc_max_keeps_block)
{
    char *volatile p = (char *)malloc(32); volatile size_t n = SIZE_MAX; char *volatile q;
    CHECK(p != NULL);
    if (!p) return;
    memset(p, 0x5a, 32);
    errno = 0; q = (char *)realloc(p, n);
    CHECK(q == NULL);
    if (TC_AT_LEAST(140)) CHECK_EQ_I(errno, ENOMEM);
    CHECK_EQ_I((unsigned char)p[0], 0x5a);
    CHECK_EQ_I((unsigned char)p[31], 0x5a);
    free(p);
}

int main(void) { tc_begin(); TC_RUN(malloc_max); TC_RUN(calloc_overflow); TC_RUN(realloc_max_keeps_block); return tc_end(); }
