/* TC
 * id:    hp_malloc_alignment16
 * area:  heap
 * funcs: malloc calloc realloc free
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs malloc: 8 bytes on x86, 16 on x64

 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static void fillp(unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(seed + i * 7 + (i >> 8)); }
static int chkp(const unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(seed + i * 7 + (i >> 8))) return 0; return 1; }
static volatile size_t vs;

#define AL (TC_X64 ? 16 : 8)
TC_CASE(malloc_1_64)
{
    size_t n; for (n = 1; n <= 64; n++) { void *p; vs = n; p = malloc(vs); CHECK(p != NULL); CHECK_EQ_U((uintptr_t)p % AL, 0); free(p); }
    { void *p; vs = 1000; p = malloc(vs); CHECK_EQ_U((uintptr_t)p % AL, 0); free(p); }
}
TC_CASE(malloc_live_set)
{
    void *p[64]; size_t n;
    for (n = 1; n <= 64; n++) { vs = n; p[n - 1] = malloc(vs); CHECK(p[n - 1] != NULL); CHECK_EQ_U((uintptr_t)p[n - 1] % AL, 0); }
    for (n = 0; n < 64; n++) free(p[n]);
}
TC_CASE(calloc_realloc)
{
    size_t n;
    for (n = 1; n <= 64; n++) {
        void *p, *q; vs = n; p = calloc(1, vs); CHECK(p != NULL); CHECK_EQ_U((uintptr_t)p % AL, 0);
        vs = n * 3 + 1; q = realloc(p, vs); CHECK(q != NULL); CHECK_EQ_U((uintptr_t)q % AL, 0); free(q);
    }
}
TC_CASE(large)
{
    void *p; vs = 1024 * 1024; p = malloc(vs); CHECK(p != NULL); CHECK_EQ_U((uintptr_t)p % AL, 0); free(p);
}
int main(void) { tc_begin(); TC_RUN(malloc_1_64); TC_RUN(malloc_live_set); TC_RUN(calloc_realloc); TC_RUN(large); return tc_end(); }
