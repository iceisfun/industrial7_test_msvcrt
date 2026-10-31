/* TC
 * id:    hp_malloc_zero_size
 * area:  heap
 * funcs: malloc free
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs malloc: if size is 0, malloc allocates a zero-length item and returns a valid pointer

 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static void fillp(unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(seed + i * 7 + (i >> 8)); }
static int chkp(const unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(seed + i * 7 + (i >> 8))) return 0; return 1; }
static volatile size_t vs;

TC_CASE(nonnull) { void *p; vs = 0; p = malloc(vs); CHECK(p != NULL); free(p); }
TC_CASE(distinct) { void *a, *b; vs = 0; a = malloc(vs); b = malloc(vs); CHECK(a != NULL); CHECK(b != NULL); CHECK(a != b); free(a); free(b); }
int main(void) { tc_begin(); TC_RUN(nonnull); TC_RUN(distinct); return tc_end(); }
