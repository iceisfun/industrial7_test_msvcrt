/* TC
 * id:    dat_neg_data_pointers_independent
 * area:  data_exports
 * funcs: __p__fmode _get_fmode
 * procs: __p__commode
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _fmode, _commode, _get_fmode
 * note:  Isolation check: writing _fmode must not change _commode and vice versa. Values are restored.
 */
#include "tc.h"
#include <stdlib.h>

/* mingw lacks a prototype for __p__commode: resolve it from the DLL */
typedef int *(__cdecl *pcommode_t)(void);
static pcommode_t p_commode;
#define __p__commode p_commode

TC_CASE(pointers_distinct)
{
    int *f = __p__fmode(), *c = __p__commode();
    CHECK(f != NULL); CHECK(c != NULL); CHECK(f != c);
}
TC_CASE(fmode_write_leaves_commode)
{
    int *f = __p__fmode(), *c = __p__commode();
    int of = *f, oc = *c, g = -1;
    *f = 0x8000;
    CHECK_EQ_I(*c, oc);
    CHECK_EQ_I(_get_fmode(&g), 0); CHECK_EQ_I(g, 0x8000);
    *f = of;
}
TC_CASE(commode_write_leaves_fmode)
{
    int *f = __p__fmode(), *c = __p__commode();
    int of = *f, oc = *c, g = -1;
    *c = 0x4321;
    CHECK_EQ_I(*f, of);
    CHECK_EQ_I(_get_fmode(&g), 0); CHECK_EQ_I(g, of);
    *c = oc;
}
int main(void) { tc_begin(); p_commode = (pcommode_t)tc_crt_proc("__p__commode"); if (!p_commode) { tc_write("TC FAIL __p__commode missing\n"); return 1; }
    TC_RUN(pointers_distinct); TC_RUN(fmode_write_leaves_commode); TC_RUN(commode_write_leaves_fmode); return tc_end(); }
