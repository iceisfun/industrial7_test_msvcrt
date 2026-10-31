/* TC
 * id:    hp_calloc_zero_args
 * area:  heap
 * funcs: calloc free
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   MS docs calloc (zero-size behaviour not explicitly documented; malloc(0) is)
 * note:  PROBING that calloc with a zero count and/or zero size returns non-NULL (believed true on all MS CRTs).

 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static void fillp(unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(seed + i * 7 + (i >> 8)); }
static int chkp(const unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(seed + i * 7 + (i >> 8))) return 0; return 1; }
static volatile size_t vs;

TC_CASE(zero_count) { void *p; vs = 0; p = calloc(vs, 16); CHECK(p != NULL); free(p); }
TC_CASE(zero_size) { void *p; vs = 0; p = calloc(16, vs); CHECK(p != NULL); free(p); }
TC_CASE(both_zero) { void *p; vs = 0; p = calloc(vs, vs); CHECK(p != NULL); free(p); }
int main(void) { tc_begin(); TC_RUN(zero_count); TC_RUN(zero_size); TC_RUN(both_zero); return tc_end(); }
