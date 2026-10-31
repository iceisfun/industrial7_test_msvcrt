/* TC
 * id:    exp_neg_beginthread_null
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _beginthread _beginthreadex _errno
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _beginthread: returns -1 on error, errno EINVAL for invalid argument; _beginthreadex returns 0 on error
 * note:  Invalid-parameter handler installed (the argument is validated).
 */
#include "tc.h"

typedef uintptr_t (__cdecl *bt_t)(void *, unsigned, void *);
typedef uintptr_t (__cdecl *bte_t)(void *, unsigned, void *, void *, unsigned, unsigned *);
typedef int *(__cdecl *en_t)(void);

TC_CASE(beginthread_null)
{
    bt_t b = (bt_t)tc_crt_proc("_beginthread"); en_t en = (en_t)tc_crt_proc("_errno");
    CHECK(b && en);
    if (!(b && en)) return;
    *en() = 0;
    CHECK((intptr_t)b(NULL, 0, NULL) == -1);
    CHECK_EQ_I(*en(), 22);
}
TC_CASE(beginthreadex_null)
{
    bte_t b = (bte_t)tc_crt_proc("_beginthreadex"); en_t en = (en_t)tc_crt_proc("_errno");
    CHECK(b && en);
    if (!(b && en)) return;
    *en() = 0;
    CHECK_EQ_U(b(NULL, 0, NULL, NULL, 0, NULL), 0);
    CHECK_EQ_I(*en(), 22);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(beginthread_null); TC_RUN(beginthreadex_null); return tc_end(); }
