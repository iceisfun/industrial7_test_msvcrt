/* TC
 * id:    exp_alloc_exported
 * area:  exports
 * funcs: malloc calloc realloc free _msize _expand
 * procs: _aligned_malloc _aligned_free _aligned_realloc _aligned_offset_malloc
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs malloc, _aligned_malloc, _msize, _expand
 * note:  All allocator entry points, plus a tiny functional round trip.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <malloc.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#define HAS(n) do { if (!tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "not exported: " n); } while (0)
#define HASNOT(n) do { if (tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "unexpectedly exported: " n); } while (0)

/* taking the address makes the linker import each name from the CRT under test */
static void *volatile tc_sink;
static void touch_imports(void)
{
    tc_sink = (void *)malloc;
    tc_sink = (void *)calloc;
    tc_sink = (void *)realloc;
    tc_sink = (void *)free;
    tc_sink = (void *)_msize;
    tc_sink = (void *)_expand;
}

typedef void *(__cdecl *am_t)(size_t, size_t);
typedef void (__cdecl *af_t)(void *);
static am_t p_am; static af_t p_af;

TC_CASE(basic_group)
{
    HAS("malloc");
    HAS("calloc");
    HAS("realloc");
    HAS("free");
    HAS("_msize");
    HAS("_expand");
}

TC_CASE(aligned_group)
{
    HAS("_aligned_malloc");
    HAS("_aligned_free");
    HAS("_aligned_realloc");
    HAS("_aligned_offset_malloc");
}

TC_CASE(malloc_free_roundtrip)
{
    volatile size_t n = 100;
    unsigned char *p = (unsigned char *)malloc(n);
    CHECK(p != NULL);
    if (!p) return;
    memset(p, 0xA5, n);
    CHECK(_msize(p) >= 100);
    p = (unsigned char *)realloc(p, 400);
    CHECK(p != NULL);
    if (!p) return;
    CHECK_EQ_U(p[0], 0xA5);
    CHECK_EQ_U(p[99], 0xA5);
    free(p);
}

TC_CASE(aligned_malloc_alignment_ok)
{
    void *p;
    if (!p_am || !p_af) TC_SKIP("aligned functions not exported");
    p = p_am(100, 64);
    CHECK(p != NULL);
    if (p) { CHECK_EQ_U((uintptr_t)p & 63, 0); memset(p, 1, 100); p_af(p); }
}

int main(void)
{
    tc_begin();
    touch_imports();
    p_am = (am_t)tc_crt_proc("_aligned_malloc");
    p_af = (af_t)tc_crt_proc("_aligned_free");
    TC_RUN(basic_group);
    TC_RUN(aligned_group);
    TC_RUN(malloc_free_roundtrip);
    TC_RUN(aligned_malloc_alignment_ok);
    return tc_end();
}
