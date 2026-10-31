/* TC
 * id:    hp_msize_ge_requested
 * area:  heap
 * funcs: _msize malloc calloc free
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _msize
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

static volatile size_t v_n;
static const size_t sizes[] = { 1, 15, 16, 17, 100, 1000, 100000 };
#define NS (sizeof sizes / sizeof sizes[0])

TC_CASE(malloc_msize)
{
    size_t i;
    for (i = 0; i < NS; i++) {
        void *p; v_n = sizes[i]; p = malloc(v_n);
        CHECK(p != NULL);
        if (p) { CHECK(_msize(p) >= sizes[i]); free(p); }
    }
}
TC_CASE(calloc_msize)
{
    size_t i;
    for (i = 0; i < NS; i++) {
        void *p; v_n = sizes[i]; p = calloc(v_n, 1);
        CHECK(p != NULL);
        if (p) { CHECK(_msize(p) >= sizes[i]); free(p); }
    }
}
TC_CASE(full_extent_writable)
{
    size_t i;
    for (i = 0; i < NS; i++) {
        unsigned char *p; size_t m, k; v_n = sizes[i]; p = (unsigned char *)malloc(v_n);
        CHECK(p != NULL);
        if (!p) continue;
        m = _msize(p);
        for (k = 0; k < m; k++) p[k] = (unsigned char)k;
        for (k = 0; k < m; k++) if (p[k] != (unsigned char)k) { CHECK(0); break; }
        free(p);
    }
}
int main(void) { tc_begin(); TC_RUN(malloc_msize); TC_RUN(calloc_msize); TC_RUN(full_extent_writable); return tc_end(); }
