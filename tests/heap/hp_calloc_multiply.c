/* TC
 * id:    hp_calloc_multiply
 * area:  heap
 * funcs: calloc _msize free
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs calloc, _msize

 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static void fillp(unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(seed + i * 7 + (i >> 8)); }
static int chkp(const unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(seed + i * 7 + (i >> 8))) return 0; return 1; }
static volatile size_t vs;

TC_CASE(msize_7_13) { void *p; vs = 7; p = calloc(vs, 13); CHECK(p != NULL); if (p) CHECK(_msize(p) >= 91); free(p); }
TC_CASE(full_extent)
{
    unsigned char *p; size_t i, bad = 0; vs = 1000; p = (unsigned char *)calloc(vs, 1000); CHECK(p != NULL);
    if (p) { CHECK(_msize(p) >= 1000000); for (i = 0; i < 1000000; i++) p[i] = (unsigned char)i; for (i = 0; i < 1000000; i++) if (p[i] != (unsigned char)i) bad++; CHECK_EQ_U(bad, 0); }
    free(p);
}
int main(void) { tc_begin(); TC_RUN(msize_7_13); TC_RUN(full_extent); return tc_end(); }
