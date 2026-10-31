/* TC
 * id:    mem_memicmp_l_basic
 * area:  memory
 * funcs:
 * procs: _memicmp_l
 * crt:   >=80
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _memicmp, _memicmp_l
 * note:  Resolved via procs: old import libs lack it. _create_locale(LC_ALL,"C") resolved via procs too.
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *memicmp_l_t)(const void *, const void *, size_t, void *);
typedef void *(__cdecl *create_locale_t)(int, const char *);
typedef void (__cdecl *free_locale_t)(void *);
static memicmp_l_t p_memicmp_l;
static create_locale_t p_create;
static free_locale_t p_free;

TC_CASE(exported) { CHECK(p_memicmp_l != NULL); CHECK(p_create != NULL); }
TC_CASE(equal_ignoring_case)
{
    static char a[] = "MixedCase", b[] = "mIXEDcASE"; void *loc; volatile size_t n = 9;
    if (!p_memicmp_l || !p_create) TC_SKIP("missing");
    loc = p_create(0 /* LC_ALL */, "C"); CHECK(loc != NULL);
    CHECK_EQ_I(p_memicmp_l(a, b, n, loc), 0);
    b[8] = 'x'; CHECK(p_memicmp_l(a, b, n, loc) != 0);
    if (loc && p_free) p_free(loc);
}
TC_CASE(zero_count)
{
    static char a[] = "a", b[] = "z"; void *loc; volatile size_t n = 0;
    if (!p_memicmp_l || !p_create) TC_SKIP("missing");
    loc = p_create(0, "C");
    CHECK_EQ_I(p_memicmp_l(a, b, n, loc), 0);
    if (loc && p_free) p_free(loc);
}
int main(void)
{
    tc_begin();
    p_memicmp_l = (memicmp_l_t)tc_crt_proc("_memicmp_l");
    p_create = (create_locale_t)tc_crt_proc("_create_locale");
    p_free = (free_locale_t)tc_crt_proc("_free_locale");
    TC_RUN(exported); TC_RUN(equal_ignoring_case); TC_RUN(zero_count);
    return tc_end();
}
