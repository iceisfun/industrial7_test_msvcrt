/* TC
 * id:    exp_neg_aligned_malloc_bad_alignment
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _aligned_malloc _aligned_free _aligned_realloc _errno
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _aligned_malloc (alignment must be an integer power of two; EINVAL), _aligned_free (NULL ignored)
 * note:  Alignment not a power of two returns NULL with errno EINVAL and invokes the invalid-parameter handler.
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

typedef void *(__cdecl *am_t)(size_t, size_t);
static am_t am;
static int *_errno_p(void) { return ((int *(__cdecl *)(void))tc_crt_proc("_errno"))(); }

TC_CASE(not_power_of_two)
{
    void *p;
    int *e = _errno_p();
    if (!am) TC_SKIP("missing");
    tc_iph_count = 0; *e = 0;
    p = am(64, 3);
    CHECK(p == NULL);
    CHECK_EQ_I(*e, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}

TC_CASE(free_null_ok)
{
    void (__cdecl *af)(void *) = (void (__cdecl *)(void *))tc_crt_proc("_aligned_free");
    if (!af) TC_SKIP("missing");
    af(NULL);
    CHECK(1);
}

int main(void)
{
    tc_begin();
    am = (am_t)tc_crt_proc("_aligned_malloc");
    tc_iph_install();
    TC_RUN(not_power_of_two);
    TC_RUN(free_null_ok);
    return tc_end();
}
