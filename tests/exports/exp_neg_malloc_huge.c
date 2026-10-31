/* TC
 * id:    exp_neg_malloc_huge
 * area:  exports
 * kind:  negative
 * funcs: malloc calloc realloc free
 * procs: _errno
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.3 (failure returns null pointer, original block untouched); MS docs malloc, calloc, realloc
 * note:  Impossible allocation sizes return NULL; realloc failure must leave the original block valid.
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
}


TC_CASE(malloc_huge)
{
    volatile size_t n = (size_t)-1;
    void *p = malloc(n);
    CHECK(p == NULL);
    n = (size_t)-1 - 64;
    p = malloc(n);
    CHECK(p == NULL);
}

TC_CASE(calloc_overflow)
{
    volatile size_t n = (size_t)-1 / 2 + 1;
    void *p = calloc(n, 2);
    CHECK(p == NULL);
    p = calloc(n, 4);
    CHECK(p == NULL);
}

TC_CASE(realloc_huge_keeps_block)
{
    volatile size_t n = (size_t)-1;
    char *p = (char *)malloc(16), *q;
    CHECK(p != NULL);
    if (!p) return;
    memset(p, 0x5A, 16);
    q = (char *)realloc(p, n);
    CHECK(q == NULL);
    CHECK_EQ_U((unsigned char)p[0], 0x5A);
    CHECK_EQ_U((unsigned char)p[15], 0x5A);
    free(p);
}

TC_CASE(free_null_ok)
{
    free(NULL);
    CHECK(1);
}

int main(void)
{
    tc_begin();
    touch_imports();
    TC_RUN(malloc_huge);
    TC_RUN(calloc_overflow);
    TC_RUN(realloc_huge_keeps_block);
    TC_RUN(free_null_ok);
    return tc_end();
}
