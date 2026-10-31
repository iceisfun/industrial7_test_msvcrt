/* TC
 * id:    hp_probe_msize_zero_block
 * area:  heap
 * funcs: malloc _msize free
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _msize
 * note:  PROBING whether _msize(malloc(0)) == 1 on ucrtbase (malloc(0) is believed to be bumped to a 1-byte request and _msize is HeapSize-based). Docs do not state the value; old CRTs with the small-block heap may round differently so they are excluded.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static void fillp(unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(seed + i * 7 + (i >> 8)); }
static int chkp(const unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(seed + i * 7 + (i >> 8))) return 0; return 1; }
static volatile size_t vs;

TC_CASE(msize_of_malloc0) { void *p; vs = 0; p = malloc(vs); CHECK(p != NULL); CHECK_EQ_U(_msize(p), 1); free(p); }
int main(void) { tc_begin(); TC_RUN(msize_of_malloc0); return tc_end(); }
