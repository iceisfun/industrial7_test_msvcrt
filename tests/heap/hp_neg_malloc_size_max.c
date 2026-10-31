/* TC
 * id:    hp_neg_malloc_size_max
 * area:  heap
 * funcs: malloc free
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs malloc (ENOMEM); _HEAP_MAXREQ
 * note:  NULL return asserted on all CRTs; errno=ENOMEM only on ucrtbase (older CRTs: see hp_neg_probe_huge_alloc_errno_old).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_n;

#define FAILS(expr_n) do { void *p; errno = 0; v_n = (expr_n); p = malloc(v_n); CHECK(p == NULL);\
    if (TC_AT_LEAST(140)) CHECK_EQ_I(errno, ENOMEM); if (p) free(p); } while (0)

TC_CASE(size_max) { FAILS(SIZE_MAX); }
/* SIZE_MAX/2+1 only on x64: on x86 2GB is below _HEAP_MAXREQ and could in principle succeed */
TC_CASE(near_max)
{
    FAILS(SIZE_MAX - 1); FAILS(SIZE_MAX - 0x1F);
#ifdef _WIN64
    FAILS(SIZE_MAX / 2 + 1);
#endif
}
/* 1<<46 omitted: hangs the Wine smoke runner (finding); 1<<47 exceeds the 128TB user address space, 1<<42 exceeds any commit limit */
TC_CASE(huge_64) { if (!TC_X64) TC_SKIP("x64 only"); 
#ifdef _WIN64
    FAILS((size_t)1 << 47);
    FAILS((size_t)1 << 42);
#endif
}
TC_CASE(still_healthy) { void *p; v_n = 16; p = malloc(v_n); CHECK(p != NULL); free(p); }
int main(void) { tc_begin(); TC_RUN(size_max); TC_RUN(near_max); TC_RUN(huge_64); TC_RUN(still_healthy); return tc_end(); }
