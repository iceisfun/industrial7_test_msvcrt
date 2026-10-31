/* TC
 * id:    hp_free_null
 * area:  heap
 * funcs: free malloc
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.3.3: if ptr is a null pointer, no action occurs
 * note:  errno-unchanged assertion moved to hp_probe_free_null_errno (standard allows libs to set errno).

 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static void fillp(unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) p[i] = (unsigned char)(seed + i * 7 + (i >> 8)); }
static int chkp(const unsigned char *p, size_t n, unsigned seed) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)(seed + i * 7 + (i >> 8))) return 0; return 1; }
static volatile size_t vs;

TC_CASE(noop_repeated) { void *volatile n = NULL; free(n); free(n); free(n); }
TC_CASE(malloc_after) { void *p; free(NULL); vs = 32; p = malloc(vs); CHECK(p != NULL); free(p); }
int main(void) { tc_begin(); TC_RUN(noop_repeated); TC_RUN(malloc_after); return tc_end(); }
