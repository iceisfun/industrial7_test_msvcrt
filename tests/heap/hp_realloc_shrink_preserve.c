/* TC
 * id:    hp_realloc_shrink_preserve
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

static void shrink(size_t from, size_t to, unsigned seed)
{
    unsigned char *p, *q; vs = from; p = (unsigned char *)malloc(vs); CHECK(p != NULL); if (!p) return;
    fillp(p, from, seed); vs = to; q = (unsigned char *)realloc(p, vs);
    CHECK(q != NULL); if (!q) { free(p); return; }
    CHECK(chkp(q, to, seed)); free(q);
}
TC_CASE(k_to_10) { shrink(1000, 10, 1); }
TC_CASE(100k_to_1) { shrink(100000, 1, 2); }
TC_CASE(same_size) { shrink(500, 500, 3); shrink(1, 1, 4); }
int main(void) { tc_begin(); TC_RUN(k_to_10); TC_RUN(100k_to_1); TC_RUN(same_size); return tc_end(); }
