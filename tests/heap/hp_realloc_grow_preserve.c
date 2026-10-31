/* TC
 * id:    hp_realloc_grow_preserve
 * area:  heap
 * funcs: realloc malloc free
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.3.5; MS docs realloc

 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static void fillp(unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(seed + i * 7 + (i >> 8)); }
static int chkp(const unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(seed + i * 7 + (i >> 8))) return 0; return 1; }
static volatile size_t vs;

TC_CASE(grow_16_1000)
{
    unsigned char *p, *q; vs = 16; p = (unsigned char *)malloc(vs); CHECK(p != NULL); if (!p) return;
    fillp(p, 16, 3); vs = 1000; q = (unsigned char *)realloc(p, vs); CHECK(q != NULL); if (!q) { free(p); return; }
    CHECK(chkp(q, 16, 3)); memset(q + 16, 1, 984); free(q);
}
TC_CASE(grow_1000_100000)
{
    unsigned char *p, *q; vs = 1000; p = (unsigned char *)malloc(vs); CHECK(p != NULL); if (!p) return;
    fillp(p, 1000, 5); vs = 100000; q = (unsigned char *)realloc(p, vs); CHECK(q != NULL); if (!q) { free(p); return; }
    CHECK(chkp(q, 1000, 5)); memset(q, 0, 100000); free(q);
}
TC_CASE(doubling_chain)
{
    size_t n = 8, old; unsigned char *p; vs = n; p = (unsigned char *)malloc(vs); CHECK(p != NULL); if (!p) return;
    fillp(p, n, 11);
    while (n < 1024 * 1024) {
        unsigned char *q; old = n; n *= 2; vs = n; q = (unsigned char *)realloc(p, vs);
        CHECK(q != NULL); if (!q) { free(p); return; }
        p = q; CHECK(chkp(p, 8, 11)); CHECK(chkp(p, old < 8 ? old : 8, 11));
        fillp(p, n, 11);
    }
    CHECK(chkp(p, n, 11)); free(p);
}
TC_CASE(realloc_null_is_malloc) { unsigned char *p; void *volatile n = NULL; vs = 64; p = (unsigned char *)realloc(n, vs); CHECK(p != NULL); if (p) memset(p, 1, 64); free(p); }
int main(void) { tc_begin(); TC_RUN(grow_16_1000); TC_RUN(grow_1000_100000); TC_RUN(doubling_chain); TC_RUN(realloc_null_is_malloc); return tc_end(); }
