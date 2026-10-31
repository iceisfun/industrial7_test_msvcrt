/* TC
 * id:    hp_neg_malloc_huge
 * area:  heap
 * funcs: malloc free
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs malloc: returns NULL if insufficient memory; sets errno to ENOMEM
 * note:  ENOMEM is documented on malloc return-value page. errno asserted only for msvcr80+ (VC6-era msvcrt.dll may not set it).
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static void fillp(unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(seed + i * 7 + (i >> 8)); }
static int chkp(const unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(seed + i * 7 + (i >> 8))) return 0; return 1; }
static volatile size_t vs;

TC_CASE(size_max) { void *p; vs = (size_t)-1; errno = 0; p = malloc(vs); CHECK(p == NULL); if (TC_AT_LEAST(80)) CHECK_EQ_I(errno, ENOMEM); free(p); }
TC_CASE(near_max) { void *p; vs = (size_t)-1 - 100; errno = 0; p = malloc(vs); CHECK(p == NULL); if (TC_AT_LEAST(80)) CHECK_EQ_I(errno, ENOMEM); free(p); }
TC_CASE(half_addr_space) { void *p; vs = ((size_t)-1) / 2; errno = 0; p = malloc(vs); CHECK(p == NULL); free(p); }
TC_CASE(still_works) { void *p; vs = 64; p = malloc(vs); CHECK(p != NULL); free(p); }
int main(void) { tc_begin(); TC_RUN(size_max); TC_RUN(near_max); TC_RUN(half_addr_space); TC_RUN(still_works); return tc_end(); }
