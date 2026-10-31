/* TC
 * id:    hp_malloc_basic
 * area:  heap
 * funcs: malloc free
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.3.4; MS docs malloc

 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static void fillp(unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(seed + i * 7 + (i >> 8)); }
static int chkp(const unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(seed + i * 7 + (i >> 8))) return 0; return 1; }
static volatile size_t vs;

TC_CASE(sizes_writable)
{
    static const size_t sz[] = {1, 2, 7, 8, 16, 100, 1000};
    unsigned char *p[7]; int i;
    for (i = 0; i < 7; i++) { vs = sz[i]; p[i] = (unsigned char *)malloc(vs); CHECK(p[i] != NULL); if (p[i]) fillp(p[i], sz[i], (unsigned)i * 31); }
    for (i = 0; i < 7; i++) if (p[i]) CHECK(chkp(p[i], sz[i], (unsigned)i * 31));
    for (i = 0; i < 7; i++) free(p[i]);
}
TC_CASE(no_overlap)
{
    unsigned char *p[16]; int i, j;
    for (i = 0; i < 16; i++) { vs = 50 + i; p[i] = (unsigned char *)malloc(vs); CHECK(p[i] != NULL); if (p[i]) memset(p[i], i + 1, 50 + i); }
    for (i = 0; i < 16; i++) if (p[i]) { for (j = 0; j < 50 + i; j++) if (p[i][j] != i + 1) { CHECK(p[i][j] == i + 1); break; } }
    for (i = 0; i < 16; i++) free(p[i]);
}
int main(void) { tc_begin(); TC_RUN(sizes_writable); TC_RUN(no_overlap); return tc_end(); }
