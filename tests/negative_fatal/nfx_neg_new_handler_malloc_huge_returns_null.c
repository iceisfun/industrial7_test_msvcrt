/* TC
 * id:    nfx_neg_new_handler_malloc_huge_returns_null
 * area:  negative_fatal
 * funcs: malloc calloc realloc free _set_new_mode
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs malloc/calloc/realloc (ENOMEM; sizes above _HEAP_MAXREQ fail), _set_new_mode
 * note:  Impossible sizes fail with NULL + ENOMEM and never abort.
 */
#include "tc.h"
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
int __cdecl _set_new_mode(int);
#include <string.h>

static volatile size_t big = SIZE_MAX;

TC_CASE(default_mode) { CHECK_EQ_I(_set_new_mode(0), 0); }
TC_CASE(malloc_max) { errno = 0; CHECK(malloc(big) == NULL); CHECK_EQ_I(errno, ENOMEM); }
TC_CASE(calloc_overflow) { errno = 0; CHECK(calloc(big / 2, 4) == NULL); CHECK_EQ_I(errno, ENOMEM); }
TC_CASE(realloc_max)
{
    char *p = malloc(16);
    CHECK(p != NULL); if (!p) return;
    memset(p, 'x', 16);
    errno = 0;
    CHECK(realloc(p, big) == NULL); CHECK_EQ_I(errno, ENOMEM);
    p[15] = 'y'; CHECK(p[0] == 'x' && p[15] == 'y');
    free(p);
}
TC_CASE(mode1_no_handler)
{
    int prev = _set_new_mode(1);
    void *r = malloc(big);
    _set_new_mode(prev);
    CHECK(r == NULL);
    CHECK_EQ_I(_set_new_mode(0), 0);
}
TC_CASE(mode1_no_handler_heap_path)   /* <= _HEAP_MAXREQ: HeapAlloc fails, no handler installed -> NULL */
{
    volatile size_t half = SIZE_MAX / 2; void *r; int prev = _set_new_mode(1);
    r = malloc(half);
    _set_new_mode(prev);
    CHECK(r == NULL);
}
int main(void) { tc_begin(); TC_RUN(default_mode); TC_RUN(malloc_max); TC_RUN(calloc_overflow); TC_RUN(realloc_max); TC_RUN(mode1_no_handler); TC_RUN(mode1_no_handler_heap_path); return tc_end(); }
