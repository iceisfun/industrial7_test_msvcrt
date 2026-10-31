/* TC
 * id:    hp_calloc_zeroed
 * area:  heap
 * funcs: calloc free
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.3.2

 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static void fillp(unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(seed + i * 7 + (i >> 8)); }
static int chkp(const unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(seed + i * 7 + (i >> 8))) return 0; return 1; }
static volatile size_t vs;

static int zero(const unsigned char *p, size_t n) { size_t i; for (i = 0; i < n; i++) if (p[i]) return 0; return 1; }
TC_CASE(small) { unsigned char *p; vs = 100; p = (unsigned char *)calloc(vs, 1); CHECK(p != NULL); if (p) CHECK(zero(p, 100)); free(p); }
TC_CASE(ints) { unsigned char *p; vs = 10; p = (unsigned char *)calloc(vs, sizeof(int) * 10); CHECK(p != NULL); if (p) CHECK(zero(p, 10 * sizeof(int) * 10)); free(p); }
TC_CASE(mb) { unsigned char *p; vs = 1024 * 1024; p = (unsigned char *)calloc(vs, 1); CHECK(p != NULL); if (p) CHECK(zero(p, 1024 * 1024)); free(p); }
TC_CASE(reused_block)
{
    int r;
    for (r = 0; r < 20; r++) {
        unsigned char *p; vs = 256; p = (unsigned char *)malloc(vs); CHECK(p != NULL); if (!p) return;
        memset(p, 0xEE, 256); free(p);
        p = (unsigned char *)calloc(vs, 1); CHECK(p != NULL); if (!p) return;
        CHECK(zero(p, 256)); memset(p, 0xEE, 256); free(p);
    }
}
int main(void) { tc_begin(); TC_RUN(small); TC_RUN(ints); TC_RUN(mb); TC_RUN(reused_block); return tc_end(); }
