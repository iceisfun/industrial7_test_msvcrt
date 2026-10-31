/* TC
 * id:    gsj_probe_neg_alloc_base_errno_enomem
 * area:  exit_signal
 * kind:  negative
 * funcs:
 * procs: _malloc_base _calloc_base _free_base
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs malloc/calloc ("ENOMEM: insufficient memory")
 * note:  PROBING that the undocumented _base entry points set errno=ENOMEM on failure (docs state it for malloc/calloc) and
 *        leave errno alone on success.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(void *, _malloc_base, (size_t));
TC_DECL_PROC(void *, _calloc_base, (size_t, size_t));
TC_DECL_PROC(void, _free_base, (void *));
static volatile size_t v_n = (size_t)-1, v_two = 2, v_16 = 16;

TC_CASE(malloc_base_enomem)
{
    void *p; if (!tc_p__malloc_base) TC_SKIP("no _malloc_base");
    errno = 0; p = tc_p__malloc_base(v_n);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, ENOMEM);
}
TC_CASE(calloc_base_enomem)
{
    void *p; if (!tc_p__calloc_base) TC_SKIP("no _calloc_base");
    errno = 0; p = tc_p__calloc_base(v_n, v_two);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, ENOMEM);
}
TC_CASE(success_keeps_errno)
{
    void *p, *q; if (!tc_p__malloc_base || !tc_p__calloc_base) TC_SKIP("no _base allocators");
    errno = EINVAL; p = tc_p__malloc_base(v_16); q = tc_p__calloc_base(v_16, v_two);
    CHECK(p != NULL); CHECK(q != NULL);
    CHECK_EQ_I(errno, EINVAL);
    tc_p__free_base(p); tc_p__free_base(q);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_malloc_base); TC_LOAD_PROC(_calloc_base); TC_LOAD_PROC(_free_base);
    TC_RUN(malloc_base_enomem); TC_RUN(calloc_base_enomem); TC_RUN(success_keeps_errno);
    return tc_end();
}
