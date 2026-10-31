/* TC
 * id:    ucr_new_handler_malloc_probe
 * area:  ucrt_only
 * funcs: _set_new_mode malloc
 * procs: _set_new_handler
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_new_mode, _set_new_handler
 * note:  PROBE: with new mode 1, a failing malloc of 128 TiB (below _HEAP_MAXREQ) calls the new handler; a handler returning 0 makes malloc return NULL.
 */
#include "tc.h"
#include <stdlib.h>
#include <new.h>

typedef int (__cdecl *nh_t)(size_t);
TC_DECL_PROC(nh_t, _set_new_handler, (nh_t));

static volatile int g_cnt;
static int __cdecl nh(size_t n) { (void)n; g_cnt++; return 0; }
#if TC_X64
static volatile size_t big = (size_t)1 << 47;   /* 128 TiB: above any user address space, below _HEAP_MAXREQ */
#else
static volatile size_t big = (size_t)0x7FF00000u;
#endif

TC_CASE(mode1_calls_handler)
{
    nh_t prev; void *p;
    if (!tc_p__set_new_handler) TC_SKIP("no _set_new_handler");
    g_cnt = 0;
    prev = tc_p__set_new_handler(nh);
    CHECK(prev == NULL);
    _set_new_mode(1);
    p = malloc(big);
    _set_new_mode(0);
    CHECK(p == NULL); CHECK_EQ_I(g_cnt, 1);
    if (p) free(p);
    CHECK(tc_p__set_new_handler(prev) == nh);
}
TC_CASE(mode0_no_handler)
{
    nh_t prev; void *p;
    if (!tc_p__set_new_handler) TC_SKIP("no _set_new_handler");
    g_cnt = 0;
    prev = tc_p__set_new_handler(nh);
    _set_new_mode(0);
    p = malloc(big);
    CHECK(p == NULL); CHECK_EQ_I(g_cnt, 0);
    if (p) free(p);
    tc_p__set_new_handler(prev);
}

int main(void) { tc_begin(); TC_LOAD_PROC(_set_new_handler); TC_RUN(mode1_calls_handler); TC_RUN(mode0_no_handler); return tc_end(); }
