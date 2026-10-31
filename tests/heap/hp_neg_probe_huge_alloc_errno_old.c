/* TC
 * id:    hp_neg_probe_huge_alloc_errno_old
 * area:  heap
 * kind:  negative
 * funcs: malloc calloc realloc free
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs malloc, calloc, realloc (errno ENOMEM on failure)
 * note:  PROBING that VS2010-2013 CRTs set errno=ENOMEM when a request exceeds _HEAP_MAXREQ / overflows (documented for the
 *        UCRT in hp_neg_huge_alloc_fails; unverified for these older CRTs). NULL return is asserted in the documented test.
 */
#include "tc.h"
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

TC_CASE(malloc_max_errno)
{
    volatile size_t n = SIZE_MAX; void *volatile p;
    errno = 0; p = malloc(n);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, ENOMEM);
    free(p);
}
TC_CASE(calloc_overflow_errno)
{
    volatile size_t a = SIZE_MAX / 2, b = 4; void *volatile p;
    errno = 0; p = calloc(a, b);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, ENOMEM);
    free(p);
}
TC_CASE(realloc_max_errno)
{
    char *volatile p = (char *)malloc(32); volatile size_t n = SIZE_MAX; char *volatile q;
    CHECK(p != NULL);
    if (!p) return;
    errno = 0; q = (char *)realloc(p, n);
    CHECK(q == NULL);
    CHECK_EQ_I(errno, ENOMEM);
    free(p);
}

int main(void) { tc_begin(); TC_RUN(malloc_max_errno); TC_RUN(calloc_overflow_errno); TC_RUN(realloc_max_errno); return tc_end(); }
