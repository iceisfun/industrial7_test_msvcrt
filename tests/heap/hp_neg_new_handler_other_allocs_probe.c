/* TC
 * id:    hp_neg_new_handler_other_allocs_probe
 * area:  heap
 * kind:  negative
 * funcs: calloc realloc free _set_new_mode _set_new_handler _query_new_mode
 * procs: _recalloc
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _set_new_mode (applies to malloc "and related" allocators); calloc/realloc/_recalloc return NULL on failure
 * note:  PROBING whether calloc, realloc and _recalloc also invoke the new handler when new mode is 1. The handler call count is
 *        printed as TC INFO only. Asserted (documented): each failing request returns NULL and realloc leaves the old block intact.
 *        The handler returns 0 so no retry loop happens.
 */
#include "tc.h"
#include <stdlib.h>
#include <stdint.h>
#include <new.h>

typedef void *(__cdecl *recalloc_t)(void *, size_t, size_t);
static volatile size_t v_huge = SIZE_MAX / 2, v_one = 1;
static int hcount;
static int __cdecl handler(size_t n) { (void)n; hcount++; return 0; }
static void info(const char *n, int v) { char b[32]; tc_i64(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(n); tc_write("="); tc_write(b); tc_write("\n"); }

TC_CASE(calloc_path)
{
    int old = _query_new_mode(); void *p;
    _set_new_mode(1); _set_new_handler(handler); hcount = 0;
    p = calloc(v_one, v_huge);
    _set_new_handler(NULL); _set_new_mode(old);
    CHECK(p == NULL); if (p) free(p);
    info("calloc_calls", hcount);
}
TC_CASE(realloc_path)
{
    int old = _query_new_mode(); void *p = malloc(v_one + 15), *q; unsigned char *c = (unsigned char *)p;
    CHECK(p != NULL); if (!p) return;
    c[0] = 0x5A;
    _set_new_mode(1); _set_new_handler(handler); hcount = 0;
    q = realloc(p, v_huge);
    _set_new_handler(NULL); _set_new_mode(old);
    CHECK(q == NULL);
    if (q) { free(q); return; }
    CHECK_EQ_I(c[0], 0x5A);
    free(p);
    info("realloc_calls", hcount);
}
TC_CASE(recalloc_path)
{
    recalloc_t rc = (recalloc_t)tc_crt_proc("_recalloc");
    int old = _query_new_mode(); void *p, *q;
    if (!rc) TC_SKIP("_recalloc not exported");
    p = malloc(v_one + 15);
    CHECK(p != NULL); if (!p) return;
    _set_new_mode(1); _set_new_handler(handler); hcount = 0;
    q = rc(p, v_one, v_huge);
    _set_new_handler(NULL); _set_new_mode(old);
    CHECK(q == NULL);
    if (q) free(q); else free(p);
    info("recalloc_calls", hcount);
}
int main(void) { tc_begin(); TC_RUN(calloc_path); TC_RUN(realloc_path); TC_RUN(recalloc_path); return tc_end(); }
